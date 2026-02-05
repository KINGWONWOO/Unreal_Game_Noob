#include "OXQuizGameMode.h"
#include "OXQuizGameState.h"
#include "OXQuizPlayerController.h"
#include "OXQuizPlayerState.h"
#include "QuizObstacleBase.h"
#include "NoobPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// =============================================================
// 1. 초기화 및 플레이어 로그인 (Initialization)
// =============================================================

AOXQuizGameMode::AOXQuizGameMode()
{
    GameStateClass = AOXQuizGameState::StaticClass();
    PlayerStateClass = AOXQuizPlayerState::StaticClass();
    PlayerControllerClass = AOXQuizPlayerController::StaticClass();
    MyGameState = nullptr;
}

void AOXQuizGameMode::PostLogin(APlayerController* NewPlayer)
{
    // 부모 클래스의 기본 방장 설정 로직 실행
    Super::PostLogin(NewPlayer);

    if (!MyGameState) MyGameState = GetGameState<AOXQuizGameState>();

    // 1번 플레이어 접속 시 방장 권한 명시적 부여
    if (GetNumPlayers() == 1)
    {
        if (AOXQuizPlayerState* PS = NewPlayer->GetPlayerState<AOXQuizPlayerState>())
        {
            PS->bIsRoomOwner = true;
            UE_LOG(LogTemp, Warning, TEXT("[OX GM] Player %s explicitly assigned as Room Owner."), *NewPlayer->GetName());
        }
    }

    // 2명이 모두 모이면 대기 상태에서 지침(Instructions) 단계로 전환
    if (MyGameState && GetNumPlayers() == 2)
    {
        if (MyGameState->CurrentGamePhase == EQuizGamePhase::GP_WaitingToStart)
        {
            MyGameState->CurrentGamePhase = EQuizGamePhase::GP_Instructions;
        }
    }
}

bool AOXQuizGameMode::IsGameInProgress() const
{
    return MyGameState && MyGameState->CurrentGamePhase == EQuizGamePhase::GP_Playing;
}

// =============================================================
// 2. 준비 확인 및 카운트다운 (Ready & Countdown)
// =============================================================

void AOXQuizGameMode::PlayerIsReady(AController* PlayerController)
{
    if (!MyGameState || MyGameState->CurrentGamePhase != EQuizGamePhase::GP_Instructions) return;

    if (AOXQuizPlayerState* PS = PlayerController->GetPlayerState<AOXQuizPlayerState>())
    {
        PS->SetInstructionReady_Server();
        CheckBothPlayersReady_Instruction();
    }
}

void AOXQuizGameMode::CheckBothPlayersReady_Instruction()
{
    if (!MyGameState) return;
    int32 ReadyCnt = 0;
    for (APlayerState* PS : MyGameState->PlayerArray)
    {
        if (AOXQuizPlayerState* NPS = Cast<AOXQuizPlayerState>(PS))
        {
            if (NPS->bIsReady_Instructions) ++ReadyCnt;
        }
    }

    // 두 명 모두 준비되면 게임 플레이 시작 및 카운트다운 진입
    if (ReadyCnt == 2)
    {
        MyGameState->CurrentGamePhase = EQuizGamePhase::GP_Playing;
        RemainingPlayingCountdown = PlayingStartCountdownDuration;
        if (MyGameState) MyGameState->SetPlayingCountdown(RemainingPlayingCountdown);

        GetWorldTimerManager().SetTimer(TimerHandle_GamePhase, this, &AOXQuizGameMode::UpdatePlayingCountdown, 1.f, true);
    }
}

void AOXQuizGameMode::UpdatePlayingCountdown()
{
    --RemainingPlayingCountdown;
    if (MyGameState) MyGameState->SetPlayingCountdown(RemainingPlayingCountdown);

    // 카운트다운 종료 시 실제 퀴즈 스폰 시작
    if (RemainingPlayingCountdown <= 0)
    {
        GetWorldTimerManager().ClearTimer(TimerHandle_GamePhase);
        StartQuizSpawning();
    }
}

// =============================================================
// 3. 퀴즈 스폰 및 로직 (Core Gameplay)
// =============================================================

void AOXQuizGameMode::SetGameDifficulty(EQuizDifficulty NewDifficulty)
{
    CurrentGameDifficulty = NewDifficulty;
    if (MyGameState) MyGameState->SetRepDifficulty(NewDifficulty);
}

