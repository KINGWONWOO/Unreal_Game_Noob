// FruitGameMode.cpp

#include "FruitGame/FruitGameMode.h"
#include "FruitGame/FruitGameState.h"
#include "FruitGame/FruitPlayerState.h"
#include "FruitGame/FruitPlayerController.h"
#include "FruitGame/InteractableFruitObject.h" // Guessing object
#include "FruitGame/SubmitGuessButton.h"     // Guessing submit button
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h" // Needed for GetWorld()->GetTimeSeconds()
#include "TimerManager.h" // Needed for FTimerHandle

AFruitGameMode::AFruitGameMode()
{
	// Set default classes
	GameStateClass = AFruitGameState::StaticClass();
	PlayerStateClass = AFruitPlayerState::StaticClass();
	PlayerControllerClass = AFruitPlayerController::StaticClass();

	// GameMode doesn't need to tick anymore for the timer
	PrimaryActorTick.bCanEverTick = false;

	MyGameState = nullptr;
	NumPlayersReady_Setup = 0;
}

void AFruitGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Cache GameState reference
	if (!MyGameState)
	{
		MyGameState = GetGameState<AFruitGameState>();
	}

	// Check if 2 players are connected to start the Instructions phase
	if (MyGameState && GetNumPlayers() == 2)
	{
		MyGameState->CurrentGamePhase = EGamePhase::GP_Instructions;
	}
}

// Handles interaction requests coming from the PlayerController (originating from Character's F key)
void AFruitGameMode::PlayerInteracted(AController* PlayerController, AActor* HitActor, EGamePhase CurrentPhase)
{
	// --- Guessing Phase Logic ---
	if (CurrentPhase == EGamePhase::GP_PlayerTurn)
	{
		// Check if it's the interacting player's turn
		if (!IsPlayerTurn(PlayerController)) return;

		// Check if the hit actor is an InteractableFruitObject (for guessing)
		if (AInteractableFruitObject* GuessObject = Cast<AInteractableFruitObject>(HitActor))
		{
			// Tell the object to cycle its fruit (server call, replicates automatically)
			GuessObject->CycleFruit();
		}
		// Check if the hit actor is the SubmitGuessButton (for guessing)
		else if (ASubmitGuessButton* GuessSubmitButton = Cast<ASubmitGuessButton>(HitActor))
		{
			// Collect guesses from world objects and process the guess
			ProcessGuessFromWorldObjects(PlayerController);
		}
	}
	// (No logic needed here for GP_Setup, as it's handled by UI)
}

// --- 1. Instructions Phase ---
void AFruitGameMode::PlayerIsReady(AController* PlayerController)
{
	// Only proceed if in the correct game phase
	if (!MyGameState || MyGameState->CurrentGamePhase != EGamePhase::GP_Instructions)
	{
		return;
	}

	AFruitPlayerState* PS = PlayerController->GetPlayerState<AFruitPlayerState>();
	if (PS)
	{
		PS->SetInstructionReady_Server(); // Mark player as ready on the server
		CheckBothPlayersReady_Instructions(); // Check if both players are ready
	}
}

void AFruitGameMode::CheckBothPlayersReady_Instructions()
{
	if (!MyGameState) return;

	int32 ReadyPlayers = 0;
	for (APlayerState* PS : MyGameState->PlayerArray)
	{
		AFruitPlayerState* FruitPS = Cast<AFruitPlayerState>(PS);
		if (FruitPS && FruitPS->bIsReady_Instructions)
		{
			ReadyPlayers++;
		}
	}

	// If both players are ready, move to the Setup phase
	if (ReadyPlayers == 2)
	{
		MyGameState->CurrentGamePhase = EGamePhase::GP_Setup;
	}
}

// --- 2. Setup Phase ---
// Called by PlayerController's RPC when UI submits the secret fruits
void AFruitGameMode::PlayerSubmittedFruits(AController* PlayerController, const TArray<EFruitType>& SecretFruits)
{
	// Only proceed if in the correct game phase
	if (!MyGameState || MyGameState->CurrentGamePhase != EGamePhase::GP_Setup)
	{
		return;
	}

	AFruitPlayerState* PS = PlayerController->GetPlayerState<AFruitPlayerState>();
	// Ensure the player hasn't submitted already
	if (PS && !PS->bHasSubmittedFruits)
	{
		PS->SetSecretAnswers_Server(SecretFruits); // Store the secret answer securely on the server
		NumPlayersReady_Setup++;
		CheckBothPlayersReady_Setup(); // Check if both players have submitted
	}
}

void AFruitGameMode::CheckBothPlayersReady_Setup()
{
	// If both players submitted their answers, start the coin toss
	if (NumPlayersReady_Setup == 2)
	{
		StartCoinToss();
	}
}

// --- 3. PlayerTurn Phase ---
void AFruitGameMode::StartCoinToss()
{
	if (!MyGameState) return;

	MyGameState->CurrentGamePhase = EGamePhase::GP_PlayerTurn;

	// Randomly select the starting player
	FirstPlayerIndex = FMath::RandRange(0, 1);
	MyGameState->CurrentActivePlayer = MyGameState->PlayerArray[FirstPlayerIndex];

	StartTurn(); // Start the first turn
}

