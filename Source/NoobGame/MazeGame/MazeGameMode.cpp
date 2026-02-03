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

	// 방장(RoomOwner) 설정 로직
	if (AMazePlayerState* MPS = NewPlayer->GetPlayerState<AMazePlayerState>())
	{
		if (GetNumPlayers() == 1)
		{
			MPS->bIsRoomOwner = true;
		}
	}

	if (MyGameState && GetNumPlayers() == 2)
	{
		// 현재 맵 이름을 확인하여 로직 분기
		FString CurrentMapName = GetWorld()->GetMapName();

		// 맵 선택 로비(Selection) 인 경우
		if (CurrentMapName.Contains("Lvl_MazeSelect"))
		{
			if (MyGameState->CurrentGamePhase == EMazeGamePhase::GP_WaitingToStart)
			{
				MyGameState->CurrentGamePhase = EMazeGamePhase::GP_Instructions;
			}
		}
		// 실제 게임 맵으로 이동한 상태인 경우 (ServerTravel 이후 자동 시작)
		else
		{
			StartPlayingPhase();
		}
	}
}

void AMazeGameMode::StartPlayingPhase()
{
	if (!MyGameState || MyGameState->CurrentGamePhase == EMazeGamePhase::GP_Playing) return;

	// [수정] GetAddressURL 대신 GameMode가 가진 OptionsString을 직접 사용합니다.
	FString CurrentOptions = OptionsString;
	UE_LOG(LogTemp, Warning, TEXT("[Server] Received Options String: %s"), *CurrentOptions);

	// 1. MapSize 옵션 파싱 (UGameplayStatics::ParseOption 사용)
	FString SizeOpt = UGameplayStatics::ParseOption(CurrentOptions, TEXT("MapSize"));
	UE_LOG(LogTemp, Log, TEXT("[Server] Parsed MapSize Value: %s"), *SizeOpt);

	if (SizeOpt.Equals(TEXT("Small"), ESearchCase::IgnoreCase)) {
		MyGameState->MapSize = EMazeMapSize::Small;
		UE_LOG(LogTemp, Warning, TEXT("[Server] Final Decision: SMALL"));
	}
	else if (SizeOpt.Equals(TEXT("Big"), ESearchCase::IgnoreCase)) {
		MyGameState->MapSize = EMazeMapSize::Big;
		UE_LOG(LogTemp, Warning, TEXT("[Server] Final Decision: BIG"));
	}
	else {
		MyGameState->MapSize = EMazeMapSize::Medium;
		UE_LOG(LogTemp, Warning, TEXT("[Server] Final Decision: MEDIUM (Default)"));
	}

	// 2. 게임 페이즈 전환 및 시드 설정 (동기화 시작)
	MyGameState->CurrentGamePhase = EMazeGamePhase::GP_Playing;
	int32 NewRandomSeed = FMath::RandRange(1, 999999);
	MyGameState->SetMazeSeed(NewRandomSeed);

	RemainingPlayingCountdown = PlayingStartCountdownDuration;
	MyGameState->SetPlayingCountdown(RemainingPlayingCountdown);

	// 월드의 모든 PlayerStart 검색
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

					// [수정] TeleportTo 대신 SetActorLocationAndRotation을 사용하고
					// ETeleportType::TeleportPhysics 옵션을 주어 물리 충돌 처리를 건너뜁니다.
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

bool AMazeGameMode::IsGameInProgress() const
{
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
		UE_LOG(LogTemp, Warning, TEXT("[Maze GM] All Ready. Map Selection Phase."));
		MyGameState->CurrentGamePhase = EMazeGamePhase::GP_MapSelection;
	}
}

void AMazeGameMode::UpdatePlayingCountdown()
{
	RemainingPlayingCountdown--;

	if (MyGameState)
	{
		MyGameState->SetPlayingCountdown(RemainingPlayingCountdown);
	}

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
		if (APlayerController* PC = It->Get())
		{
			PC->SetIgnoreMoveInput(false);
		}
	}
}

void AMazeGameMode::ProcessPlayerReachedGoal(AController* WinnerController)
{
	if (!MyGameState || MyGameState->CurrentGamePhase != EMazeGamePhase::GP_Playing) return;
	if (RemainingPlayingCountdown > 0) return;

	if (WinnerController)
	{
		StartWinnerAnnouncement(WinnerController->PlayerState);
	}
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