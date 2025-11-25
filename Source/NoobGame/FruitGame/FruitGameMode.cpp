#include "FruitGame/FruitGameMode.h"
#include "FruitGame/FruitGameState.h"
#include "FruitGame/FruitPlayerState.h"
#include "FruitGame/FruitPlayerController.h"
#include "FruitGame/InteractableFruitObject.h"
#include "FruitGame/SubmitGuessButton.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AFruitGameMode::AFruitGameMode()
{
	GameStateClass = AFruitGameState::StaticClass();
	PlayerStateClass = AFruitPlayerState::StaticClass();
	PlayerControllerClass = AFruitPlayerController::StaticClass();
	MyGameState = nullptr;
}

void AFruitGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (!MyGameState) MyGameState = GetGameState<AFruitGameState>();
	if (MyGameState && GetNumPlayers() == 2)
	{
		MyGameState->CurrentGamePhase = EFruitGamePhase::GP_Instructions;
	}
}

bool AFruitGameMode::IsGameInProgress() const
{
	return MyGameState && MyGameState->CurrentGamePhase != EFruitGamePhase::GP_GameOver;
}

bool AFruitGameMode::IsPlayerTurn(AController* PlayerController) const
{
	if (!MyGameState || !PlayerController || !PlayerController->PlayerState) return false;
	return (MyGameState->CurrentActivePlayer == PlayerController->PlayerState);
}

void AFruitGameMode::AnnounceWinnerToClients(APlayerState* Winner)
{
	if (!MyGameState) return;
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	GetWorldTimerManager().ClearTimer(GuessResultTimerHandle);

	FString WinnerName = Winner ? Winner->GetPlayerName() : TEXT("Draw");
	MyGameState->Multicast_AnnounceWinner(WinnerName);
}

void AFruitGameMode::EndGame(APlayerState* Winner)
{
	if (!MyGameState || MyGameState->CurrentGamePhase == EFruitGamePhase::GP_GameOver) return;
	MyGameState->CurrentGamePhase = EFruitGamePhase::GP_GameOver;
	MyGameState->Winner = Winner;
	MyGameState->ServerTimeAtTurnStart = 0.0f;

	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	GetWorldTimerManager().ClearTimer(GuessResultTimerHandle);

	Super::EndGame(Winner);
}

void AFruitGameMode::PlayerIsReady(AController* PlayerController)
{
	if (!MyGameState || MyGameState->CurrentGamePhase != EFruitGamePhase::GP_Instructions) return;
	if (AFruitPlayerState* PS = PlayerController->GetPlayerState<AFruitPlayerState>())
	{
		PS->SetInstructionReady_Server();
		CheckBothPlayersReady_Instructions();
	}
}

void AFruitGameMode::CheckBothPlayersReady_Instructions()
{
	if (!MyGameState) return;
	int32 ReadyPlayers = 0;
	for (APlayerState* PS : MyGameState->PlayerArray)
	{
		if (AFruitPlayerState* FruitPS = Cast<AFruitPlayerState>(PS))
		{
			if (FruitPS->bIsReady_Instructions) ReadyPlayers++;
		}
	}
	if (ReadyPlayers == 2) MyGameState->CurrentGamePhase = EFruitGamePhase::GP_Setup;
}

void AFruitGameMode::PlayerSubmittedFruits(AController* PlayerController, const TArray<EFruitType>& SecretFruits)
{
	if (!MyGameState || MyGameState->CurrentGamePhase != EFruitGamePhase::GP_Setup) return;
	AFruitPlayerState* PS = PlayerController->GetPlayerState<AFruitPlayerState>();
	if (PS && !PS->bHasSubmittedFruits)
	{
		PS->SetSecretAnswers_Server(SecretFruits);
		NumPlayersReady_Setup++;
		CheckBothPlayersReady_Setup();
	}
}

void AFruitGameMode::CheckBothPlayersReady_Setup()
{
	if (NumPlayersReady_Setup == 2) StartSpinnerPhase();
}

void AFruitGameMode::StartSpinnerPhase()
{
	if (!MyGameState) return;
	MyGameState->CurrentGamePhase = EFruitGamePhase::GP_SpinnerTurn;
	SpinnerResultIndex = FMath::RandRange(0, 1);
	for (APlayerState* PS : MyGameState->PlayerArray)
	{
		AFruitPlayerController* PC = Cast<AFruitPlayerController>(PS->GetPlayerController());
		if (PC) PC->Client_PlaySpinnerAnimation(SpinnerResultIndex);
	}
}

void AFruitGameMode::PlayerRequestsStartTurn(AController* PlayerController)
{
	if (!MyGameState || MyGameState->CurrentGamePhase != EFruitGamePhase::GP_SpinnerTurn || SpinnerResultIndex == -1) return;
	if (MyGameState->CurrentGamePhase == EFruitGamePhase::GP_PlayerTurn) return;

	MyGameState->CurrentActivePlayer = MyGameState->PlayerArray[SpinnerResultIndex];
	MyGameState->CurrentGamePhase = EFruitGamePhase::GP_PlayerTurn;
	StartTurn();
}

