#include "FruitGame/FruitPlayerController.h"
#include "FruitGame/FruitGameMode.h"
#include "FruitGame/FruitGameState.h"
#include "FruitGame/InteractableFruitObject.h"

// =============================================================
// 1. 로컬 API 구현 (Internal Logic & UI Interface)
// =============================================================

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

// =============================================================
// 2. Server RPC 구현 (Request to Server)
// =============================================================

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

// =============================================================
// 3. Client RPC 구현 (Response from Server)
// =============================================================

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