#include "MazeGameMode.h"
#include "MazeGameState.h"
#include "MazePlayerState.h"
#include "MazePlayerController.h"
#include "MazeStartPoint.h"
#include "NoobPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"

AMazeGameMode::AMazeGameMode()
{
	GameStateClass = AMazeGameState::StaticClass();
	PlayerStateClass = AMazePlayerState::StaticClass();
	PlayerControllerClass = AMazePlayerController::StaticClass();
	MyGameState = nullptr;
	PlayingStartCountdownDuration = 3; // 3초 설정
}

void AMazeGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (!MyGameState) MyGameState = GetGameState<AMazeGameState>();

	if (MyGameState && GetNumPlayers() == 2)
	{
		if (MyGameState->CurrentGamePhase == EMazeGamePhase::GP_WaitingToStart)
		{
			UE_LOG(LogTemp, Warning, TEXT("[Maze GM] 2 Players detected. Phase set to GP_Description."));
			MyGameState->CurrentGamePhase = EMazeGamePhase::GP_Instructions;
		}
	}
}

bool AMazeGameMode::IsGameInProgress() const
{
	// GP_Playing 상태면 (카운트다운 중이어도) 게임 중으로 간주 (펀치 가능)
	return MyGameState && MyGameState->CurrentGamePhase == EMazeGamePhase::GP_Playing;
}

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
		UE_LOG(LogTemp, Warning, TEXT("[Maze GM] All Players Ready. Starting Countdown Sequence..."));

		// 1. 페이즈 변경 (OXQuiz처럼 즉시 변경 -> Ready UI 닫힘)
		MyGameState->CurrentGamePhase = EMazeGamePhase::GP_Playing;

		// 2. 카운트다운 초기화
		RemainingPlayingCountdown = PlayingStartCountdownDuration;
		MyGameState->SetPlayingCountdown(RemainingPlayingCountdown);

		// 3. 시작 지점 선정 및 플레이어 이동/잠금
		TArray<AActor*> StartPoints;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMazeStartPoint::StaticClass(), StartPoints);

		AActor* SelectedStartPoint = nullptr;
		if (StartPoints.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, StartPoints.Num() - 1);
			SelectedStartPoint = StartPoints[RandomIndex];
		}

		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (APlayerController* PC = It->Get())
			{
				if (APawn* Pawn = PC->GetPawn())
				{
					// 텔레포트
					if (SelectedStartPoint)
					{
						FVector SpawnLocation = SelectedStartPoint->GetActorLocation();
						SpawnLocation.X += FMath::RandRange(-50.0f, 50.0f);
						SpawnLocation.Y += FMath::RandRange(-50.0f, 50.0f);
						Pawn->TeleportTo(SpawnLocation, SelectedStartPoint->GetActorRotation());
						PC->SetControlRotation(SelectedStartPoint->GetActorRotation());
					}

					// [핵심] 이동 입력 잠금 (Freeze)
					PC->SetIgnoreMoveInput(true);
				}
			}
		}

		// 4. 카운트다운 타이머 시작 (1초 간격)
		GetWorldTimerManager().SetTimer(TimerHandle_GamePhase, this, &AMazeGameMode::UpdatePlayingCountdown, 1.0f, true);
	}
}

void AMazeGameMode::UpdatePlayingCountdown()
{
	RemainingPlayingCountdown--;

	if (MyGameState)
	{
		// UI 업데이트용 (3 -> 2 -> 1 -> 0)
		MyGameState->SetPlayingCountdown(RemainingPlayingCountdown);
	}

	if (RemainingPlayingCountdown <= 0)
	{
		// 카운트다운 종료 -> 게임 시작
		GetWorldTimerManager().ClearTimer(TimerHandle_GamePhase);
		EnablePlayerMovement();
	}
}

void AMazeGameMode::EnablePlayerMovement()
{
	UE_LOG(LogTemp, Warning, TEXT("[Maze GM] GO! Movement Enabled."));

	// 모든 플레이어 이동 잠금 해제
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (APlayerController* PC = It->Get())
		{
			PC->SetIgnoreMoveInput(false);
		}
	}
}

void AMazeGameMode::ProcessPlayerReachedGoal(AController* WinnerController)
{
	if (!MyGameState || MyGameState->CurrentGamePhase != EMazeGamePhase::GP_Playing) return;

	// 카운트다운 중(아직 이동 불가 상태)에 트리거에 닿는 경우 방지 (0 이하일 때만 승리 인정)
	if (RemainingPlayingCountdown > 0) return;

	UE_LOG(LogTemp, Warning, TEXT("[Maze GM] Player Reached Goal! Winner: %s"), *WinnerController->GetName());

	if (WinnerController)
	{
		StartWinnerAnnouncement(WinnerController->PlayerState);
	}
}

void AMazeGameMode::AnnounceWinnerToClients(APlayerState* Winner)
{
	if (!MyGameState) return;
	GetWorldTimerManager().ClearTimer(TimerHandle_GamePhase); // 타이머 정지

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