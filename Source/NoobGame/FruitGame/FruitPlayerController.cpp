#include "FruitGame/FruitPlayerController.h"
#include "FruitGame/FruitGameMode.h"
#include "FruitGame/FruitGameState.h"
#include "FruitGame/InteractableFruitObject.h"

void AFruitPlayerController::PlayerReady()
{
	Server_PlayerReady();
}

void AFruitPlayerController::SubmitSecretFruits(const TArray<EFruitType>& SecretFruits)
{
	MyLocalSecretAnswers = SecretFruits;
	Server_SubmitSecretFruits(SecretFruits);
}

void AFruitPlayerController::RequestInteract(AActor* HitActor)
{
	Server_RequestInteract(HitActor);
}

void AFruitPlayerController::RequestStartPlayerTurn()
{
	Server_RequestStartPlayerTurn();
}

const TArray<EFruitType>& AFruitPlayerController::GetMyLocalSecretAnswers() const
{
	return MyLocalSecretAnswers;
}

// --- RPC Implementation ---

void AFruitPlayerController::Client_StartTurn_Implementation()
{
	OnTurnStarted.Broadcast();
}

void AFruitPlayerController::Client_ReceiveGuessResult_Implementation(const TArray<EFruitType>& Guess, int32 MatchCount)
{
	OnGuessResultReceived.Broadcast(Guess, MatchCount);
}

void AFruitPlayerController::Client_OpponentGuessed_Implementation(const TArray<EFruitType>& Guess, int32 MatchCount)
{
	OnOpponentGuessReceived.Broadcast(Guess, MatchCount);
}

void AFruitPlayerController::Client_PlaySpinnerAnimation_Implementation(int32 WinningPlayerIndex)
{
	PlaySpinnerAnimationEvent(WinningPlayerIndex);
}

// --- Server RPCs ---
// 주의: GameMode 캐스팅은 여전히 필요 (Fruit만의 로직 호출)

bool AFruitPlayerController::Server_PlayerReady_Validate() { return true; }
void AFruitPlayerController::Server_PlayerReady_Implementation()
{
	if (AFruitGameMode* GM = GetWorld()->GetAuthGameMode<AFruitGameMode>())
	{
		GM->PlayerIsReady(this);
	}
}

bool AFruitPlayerController::Server_SubmitSecretFruits_Validate(const TArray<EFruitType>& SecretFruits) { return true; }
void AFruitPlayerController::Server_SubmitSecretFruits_Implementation(const TArray<EFruitType>& SecretFruits)
{
	MyLocalSecretAnswers = SecretFruits;
	if (AFruitGameMode* GM = GetWorld()->GetAuthGameMode<AFruitGameMode>())
	{
		GM->PlayerSubmittedFruits(this, SecretFruits);
	}
}

bool AFruitPlayerController::Server_RequestInteract_Validate(AActor* HitActor) { return true; }
void AFruitPlayerController::Server_RequestInteract_Implementation(AActor* HitActor)
{
	AFruitGameMode* GM = GetWorld()->GetAuthGameMode<AFruitGameMode>();
	AFruitGameState* GS = GetWorld()->GetGameState<AFruitGameState>();
	if (GM && GS && HitActor)
	{
		GM->PlayerInteracted(this, HitActor, GS->CurrentGamePhase);
	}
}

bool AFruitPlayerController::Server_RequestStartPlayerTurn_Validate() { return true; }
void AFruitPlayerController::Server_RequestStartPlayerTurn_Implementation()
{
	if (AFruitGameMode* GM = GetWorld()->GetAuthGameMode<AFruitGameMode>())
	{
		GM->PlayerRequestsStartTurn(this);
	}
}