#include "NoobGameModeBase.h"
#include "NoobGameStateBase.h"
#include "NoobPlayerState.h"
#include "NoobPlayerController.h"
#include "NoobGame/NoobGameCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "Camera/CameraActor.h"
#include "TimerManager.h"
#include "Engine/World.h"

ANoobGameModeBase::ANoobGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;
	GameStateClass = ANoobGameStateBase::StaticClass();
	PlayerStateClass = ANoobPlayerState::StaticClass();
	PlayerControllerClass = ANoobPlayerController::StaticClass();
	bHasAssignedRoomOwner = false;
}

void ANoobGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (ANoobPlayerState* PS = NewPlayer->GetPlayerState<ANoobPlayerState>())
	{
#if WITH_EDITOR 
		FString DebugName = (GetNumPlayers() == 1) ? TEXT("Cat") : TEXT("Dog");
		PS->SetPlayerName(DebugName);
#endif
		if (!bHasAssignedRoomOwner || GetNumPlayers() == 1)
		{
			PS->bIsRoomOwner = true;
			bHasAssignedRoomOwner = true;

			// 서버 로그 확인용
			UE_LOG(LogTemp, Warning, TEXT("[Server] Player %s is assigned as Room Owner."), *NewPlayer->GetName());

			// [중요] 호스트(Listen Server)인 경우 OnRep이 호출되지 않으므로 수동 호출
			if (NewPlayer->IsLocalController())
			{
				PS->OnRep_IsRoomOwner();
			}
		}
		else
		{
			PS->bIsRoomOwner = false;
			UE_LOG(LogTemp, Warning, TEXT("[Server] Player %s joined as Guest."), *NewPlayer->GetName());
		}
	}
}

void ANoobGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (IsGameInProgress())
	{
		HandlePlayerDisconnect(Exiting);
	}
}

void ANoobGameModeBase::HandlePlayerDisconnect(AController* ExitingPlayer)
{
	if (GetWorldTimerManager().IsTimerActive(EndGameDelayTimerHandle)) return;

	APlayerState* WinnerState = nullptr;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (AController* Other = It->Get(); Other && Other != ExitingPlayer)
		{
			WinnerState = Other->GetPlayerState<APlayerState>();
			break;
		}
	}
	StartWinnerAnnouncement(WinnerState);
}

void ANoobGameModeBase::StartWinnerAnnouncement(APlayerState* Winner)
{
	AnnounceWinnerToClients(Winner);

	if (ANoobGameStateBase* GS = GetGameState<ANoobGameStateBase>())
	{
		FString WinnerName = Winner ? Winner->GetPlayerName() : TEXT("Draw");
		if (Winner && Winner->GetPawn())
		{
			if (Winner->GetPawn()->ActorHasTag(FName("Cat"))) WinnerName = TEXT("Cat");
			else if (Winner->GetPawn()->ActorHasTag(FName("Dog"))) WinnerName = TEXT("Dog");
		}
		GS->Multicast_AnnounceWinner(WinnerName);
	}

	GetWorldTimerManager().ClearTimer(EndGameDelayTimerHandle);
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("EndGame"), Winner);
	GetWorldTimerManager().SetTimer(EndGameDelayTimerHandle, TimerDel, WinnerAnnouncementDuration, false);
}

void ANoobGameModeBase::EndGame(APlayerState* Winner)
{
	GetWorldTimerManager().ClearTimer(EndGameDelayTimerHandle);
	for (auto& TimerPair : KnockdownTimers) GetWorldTimerManager().ClearTimer(TimerPair.Value);
	KnockdownTimers.Empty();

	CleanupLevelActors();

	if (ANoobGameStateBase* GS = GetGameState<ANoobGameStateBase>())
	{
		GS->Winner = Winner;
	}

	APlayerState* Loser = nullptr;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (APlayerState* PS = It->Get()->GetPlayerState<APlayerState>())
		{
			if (PS != Winner) { Loser = PS; break; }
		}
	}

	ECharacterType WinType = ECharacterType::ECT_Cat;
	if (Winner && Winner->GetPawn() && Winner->GetPawn()->ActorHasTag(FName("Dog")))
		WinType = ECharacterType::ECT_Dog;

	TArray<AActor*> WinSpawns, DefeatSpawns, Cameras;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATargetPoint::StaticClass(), WinnerSpawnTag, WinSpawns);
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATargetPoint::StaticClass(), LoserSpawnTag, DefeatSpawns);
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACameraActor::StaticClass(), EndingCameraTag, Cameras);

	AActor* WinPt = WinSpawns.IsValidIndex(0) ? WinSpawns[0] : nullptr;
	AActor* LosePt = DefeatSpawns.IsValidIndex(0) ? DefeatSpawns[0] : nullptr;
	ACameraActor* Cam = Cameras.IsValidIndex(0) ? Cast<ACameraActor>(Cameras[0]) : nullptr;

	if (Winner)
	{
		if (ANoobPlayerController* WPC = Cast<ANoobPlayerController>(Winner->GetPlayerController()))
			if (WinPt && Cam) WPC->Server_SetupEnding(true, WinPt->GetActorLocation(), WinPt->GetActorRotation(), WinType, Cam);
	}
	if (Loser)
	{
		if (ANoobPlayerController* LPC = Cast<ANoobPlayerController>(Loser->GetPlayerController()))
			if (LosePt && Cam) LPC->Server_SetupEnding(false, LosePt->GetActorLocation(), LosePt->GetActorRotation(), WinType, Cam);
	}
}

