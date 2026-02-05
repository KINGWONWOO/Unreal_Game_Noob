#include "MazeGameMode.h"
#include "MazeGameState.h"
#include "MazePlayerState.h"
#include "MazePlayerController.h"
#include "MazeStartPoint.h"
#include "NoobPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "TimerManager.h"

// =============================================================
// 1. 초기화 및 플레이어 입장 (Initialization & Login)
// =============================================================

AMazeGameMode::AMazeGameMode()
{
    GameStateClass = AMazeGameState::StaticClass();
    PlayerStateClass = AMazePlayerState::StaticClass();
    PlayerControllerClass = AMazePlayerController::StaticClass();
    MyGameState = nullptr;
    PlayingStartCountdownDuration = 3;
}

void AMazeGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    if (!MyGameState) MyGameState = GetGameState<AMazeGameState>();

    // 최초 접속자를 방장(Owner)으로 설정
    if (AMazePlayerState* MPS = NewPlayer->GetPlayerState<AMazePlayerState>())
    {
        if (GetNumPlayers() == 1) MPS->bIsRoomOwner = true;
    }

    if (MyGameState && GetNumPlayers() == 2)
    {
        FString CurrentMapName = GetWorld()->GetMapName();
        // 로비 맵이면 지침 단계로, 게임 맵이면 즉시 플레이 단계로 진입
        if (CurrentMapName.Contains("Lvl_MazeSelect"))
        {
            if (MyGameState->CurrentGamePhase == EMazeGamePhase::GP_WaitingToStart)
                MyGameState->CurrentGamePhase = EMazeGamePhase::GP_Instructions;
        }
        else StartPlayingPhase();
    }
}

// =============================================================
// 2. 게임 준비 및 단계 전환 (Ready & Phase Control)
// =============================================================

void AMazeGameMode::PlayerIsReady(AController* PlayerController)
{
    if (!MyGameState || MyGameState->CurrentGamePhase != EMazeGamePhase::GP_Instructions) return;

    if (ANoobPlayerState* PS = PlayerController->GetPlayerState<ANoobPlayerState>())
    {
        PS->SetInstructionReady_Server();
        CheckBothPlayersReady();
    }
}

void AMazeGameMode::CheckBothPlayersReady()
{
    if (!MyGameState) return;
    int32 ReadyCnt = 0;
    for (APlayerState* PS : MyGameState->PlayerArray)
    {
        if (ANoobPlayerState* NPS = Cast<ANoobPlayerState>(PS))
        {
            if (NPS->bIsReady_Instructions) ++ReadyCnt;
        }
    }

    if (ReadyCnt == 2)
    {
        MyGameState->CurrentGamePhase = EMazeGamePhase::GP_MapSelection;
    }
}

// =============================================================
// 3. 미로 생성 및 게임 시작 (Maze Generation & Start)
// =============================================================

void AMazeGameMode::StartPlayingPhase()
{
    if (!MyGameState || MyGameState->CurrentGamePhase == EMazeGamePhase::GP_Playing) return;

    // ServerTravel 시 전달된 옵션에서 맵 크기 파싱
    FString CurrentOptions = OptionsString;
    FString SizeOpt = UGameplayStatics::ParseOption(CurrentOptions, TEXT("MapSize"));

    if (SizeOpt.Equals(TEXT("Small"), ESearchCase::IgnoreCase)) MyGameState->MapSize = EMazeMapSize::Small;
    else if (SizeOpt.Equals(TEXT("Big"), ESearchCase::IgnoreCase)) MyGameState->MapSize = EMazeMapSize::Big;
    else MyGameState->MapSize = EMazeMapSize::Medium;

    // 게임 페이즈 전환 및 랜덤 시드 동기화
    MyGameState->CurrentGamePhase = EMazeGamePhase::GP_Playing;
    int32 NewRandomSeed = FMath::RandRange(1, 999999);
    MyGameState->SetMazeSeed(NewRandomSeed);

    // 카운트다운 설정
    RemainingPlayingCountdown = PlayingStartCountdownDuration;
    MyGameState->SetPlayingCountdown(RemainingPlayingCountdown);

    // 플레이어 배치 및 이동 입력 제한
    TArray<AActor*> FoundPlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundPlayerStarts);

    int32 StartIndex = 0;
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (APlayerController* PC = It->Get())
        {
            PC->SetIgnoreMoveInput(true);
            if (APawn* Pawn = PC->GetPawn())
            {
                if (FoundPlayerStarts.Num() > 0)
                {
                    AActor* SelectedStart = FoundPlayerStarts[StartIndex % FoundPlayerStarts.Num()];
                    FVector SpawnPos = SelectedStart->GetActorLocation();
                    FRotator SpawnRot = SelectedStart->GetActorRotation();

                    Pawn->SetActorLocationAndRotation(SpawnPos, SpawnRot, false, nullptr, ETeleportType::TeleportPhysics);
                    PC->SetControlRotation(SpawnRot);
                    StartIndex++;
                }
            }
        }
    }
    GetWorldTimerManager().SetTimer(TimerHandle_GamePhase, this, &AMazeGameMode::UpdatePlayingCountdown, 1.0f, true);
}

// =============================================================
// 4. 게임 진행 및 카운트다운 (In-Game Logic)
// =============================================================

void AMazeGameMode::UpdatePlayingCountdown()
{
    RemainingPlayingCountdown--;
    if (MyGameState) MyGameState->SetPlayingCountdown(RemainingPlayingCountdown);

    if (RemainingPlayingCountdown <= 0)
    {
        GetWorldTimerManager().ClearTimer(TimerHandle_GamePhase);
        EnablePlayerMovement();
    }
}

void AMazeGameMode::EnablePlayerMovement()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (APlayerController* PC = It->Get()) PC->SetIgnoreMoveInput(false);
    }
}

bool AMazeGameMode::IsGameInProgress() const
{
    return MyGameState && MyGameState->CurrentGamePhase == EMazeGamePhase::GP_Playing;
}

// =============================================================
// 5. 종료 판정 및 처리 (Game Over)
// =============================================================

void AMazeGameMode::ProcessPlayerReachedGoal(AController* WinnerController)
{
    if (!MyGameState || MyGameState->CurrentGamePhase != EMazeGamePhase::GP_Playing) return;
    if (RemainingPlayingCountdown > 0) return; // 카운트다운 중 도착 무시

    if (WinnerController) StartWinnerAnnouncement(WinnerController->PlayerState);
}

void AMazeGameMode::AnnounceWinnerToClients(APlayerState* Winner)
{
    if (!MyGameState) return;
    GetWorldTimerManager().ClearTimer(TimerHandle_GamePhase);

    FString WinnerName = Winner ? Winner->GetPlayerName() : TEXT("Draw");
    MyGameState->Multicast_AnnounceWinner(WinnerName);
}

void AMazeGameMode::EndGame(APlayerState* Winner)
{
    if (!MyGameState || MyGameState->CurrentGamePhase == EMazeGamePhase::GP_GameOver) return;
    MyGameState->CurrentGamePhase = EMazeGamePhase::GP_GameOver;
    MyGameState->Winner = Winner;
    GetWorldTimerManager().ClearTimer(TimerHandle_GamePhase);

    Super::EndGame(Winner);
}