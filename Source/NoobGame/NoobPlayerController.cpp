#include "NoobPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/Input/SVirtualJoystick.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraActor.h"
#include "NoobGameModeBase.h"
#include "NoobGame/NoobGameCharacter.h"
#include "NoobPlayerState.h"

void ANoobPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (SVirtualJoystick::ShouldDisplayTouchInterface() && IsLocalPlayerController())
	{
		if (MobileControlsWidgetClass)
		{
			MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);
			if (MobileControlsWidget) MobileControlsWidget->AddToPlayerScreen(0);
		}
	}
}

void ANoobPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (IsLocalPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}
			if (!SVirtualJoystick::ShouldDisplayTouchInterface())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}

void ANoobPlayerController::RequestPunch(ACharacter* HitCharacter) { Server_RequestPunch(HitCharacter); }
void ANoobPlayerController::RequestPlayPunchMontage() { Server_RequestPlayPunchMontage(); }

bool ANoobPlayerController::Server_RequestPunch_Validate(ACharacter* HitCharacter) { return true; }
void ANoobPlayerController::Server_RequestPunch_Implementation(ACharacter* HitCharacter)
{
	if (ANoobGameModeBase* GM = GetWorld()->GetAuthGameMode<ANoobGameModeBase>())
	{
		GM->ProcessPunch(this, HitCharacter);
	}
}

bool ANoobPlayerController::Server_RequestPlayPunchMontage_Validate() { return true; }
void ANoobPlayerController::Server_RequestPlayPunchMontage_Implementation()
{
	ANoobGameCharacter* MyChar = Cast<ANoobGameCharacter>(GetPawn());
	ANoobPlayerState* MyPS = GetPlayerState<ANoobPlayerState>();
	ANoobGameModeBase* GM = GetWorld()->GetAuthGameMode<ANoobGameModeBase>();

	if (MyChar && MyPS && GM)
	{
		bool bIsLeft = MyPS->bIsNextPunchLeft;
		MyPS->bIsNextPunchLeft = !bIsLeft;
		UAnimMontage* Montage = bIsLeft ? MyChar->LeftPunchMontage : MyChar->RightPunchMontage;
		GM->ProcessPunchAnimation(MyChar, Montage);
	}
}

void ANoobPlayerController::Multicast_PlayHitReaction_Implementation(ACharacter* TargetCharacter, UAnimMontage* MontageToPlay)
{
	if (TargetCharacter && MontageToPlay) TargetCharacter->PlayAnimMontage(MontageToPlay);
}

void ANoobPlayerController::Multicast_PlayPunchMontage_Implementation(ACharacter* PunchingCharacter, UAnimMontage* MontageToPlay)
{
	if (PunchingCharacter && MontageToPlay) PunchingCharacter->PlayAnimMontage(MontageToPlay);
}

void ANoobPlayerController::Client_SetCameraEffect_Implementation(bool bEnableKnockdownEffect)
{
	ApplyKnockdownCameraEffect(bEnableKnockdownEffect);
}

bool ANoobPlayerController::Server_SetupEnding_Validate(bool bIsWinner, FVector TargetLocation, FRotator TargetRotation, ECharacterType WinnerType, ACameraActor* EndingCamera) { return true; }
void ANoobPlayerController::Server_SetupEnding_Implementation(bool bIsWinner, FVector TargetLocation, FRotator TargetRotation, ECharacterType WinnerType, ACameraActor* EndingCamera)
{
	ANoobGameCharacter* MyPawn = Cast<ANoobGameCharacter>(GetPawn());
	if (!MyPawn) return;

	MyPawn->SetRagdollState_Server(false);
	if (auto* MoveComp = MyPawn->GetCharacterMovement())
	{
		MoveComp->SetMovementMode(EMovementMode::MOVE_Walking);
		MoveComp->StopMovementImmediately();
	}
	MyPawn->TeleportTo(TargetLocation, TargetRotation, false, true);

	Client_SetUIOnlyInput(bIsWinner, WinnerType, EndingCamera);

	ANoobGameModeBase* GM = GetWorld()->GetAuthGameMode<ANoobGameModeBase>();
	UAnimMontage* MontageToPlay = bIsWinner ? MyPawn->VictoryMontage : MyPawn->DefeatMontage;

	if (GM && MontageToPlay)
	{
		if (UAnimInstance* AnimInst = MyPawn->GetMesh()->GetAnimInstance())
		{
			AnimInst->OnMontageEnded.AddUniqueDynamic(this, &ANoobPlayerController::OnEndingMontageEnded);
			GM->ProcessPunchAnimation(MyPawn, MontageToPlay);
		}
		else
		{
			Client_EnableMovementAfterEnding();
		}
	}
	else
	{
		Client_EnableMovementAfterEnding();
	}
}

void ANoobPlayerController::Client_SetUIOnlyInput_Implementation(bool bYouWon, ECharacterType WinnerType, ACameraActor* EndingCamera)
{
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;

	if (EndingCamera) SetViewTargetWithBlend(EndingCamera, 0.5f);

	OnGameOver.Broadcast(bYouWon);
	Event_ShowResultsScreen(WinnerType, bYouWon);
}

void ANoobPlayerController::Client_EnableMovementAfterEnding_Implementation()
{
	SetInputMode(FInputModeGameAndUI());
	bShowMouseCursor = true;
}

void ANoobPlayerController::OnEndingMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		ANoobGameCharacter* MyPawn = Cast<ANoobGameCharacter>(GetPawn());
		if (MyPawn && (Montage == MyPawn->VictoryMontage || Montage == MyPawn->DefeatMontage))
		{
			if (UAnimInstance* AnimInst = MyPawn->GetMesh()->GetAnimInstance())
			{
				AnimInst->OnMontageEnded.RemoveDynamic(this, &ANoobPlayerController::OnEndingMontageEnded);
			}
			Client_EnableMovementAfterEnding();
		}
	}
}