void ANoobGameModeBase::ProcessPunch(APlayerController* PuncherController, ACharacter* HitCharacter)
{
	if (!IsGameInProgress() || !HitCharacter || !PuncherController) return;

	ANoobPlayerState* HitPS = HitCharacter->GetController() ? HitCharacter->GetController()->GetPlayerState<ANoobPlayerState>() : nullptr;
	if (!HitPS || HitPS->bIsKnockedDown) return;

	const FVector PunchDir = (HitCharacter->GetActorLocation() - PuncherController->GetPawn()->GetActorLocation()).GetSafeNormal();
	HitCharacter->GetCharacterMovement()->AddImpulse(PunchDir * PunchPushForce, true);

	HitPS->PunchHitCount++;

	if (HitPS->PunchHitCount >= 10)
	{
		HitPS->bIsKnockedDown = true;
		HitPS->PunchHitCount = 0;
		if (ANoobGameCharacter* HitChar = Cast<ANoobGameCharacter>(HitCharacter)) HitChar->SetRagdollState_Server(true);
		if (ANoobPlayerController* HitPC = Cast<ANoobPlayerController>(HitCharacter->GetController())) HitPC->Client_SetCameraEffect(true);

		FTimerHandle KDTimer;
		FTimerDelegate Del = FTimerDelegate::CreateUObject(this, &ANoobGameModeBase::RecoverCharacter, HitCharacter);
		GetWorldTimerManager().SetTimer(KDTimer, Del, KnockdownDuration, false);
		KnockdownTimers.Add(HitCharacter, KDTimer);
	}
	else
	{
		ANoobGameCharacter* HitChar = Cast<ANoobGameCharacter>(HitCharacter);
		if (!HitChar) return;

		const FVector HitVector = (HitCharacter->GetActorLocation() - PuncherController->GetPawn()->GetActorLocation()).GetSafeNormal();
		float ForwardDot = FVector::DotProduct(HitVector, HitChar->GetActorForwardVector());
		float RightDot = FVector::DotProduct(HitVector, HitChar->GetActorRightVector());

		UAnimMontage* Selected = nullptr;
		if (FMath::Abs(ForwardDot) > FMath::Abs(RightDot))
			Selected = (ForwardDot > 0) ? HitChar->HitReaction_Back : HitChar->HitReaction_Front;
		else
			Selected = (RightDot > 0) ? HitChar->HitReaction_Right : HitChar->HitReaction_Left;

		ProcessPunchAnimation(HitCharacter, Selected ? Selected : HitChar->HitReaction_Front);
	}
}

void ANoobGameModeBase::ProcessPunchAnimation(ACharacter* PunchingCharacter, UAnimMontage* MontageToPlay)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ANoobPlayerController* NPC = Cast<ANoobPlayerController>(It->Get()))
			NPC->Multicast_PlayPunchMontage(PunchingCharacter, MontageToPlay);
	}
}

void ANoobGameModeBase::RecoverCharacter(ACharacter* CharacterToRecover)
{
	KnockdownTimers.Remove(CharacterToRecover);
	if (!IsGameInProgress() || !CharacterToRecover || !CharacterToRecover->GetController()) return;

	ANoobPlayerState* NoobPS = CharacterToRecover->GetController()->GetPlayerState<ANoobPlayerState>();
	if (NoobPS && NoobPS->bIsKnockedDown)
	{
		NoobPS->bIsKnockedDown = false;
		if (ANoobGameCharacter* C = Cast<ANoobGameCharacter>(CharacterToRecover)) C->SetRagdollState_Server(false);
		if (ANoobPlayerController* PC = Cast<ANoobPlayerController>(CharacterToRecover->GetController())) PC->Client_SetCameraEffect(false);
	}
}