void AOXQuizGameMode::LoadQuizListByDifficulty()
{
    if (!QuizDataTable) return;
    RemainingQuizList.Empty();
    TArray<FQuizData*> AllRows;
    static const FString ContextString(TEXT("LoadQuizListByDifficulty"));
    QuizDataTable->GetAllRows(ContextString, AllRows);

    // 설정된 난이도에 맞는 퀴즈만 필터링하여 리스트 생성
    for (FQuizData* Row : AllRows)
    {
        if (Row && Row->Difficulty == CurrentGameDifficulty) RemainingQuizList.Add(*Row);
    }

    // 해당 난이도의 퀴즈가 없으면 전체 퀴즈를 로드
    if (RemainingQuizList.Num() == 0)
    {
        for (FQuizData* Row : AllRows) if (Row) RemainingQuizList.Add(*Row);
    }
}

void AOXQuizGameMode::StartQuizSpawning()
{
    if (!QuizDataTable)
    {
        MyGameState->CurrentGamePhase = EQuizGamePhase::GP_GameOver;
        return;
    }

    LoadQuizListByDifficulty();
    SpawnedQuizCount = 0;
    CurrentSpeedLevelIndex = 0;
    CurrentMoveSpeed = SpeedLevels.IsValidIndex(0) ? SpeedLevels[0] : 400.f;

    if (MyGameState) MyGameState->SetCurrentSpeedLevel(CurrentSpeedLevelIndex + 1);

    SpawnNextQuizObstacle();
}

void AOXQuizGameMode::SpawnNextQuizObstacle()
{
    if (!MyGameState || MyGameState->CurrentGamePhase != EQuizGamePhase::GP_Playing) return;

    FQuizData Quiz;
    if (RemainingQuizList.Num() > 0)
    {
        // 랜덤하게 퀴즈 선택 후 리스트에서 제거
        int32 Idx = FMath::RandRange(0, RemainingQuizList.Num() - 1);
        Quiz = RemainingQuizList[Idx];
        RemainingQuizList.RemoveAt(Idx);

        // 정답 선택지 수에 따라 스폰할 클래스 결정
        TSubclassOf<AQuizObstacleBase> ObstacleClass = (Quiz.Answers.Num() == 2) ? QuizObstacleClass_2Choice : QuizObstacleClass_3Choice;

        if (ObstacleClass)
        {
            FActorSpawnParameters SP;
            SP.Owner = this;
            SP.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

            if (AQuizObstacleBase* NewObs = GetWorld()->SpawnActor<AQuizObstacleBase>(ObstacleClass, ObstacleSpawnTransform, SP))
            {
                NewObs->InitializeObstacle(Quiz, CurrentMoveSpeed);
                ++SpawnedQuizCount;

                // 5문제마다 속도 레벨업
                if (SpawnedQuizCount % 5 == 0 && SpeedLevels.IsValidIndex(CurrentSpeedLevelIndex + 1))
                {
                    CurrentSpeedLevelIndex++;
                    CurrentMoveSpeed = SpeedLevels[CurrentSpeedLevelIndex];
                    if (MyGameState) MyGameState->SetCurrentSpeedLevel(CurrentSpeedLevelIndex + 1);
                }

                // 일정 간격으로 다음 퀴즈 스폰 타이머 설정
                GetWorldTimerManager().SetTimer(TimerHandle_SpawnQuiz, this, &AOXQuizGameMode::SpawnNextQuizObstacle, TimeBetweenSpawns, false);
            }
        }
    }
    else
    {
        // 퀴즈를 다 썼으면 다시 보충 후 즉시 스폰 요청
        LoadQuizListByDifficulty();
        GetWorldTimerManager().SetTimer(TimerHandle_SpawnQuiz, this, &AOXQuizGameMode::SpawnNextQuizObstacle, 0.1f, false);
    }
}

// =============================================================
// 4. 종료 및 정리 (Termination & Cleanup)
// =============================================================

void AOXQuizGameMode::EndGame(APlayerState* Winner)
{
    if (!MyGameState || MyGameState->CurrentGamePhase == EQuizGamePhase::GP_GameOver) return;
    MyGameState->CurrentGamePhase = EQuizGamePhase::GP_GameOver;
    MyGameState->Winner = Winner;
    Super::EndGame(Winner);
}

void AOXQuizGameMode::AnnounceWinnerToClients(APlayerState* Winner)
{
    if (!MyGameState) return;

    // 실행 중인 모든 타이머 중단
    GetWorldTimerManager().ClearTimer(TimerHandle_SpawnQuiz);
    GetWorldTimerManager().ClearTimer(TimerHandle_GamePhase);

    FString WinnerName = Winner ? Winner->GetPlayerName() : TEXT("Draw");
    MyGameState->Multicast_AnnounceWinner(WinnerName);
}

void AOXQuizGameMode::CleanupLevelActors()
{
    // 월드에 남아있는 모든 퀴즈 장애물 제거
    TArray<AActor*> Obstacles;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AQuizObstacleBase::StaticClass(), Obstacles);
    for (AActor* A : Obstacles) A->Destroy();
}