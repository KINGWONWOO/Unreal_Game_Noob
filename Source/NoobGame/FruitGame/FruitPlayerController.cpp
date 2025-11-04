// FruitPlayerController.cpp

#include "FruitGame/FruitPlayerController.h"
#include "FruitGame/FruitGameMode.h"
#include "FruitGame/FruitPlayerState.h" 
#include "FruitGame/FruitGameState.h"
#include "NoobGame/NoobGameCharacter.h" 
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
// (삭제!) #include "GameFramework/SpringArmComponent.h"
// (삭제!) #include "Camera/CameraComponent.h" 

// --- 1. UI 및 캐릭터에서 호출할 함수들 (BlueprintCallable) ---

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

void AFruitPlayerController::RequestPunch(ACharacter* HitCharacter)
{
	Server_RequestPunch(HitCharacter);
}

void AFruitPlayerController::RequestPlayPunchMontage()
{
	Server_RequestPlayPunchMontage();
}


// --- 2. 서버 -> 클라이언트 RPC 구현 ---

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

void AFruitPlayerController::Client_GameOver_Implementation(bool bYouWon)
{
	OnGameOver.Broadcast(bYouWon);
}

void AFruitPlayerController::Client_PlaySpinnerAnimation_Implementation(int32 WinningPlayerIndex)
{
	PlaySpinnerAnimationEvent(WinningPlayerIndex);
}

void AFruitPlayerController::Multicast_PlayHitReaction_Implementation(ACharacter* TargetCharacter, UAnimMontage* MontageToPlay)
{
	if (TargetCharacter && MontageToPlay)
	{
		TargetCharacter->PlayAnimMontage(MontageToPlay);
	}
}

void AFruitPlayerController::Multicast_PlayPunchMontage_Implementation(ACharacter* PunchingCharacter, UAnimMontage* MontageToPlay)
{
	if (PunchingCharacter && MontageToPlay)
	{
		PunchingCharacter->PlayAnimMontage(MontageToPlay);
	}
}

/** (수정!) 카메라 효과 Client RPC 구현부 (C++ 로직 모두 삭제) */
void AFruitPlayerController::Client_SetCameraEffect_Implementation(bool bEnableKnockdownEffect)
{
	// (삭제!) C++에서 카메라/스프링암을 제어하던 로직을 모두 삭제합니다.

	// 비네팅과 카메라 높이 조절은 모두 블루프린트에서 처리하도록 이벤트만 호출합니다.
	ApplyKnockdownCameraEffect(bEnableKnockdownEffect);
}


// --- 3. 클라이언트 -> 서버 RPC 구현 ---

// PlayerReady
bool AFruitPlayerController::Server_PlayerReady_Validate() { return true; }
void AFruitPlayerController::Server_PlayerReady_Implementation()
{
	AFruitGameMode* GM = GetWorld()->GetAuthGameMode<AFruitGameMode>();
	if (GM)
	{
		GM->PlayerIsReady(this);
	}
}

// SubmitSecretFruits
bool AFruitPlayerController::Server_SubmitSecretFruits_Validate(const TArray<EFruitType>& SecretFruits) { return true; }
void AFruitPlayerController::Server_SubmitSecretFruits_Implementation(const TArray<EFruitType>& SecretFruits)
{
	MyLocalSecretAnswers = SecretFruits;

	AFruitGameMode* GM = GetWorld()->GetAuthGameMode<AFruitGameMode>();
	if (GM)
	{
		GM->PlayerSubmittedFruits(this, SecretFruits);
	}
}

// RequestInteract
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

// RequestStartPlayerTurn
bool AFruitPlayerController::Server_RequestStartPlayerTurn_Validate() { return true; }
void AFruitPlayerController::Server_RequestStartPlayerTurn_Implementation()
{
	AFruitGameMode* GM = GetWorld()->GetAuthGameMode<AFruitGameMode>();
	if (GM)
	{
		GM->PlayerRequestsStartTurn(this);
	}
}

// RequestPunch (피격 처리)
bool AFruitPlayerController::Server_RequestPunch_Validate(ACharacter* HitCharacter) { return true; }
void AFruitPlayerController::Server_RequestPunch_Implementation(ACharacter* HitCharacter)
{
	AFruitGameMode* GM = GetWorld()->GetAuthGameMode<AFruitGameMode>();
	if (GM && HitCharacter)
	{
		GM->ProcessPunch(this, HitCharacter);
	}
}

// RequestPlayPunchMontage (애니메이션 재생)
bool AFruitPlayerController::Server_RequestPlayPunchMontage_Validate() { return true; }
void AFruitPlayerController::Server_RequestPlayPunchMontage_Implementation()
{
	ANoobGameCharacter* MyCharacter = Cast<ANoobGameCharacter>(GetPawn());
	AFruitPlayerState* MyPlayerState = GetPlayerState<AFruitPlayerState>();
	AFruitGameMode* GM = GetWorld()->GetAuthGameMode<AFruitGameMode>();

	if (MyCharacter && MyPlayerState && GM)
	{
		bool bIsLeft = MyPlayerState->bIsNextPunchLeft;
		MyPlayerState->bIsNextPunchLeft = !bIsLeft;

		UAnimMontage* MontageToPlay = bIsLeft ? MyCharacter->LeftPunchMontage : MyCharacter->RightPunchMontage;

		GM->ProcessPunchAnimation(MyCharacter, MontageToPlay);
	}
}