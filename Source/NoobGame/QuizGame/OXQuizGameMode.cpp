#include "OXQuizGameMode.h"
#include "OXQuizGameState.h"
#include "OXQuizPlayerController.h"
#include "OXQuizPlayerState.h"
#include "QuizObstacleBase.h"
#include "NoobPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AOXQuizGameMode::AOXQuizGameMode()
{
	GameStateClass = AOXQuizGameState::StaticClass();
	PlayerStateClass = AOXQuizPlayerState::StaticClass();
	PlayerControllerClass = AOXQuizPlayerController::StaticClass();
	MyGameState = nullptr;
}

void AOXQuizGameMode::PostLogin(APlayerController* NewPlayer)
{
	// 1. 부모 클래스 로직 실행 (기본적인 방장 설정은 여기서 수행됨)
	Super::PostLogin(NewPlayer);

	if (!MyGameState) MyGameState = GetGameState<AOXQuizGameState>();

	// ──────────────────────────────────────────────────────────────────────────
	// [New] 방장 권한 명시적 재설정 (안전장치)
	// ──────────────────────────────────────────────────────────────────────────
	if (GetNumPlayers() == 1)
	{
		if (AOXQuizPlayerState* PS = NewPlayer->GetPlayerState<AOXQuizPlayerState>())
		{
			// 1번 플레이어면 무조건 방장으로 설정
			PS->bIsRoomOwner = true;
			UE_LOG(LogTemp, Warning, TEXT("[OX GM] Player %s explicitly assigned as Room Owner."), *NewPlayer->GetName());

		}
	}

	// ──────────────────────────────────────────────────────────────────────────
	// 2. 2명이 모였을 때 게임 페이즈 전환
	// ──────────────────────────────────────────────────────────────────────────
	if (MyGameState && GetNumPlayers() == 2)
	{
		if (MyGameState->CurrentGamePhase == EQuizGamePhase::GP_WaitingToStart)
		{
			UE_LOG(LogTemp, Warning, TEXT("[OX GM] 2 Players detected. Phase set to GP_Ready."));
			MyGameState->CurrentGamePhase = EQuizGamePhase::GP_Instructions;
		}
	}
}

bool AOXQuizGameMode::IsGameInProgress() const
{
	return MyGameState && MyGameState->CurrentGamePhase == EQuizGamePhase::GP_Playing;
}

void AOXQuizGameMode::AnnounceWinnerToClients(APlayerState* Winner)
{
	if (!MyGameState) return;
	GetWorldTimerManager().ClearTimer(TimerHandle_SpawnQuiz);
	GetWorldTimerManager().ClearTimer(TimerHandle_GamePhase);

	FString WinnerName = Winner ? Winner->GetPlayerName() : TEXT("Draw");
	MyGameState->Multicast_AnnounceWinner(WinnerName);
}

void AOXQuizGameMode::CleanupLevelActors()
{
	TArray<AActor*> Obstacles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AQuizObstacleBase::StaticClass(), Obstacles);
	for (AActor* A : Obstacles) A->Destroy();
}

void AOXQuizGameMode::EndGame(APlayerState* Winner)
{
	if (!MyGameState || MyGameState->CurrentGamePhase == EQuizGamePhase::GP_GameOver) return;
	MyGameState->CurrentGamePhase = EQuizGamePhase::GP_GameOver;
	MyGameState->Winner = Winner;
	Super::EndGame(Winner);
}

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
			if (NPS->bIsReady_Instructions) {
				UE_LOG(LogTemp, Warning, TEXT("[OX GM] Ready Cnt Up++."));
				++ReadyCnt;
			}
		}
	}
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
	if (RemainingPlayingCountdown <= 0)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_GamePhase);
		StartQuizSpawning();
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

	for (FQuizData* Row : AllRows)
	{
		if (Row && Row->Difficulty == CurrentGameDifficulty) RemainingQuizList.Add(*Row);
	}
	if (RemainingQuizList.Num() == 0)
	{
		for (FQuizData* Row : AllRows) if (Row) RemainingQuizList.Add(*Row);
	}
}

void AOXQuizGameMode::SpawnNextQuizObstacle()
{
	if (!MyGameState || MyGameState->CurrentGamePhase != EQuizGamePhase::GP_Playing) return;

	FQuizData Quiz;
	if (RemainingQuizList.Num() > 0)
	{
		int32 Idx = FMath::RandRange(0, RemainingQuizList.Num() - 1);
		Quiz = RemainingQuizList[Idx];
		RemainingQuizList.RemoveAt(Idx);

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
				if (SpawnedQuizCount % 5 == 0 && SpeedLevels.IsValidIndex(CurrentSpeedLevelIndex + 1))
				{
					CurrentSpeedLevelIndex++;
					CurrentMoveSpeed = SpeedLevels[CurrentSpeedLevelIndex];
					if (MyGameState) MyGameState->SetCurrentSpeedLevel(CurrentSpeedLevelIndex + 1);
				}
				GetWorldTimerManager().SetTimer(TimerHandle_SpawnQuiz, this, &AOXQuizGameMode::SpawnNextQuizObstacle, TimeBetweenSpawns, false);
			}
		}
	}
	else
	{
		LoadQuizListByDifficulty();
		GetWorldTimerManager().SetTimer(TimerHandle_SpawnQuiz, this, &AOXQuizGameMode::SpawnNextQuizObstacle, 0.1f, false);
	}
}