void AFruitGameMode::StartTurn()
{
	if (!MyGameState || !MyGameState->CurrentActivePlayer) return;
	MyGameState->ServerTimeAtTurnStart = GetWorld()->GetTimeSeconds();
	GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &AFruitGameMode::OnTurnTimerExpired, TurnDuration, false);
	AFruitPlayerController* ActivePC = Cast<AFruitPlayerController>(MyGameState->CurrentActivePlayer->GetPlayerController());
	if (ActivePC) ActivePC->Client_StartTurn();
}

void AFruitGameMode::OnTurnTimerExpired() { EndTurn(true); }

void AFruitGameMode::PlayerInteracted(AController* PlayerController, AActor* HitActor, EFruitGamePhase CurrentPhase)
{
	if (CurrentPhase == EFruitGamePhase::GP_PlayerTurn)
	{
		if (!IsPlayerTurn(PlayerController)) return;
		if (AInteractableFruitObject* GuessObject = Cast<AInteractableFruitObject>(HitActor))
		{
			GuessObject->CycleFruit();
		}
		else if (ASubmitGuessButton* GuessSubmitButton = Cast<ASubmitGuessButton>(HitActor))
		{
			ProcessGuessFromWorldObjects(PlayerController);
		}
	}
}

void AFruitGameMode::ProcessGuessFromWorldObjects(AController* PlayerController)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractableFruitObject::StaticClass(), FoundActors);
	if (FoundActors.Num() != 5) return;

	FoundActors.Sort([](const AActor& A, const AActor& B) {
		const AInteractableFruitObject* ObjA = Cast<AInteractableFruitObject>(&A);
		const AInteractableFruitObject* ObjB = Cast<AInteractableFruitObject>(&B);
		if (ObjA && ObjB) return ObjA->GuessIndex < ObjB->GuessIndex;
		return false;
		});

	TArray<EFruitType> GuessedFruits;
	GuessedFruits.Init(EFruitType::FT_None, 5);
	bool bAllValid = true;
	for (AActor* Actor : FoundActors)
	{
		AInteractableFruitObject* FruitObject = Cast<AInteractableFruitObject>(Actor);
		if (FruitObject && GuessedFruits.IsValidIndex(FruitObject->GuessIndex))
		{
			GuessedFruits[FruitObject->GuessIndex] = FruitObject->CurrentFruit;
		}
		else bAllValid = false;
	}
	if (bAllValid) ProcessPlayerGuess(PlayerController, GuessedFruits);
}

void AFruitGameMode::ProcessPlayerGuess(AController* PlayerController, const TArray<EFruitType>& GuessedFruits)
{
	if (!IsPlayerTurn(PlayerController)) return;

	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	if (MyGameState) MyGameState->ServerTimeAtTurnStart = 0.0f;

	AFruitPlayerState* OpponentPS = nullptr;
	for (APlayerState* PS : MyGameState->PlayerArray)
	{
		if (PS != PlayerController->PlayerState)
		{
			OpponentPS = Cast<AFruitPlayerState>(PS);
			break;
		}
	}
	if (!OpponentPS) return;

	const TArray<EFruitType>& OpponentSecret = OpponentPS->GetSecretAnswers_Server();
	int32 MatchCount = 0;
	for (int32 i = 0; i < 5; ++i)
	{
		if (GuessedFruits.IsValidIndex(i) && OpponentSecret.IsValidIndex(i) &&
			GuessedFruits[i] == OpponentSecret[i] && GuessedFruits[i] != EFruitType::FT_None)
		{
			MatchCount++;
		}
	}

	AFruitPlayerController* GuesserPC = Cast<AFruitPlayerController>(PlayerController);
	AFruitPlayerController* OpponentPC = Cast<AFruitPlayerController>(OpponentPS->GetPlayerController());
	if (GuesserPC) GuesserPC->Client_ReceiveGuessResult(GuessedFruits, MatchCount);
	if (OpponentPC) OpponentPC->Client_OpponentGuessed(GuessedFruits, MatchCount);

	if (MatchCount == 5)
	{
		StartWinnerAnnouncement(PlayerController->PlayerState);
	}
	else
	{
		GetWorldTimerManager().SetTimer(GuessResultTimerHandle, this, &AFruitGameMode::OnGuessResultDelayExpired, GuessResultDisplayTime, false);
	}
}

void AFruitGameMode::OnGuessResultDelayExpired()
{
	EndTurn(false);
}

void AFruitGameMode::EndTurn(bool bTimeOut)
{
	if (!MyGameState) return;
	if (bTimeOut) MyGameState->ServerTimeAtTurnStart = 0.0f;

	GetWorldTimerManager().ClearTimer(GuessResultTimerHandle);

	APlayerState* NextPlayer = nullptr;
	for (APlayerState* PS : MyGameState->PlayerArray)
	{
		if (PS != MyGameState->CurrentActivePlayer)
		{
			NextPlayer = PS;
			break;
		}
	}
	if (NextPlayer)
	{
		MyGameState->CurrentActivePlayer = NextPlayer;
		StartTurn();
	}
	else EndGame(nullptr);
}