void AFruitGameMode::StartTurn()
{
	if (!MyGameState || !MyGameState->CurrentActivePlayer) return;

	// **Record the server time when the turn starts**
	MyGameState->ServerTimeAtTurnStart = GetWorld()->GetTimeSeconds();

	// **Set a timer for the actual timeout logic (independent of frame rate)**
	GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &AFruitGameMode::OnTurnTimerExpired, TurnDuration, false);

	// Notify the current player's client that their turn has started (for UI)
	AFruitPlayerController* ActivePC = Cast<AFruitPlayerController>(MyGameState->CurrentActivePlayer->GetPlayerController());
	if (ActivePC)
	{
		ActivePC->Client_StartTurn();
	}
}

// Called by the FTimerHandle when TurnDuration expires
void AFruitGameMode::OnTurnTimerExpired()
{
	EndTurn(true); // End the turn due to timeout
}

bool AFruitGameMode::IsPlayerTurn(AController* PlayerController) const
{
	if (!MyGameState || !PlayerController || !PlayerController->PlayerState)
	{
		return false;
	}
	// Check if the provided controller's PlayerState matches the active one in GameState
	return (MyGameState->CurrentActivePlayer == PlayerController->PlayerState);
}

// Collects fruit types from world objects and calls ProcessPlayerGuess
void AFruitGameMode::ProcessGuessFromWorldObjects(AController* PlayerController)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractableFruitObject::StaticClass(), FoundActors);

	if (FoundActors.Num() != 5)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProcessGuessFromWorldObjects: Did not find exactly 5 AInteractableFruitObject actors."));
		return;
	}

	// Sort actors based on their assigned index
	FoundActors.Sort([](const AActor& A, const AActor& B) {
		const AInteractableFruitObject* ObjA = Cast<AInteractableFruitObject>(&A);
		const AInteractableFruitObject* ObjB = Cast<AInteractableFruitObject>(&B);
		if (ObjA && ObjB) return ObjA->GuessIndex < ObjB->GuessIndex;
		return false;
		});

	// Build the guess array
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
		else
		{
			bAllValid = false;
			break; // Exit loop if any object is invalid
		}
	}

	if (bAllValid)
	{
		// Process the collected guess
		ProcessPlayerGuess(PlayerController, GuessedFruits);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ProcessGuessFromWorldObjects: Invalid GuessIndex found on one or more AInteractableFruitObject actors. Ensure indices are 0-4."));
	}
}

// Compares the guess with the opponent's secret answer
void AFruitGameMode::ProcessPlayerGuess(AController* PlayerController, const TArray<EFruitType>& GuessedFruits)
{
	// Double-check if it's the player's turn (security)
	if (!IsPlayerTurn(PlayerController))
	{
		return;
	}

	// **Stop the actual timeout timer**
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	// **Signal clients that the timer should display 0**
	if (MyGameState) MyGameState->ServerTimeAtTurnStart = 0.0f;

	// Find the opponent's PlayerState
	AFruitPlayerState* OpponentPS = nullptr;
	for (APlayerState* PS : MyGameState->PlayerArray)
	{
		if (PS != PlayerController->PlayerState)
		{
			OpponentPS = Cast<AFruitPlayerState>(PS);
			break;
		}
	}
	if (!OpponentPS) return; // Should not happen in a 2-player game

	// Get the secret answer (only available on the server)
	const TArray<EFruitType>& OpponentSecret = OpponentPS->GetSecretAnswers_Server();

	// Compare the guess with the secret
	int32 MatchCount = 0;
	for (int32 i = 0; i < 5; ++i)
	{
		// Ensure indices are valid and the fruit is not 'None'
		if (GuessedFruits.IsValidIndex(i) && OpponentSecret.IsValidIndex(i) &&
			GuessedFruits[i] == OpponentSecret[i] && GuessedFruits[i] != EFruitType::FT_None)
		{
			MatchCount++;
		}
	}

	// Send results back to both clients via RPCs
	AFruitPlayerController* GuesserPC = Cast<AFruitPlayerController>(PlayerController);
	AFruitPlayerController* OpponentPC = Cast<AFruitPlayerController>(OpponentPS->GetPlayerController());

	if (GuesserPC)
	{
		GuesserPC->Client_ReceiveGuessResult(GuessedFruits, MatchCount);
	}
	if (OpponentPC)
	{
		OpponentPC->Client_OpponentGuessed(GuessedFruits, MatchCount);
	}

	// Check for win condition or end the turn
	if (MatchCount == 5)
	{
		EndGame(PlayerController->PlayerState); // Guesser wins
	}
	else
	{
		EndTurn(false); // Guess submitted, not a timeout
	}
}

void AFruitGameMode::EndTurn(bool bTimeOut)
{
	if (!MyGameState) return;

	// **If the turn ended due to timeout, signal clients timer is 0**
	// (If not timeout, ProcessPlayerGuess already set it to 0)
	if (bTimeOut)
	{
		MyGameState->ServerTimeAtTurnStart = 0.0f;
	}

	// Find the next player
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
		StartTurn(); // Start the next turn
	}
	else
	{
		EndGame(nullptr); // Error or only one player left
	}
}

void AFruitGameMode::EndGame(APlayerState* Winner)
{
	if (!MyGameState) return;

	MyGameState->CurrentGamePhase = EGamePhase::GP_GameOver;
	MyGameState->Winner = Winner;

	// **Stop the actual timeout timer**
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	// **Signal clients that the timer should display 0**
	MyGameState->ServerTimeAtTurnStart = 0.0f;

	// Notify all clients about the game over state and winner
	for (APlayerState* PS : MyGameState->PlayerArray)
	{
		AFruitPlayerController* PC = Cast<AFruitPlayerController>(PS->GetPlayerController());
		if (PC)
		{
			PC->Client_GameOver(PS == Winner);
		}
	}

	// Optional: Add logic here to automatically return to lobby after a delay
}