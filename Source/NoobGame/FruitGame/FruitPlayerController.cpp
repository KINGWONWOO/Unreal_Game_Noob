#include "FruitGame/FruitPlayerController.h"
#include "FruitGame/FruitGameMode.h"
#include "FruitGame/FruitPlayerState.h" // PlayerState 접근을 위해
#include "FruitGame/FruitGameState.h"
#include "NoobGame/NoobGameCharacter.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"


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

/** [기존] 게임 종료 시 UI 모드로 전환하는 RPC 구현 (카메라 참조 사용) */
void AFruitPlayerController::Client_SetUIOnlyInput_Implementation(bool bIsWinner, ECharacterType WinnerType, ACameraActor* EndingCamera)
{
	UE_LOG(LogTemp, Warning, TEXT("[Client %s] Client_SetUIOnlyInput CALLED. bYouWon: %s"), *GetName(), bIsWinner ? TEXT("True") : TEXT("False"));

	// [핵심] FInputModeGameAndUI: UI 클릭 + 게임(펀치, 이동) 입력 모두 받음
	SetInputMode(FInputModeGameAndUI());
	bShowMouseCursor = true;

	// C++에서 직접 카메라를 설정합니다.
	if (EndingCamera)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Client %s] Setting View Target to %s"), *GetName(), *EndingCamera->GetName());
		SetViewTargetWithBlend(EndingCamera, 0.5f); // 0.5초 블렌딩
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[Client %s] EndingCamera reference was NULL!"), *GetName());
	}

	// 델리게이트 및 BP 이벤트 호출
	OnGameOver.Broadcast(bIsWinner);
	// Event_SetupResultsScreen(); // C++로 카메라를 옮겼으므로 주석 처리
	Event_ShowResultsScreen(WinnerType, bIsWinner); // UI 표시는 BP가 계속 담당
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

/** 래그돌 카메라 효과 (비네팅) */
void AFruitPlayerController::Client_SetCameraEffect_Implementation(bool bEnableKnockdownEffect)
{
	ApplyKnockdownCameraEffect(bEnableKnockdownEffect);
}


// --- 3. 클라이언트 -> 서버 RPC 구현 ---

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


bool AFruitPlayerController::Server_SetupEnding_Validate(bool bIsWinner, FVector TargetLocation, FRotator TargetRotation, ECharacterType WinnerType, ACameraActor* EndingCamera) { return true; }
void AFruitPlayerController::Server_SetupEnding_Implementation(bool bIsWinner, FVector TargetLocation, FRotator TargetRotation, ECharacterType WinnerType, ACameraActor* EndingCamera)
{
	// [서버 실행]
	UE_LOG(LogTemp, Warning, TEXT("[Server PC %s] Executing Server_SetupEnding. bIsWinner: %s"), *GetName(), bIsWinner ? TEXT("True") : TEXT("False"));

	ANoobGameCharacter* MyPawn = Cast<ANoobGameCharacter>(GetPawn());
	if (!MyPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("[Server PC %s] ... FAILED to GetPawn()!"), *GetName());
		return;
	}

	// [안전 장치] (K.O. 래그돌 강제 해제)
	MyPawn->SetRagdollState_Server(false);

	UCharacterMovementComponent* MovementComp = MyPawn->GetCharacterMovement();
	if (MovementComp)
	{
		// [핵심] 이동 모드를 'Walking'으로 강제 설정 (절대 잠그지 않음)
		MovementComp->SetMovementMode(EMovementMode::MOVE_Walking);
		// (안전 장치) 현재 속도를 0으로 만듭니다.
		MovementComp->StopMovementImmediately();
	}

	// 텔레포트합니다.
	bool bTeleportSuccess = MyPawn->TeleportTo(TargetLocation, TargetRotation, false, true);
	UE_LOG(LogTemp, Warning, TEXT("[Server PC %s] Teleport attempt result: %s"), *GetName(), bTeleportSuccess ? TEXT("Success") : TEXT("Failed"));

	// 클라이언트에게 카메라 참조를 포함하여 RPC 호출
	Client_SetUIOnlyInput(bIsWinner, WinnerType, EndingCamera);
}
