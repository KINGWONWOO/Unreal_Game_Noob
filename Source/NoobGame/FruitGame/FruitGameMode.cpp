// FruitGameMode.cpp

#include "FruitGame/FruitGameMode.h"
#include "FruitGame/FruitGameState.h"
#include "FruitGame/FruitPlayerState.h"
#include "FruitGame/FruitPlayerController.h"
#include "FruitGame/InteractableFruitObject.h"
#include "FruitGame/SubmitGuessButton.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h" 
#include "GameFramework/Character.h" 
#include "NoobGame/NoobGameCharacter.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "Animation/AnimMontage.h"

AFruitGameMode::AFruitGameMode()
{
	GameStateClass = AFruitGameState::StaticClass();
	PlayerStateClass = AFruitPlayerState::StaticClass();
	PlayerControllerClass = AFruitPlayerController::StaticClass();
	PrimaryActorTick.bCanEverTick = false;
	MyGameState = nullptr;
	NumPlayersReady_Setup = 0;
	SpinnerResultIndex = -1;
	PunchPushForce = 50000.0f;
	KnockdownDuration = 3.0f;
}

void AFruitGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (!MyGameState)
	{
		MyGameState = GetGameState<AFruitGameState>();
	}
	if (MyGameState && GetNumPlayers() == 2)
	{
		MyGameState->CurrentGamePhase = EGamePhase::GP_Instructions;
	}
}

void AFruitGameMode::PlayerInteracted(AController* PlayerController, AActor* HitActor, EGamePhase CurrentPhase)
{
	if (CurrentPhase == EGamePhase::GP_PlayerTurn)
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

// --- 1. Instructions 단계 ---
void AFruitGameMode::PlayerIsReady(AController* PlayerController)
{
	if (!MyGameState || MyGameState->CurrentGamePhase != EGamePhase::GP_Instructions) return;
	AFruitPlayerState* PS = PlayerController->GetPlayerState<AFruitPlayerState>();
	if (PS)
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
		AFruitPlayerState* FruitPS = Cast<AFruitPlayerState>(PS);
		if (FruitPS && FruitPS->bIsReady_Instructions)
		{
			ReadyPlayers++;
		}
	}
	if (ReadyPlayers == 2)
	{
		MyGameState->CurrentGamePhase = EGamePhase::GP_Setup;
	}
}

// --- 2. Setup 단계 ---
void AFruitGameMode::PlayerSubmittedFruits(AController* PlayerController, const TArray<EFruitType>& SecretFruits)
{
	if (!MyGameState || MyGameState->CurrentGamePhase != EGamePhase::GP_Setup) return;
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
	if (NumPlayersReady_Setup == 2)
	{
		StartSpinnerPhase();
	}
}

// --- 3. SpinnerTurn 단계 ---
void AFruitGameMode::StartSpinnerPhase()
{
	if (!MyGameState) return;
	MyGameState->CurrentGamePhase = EGamePhase::GP_SpinnerTurn;
	SpinnerResultIndex = FMath::RandRange(0, 1);
	for (APlayerState* PS : MyGameState->PlayerArray)
	{
		AFruitPlayerController* PC = Cast<AFruitPlayerController>(PS->GetPlayerController());
		if (PC)
		{
			PC->Client_PlaySpinnerAnimation(SpinnerResultIndex);
		}
	}
}

void AFruitGameMode::PlayerRequestsStartTurn(AController* PlayerController)
{
	if (!MyGameState || MyGameState->CurrentGamePhase != EGamePhase::GP_SpinnerTurn || SpinnerResultIndex == -1) return;
	if (MyGameState->CurrentGamePhase == EGamePhase::GP_PlayerTurn) return;

	MyGameState->CurrentActivePlayer = MyGameState->PlayerArray[SpinnerResultIndex];
	MyGameState->CurrentGamePhase = EGamePhase::GP_PlayerTurn;
	StartTurn();
}


// --- 4. PlayerTurn 단계 ---
void AFruitGameMode::StartTurn()
{
	if (!MyGameState || !MyGameState->CurrentActivePlayer) return;
	MyGameState->ServerTimeAtTurnStart = GetWorld()->GetTimeSeconds();
	GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &AFruitGameMode::OnTurnTimerExpired, TurnDuration, false);
	AFruitPlayerController* ActivePC = Cast<AFruitPlayerController>(MyGameState->CurrentActivePlayer->GetPlayerController());
	if (ActivePC)
	{
		ActivePC->Client_StartTurn();
	}
}

void AFruitGameMode::OnTurnTimerExpired()
{
	EndTurn(true);
}

bool AFruitGameMode::IsPlayerTurn(AController* PlayerController) const
{
	if (!MyGameState || !PlayerController || !PlayerController->PlayerState) return false;
	return (MyGameState->CurrentActivePlayer == PlayerController->PlayerState);
}

void AFruitGameMode::ProcessGuessFromWorldObjects(AController* PlayerController)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractableFruitObject::StaticClass(), FoundActors);
	if (FoundActors.Num() != 5)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProcessGuessFromWorldObjects: 5개의 추측(Guessing) 오브젝트를 찾을 수 없습니다."));
		return;
	}
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
		else
		{
			bAllValid = false;
		}
	}
	if (bAllValid)
	{
		ProcessPlayerGuess(PlayerController, GuessedFruits);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ProcessGuessFromWorldObjects: 추측(Guessing) 오브젝트의 GuessIndex가 잘못 설정되었습니다. (0-4)"));
	}
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
	if (GuesserPC)
	{
		GuesserPC->Client_ReceiveGuessResult(GuessedFruits, MatchCount);
	}
	if (OpponentPC)
	{
		OpponentPC->Client_OpponentGuessed(GuessedFruits, MatchCount);
	}
	if (MatchCount == 5)
	{
		EndGame(PlayerController->PlayerState);
	}
	else
	{
		EndTurn(false);
	}
}

void AFruitGameMode::EndTurn(bool bTimeOut)
{
	if (!MyGameState) return;
	if (bTimeOut)
	{
		if (MyGameState) MyGameState->ServerTimeAtTurnStart = 0.0f;
	}
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
	else
	{
		EndGame(nullptr);
	}
}

void AFruitGameMode::EndGame(APlayerState* Winner)
{
	if (!MyGameState) return;
	MyGameState->CurrentGamePhase = EGamePhase::GP_GameOver;
	MyGameState->Winner = Winner;
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	if (MyGameState) MyGameState->ServerTimeAtTurnStart = 0.0f;
	for (APlayerState* PS : MyGameState->PlayerArray)
	{
		AFruitPlayerController* PC = Cast<AFruitPlayerController>(PS->GetPlayerController());
		if (PC)
		{
			PC->Client_GameOver(PS == Winner);
		}
	}
}

// --- 5. 펀치 기능 ---

void AFruitGameMode::ProcessPunchAnimation(ACharacter* PunchingCharacter, UAnimMontage* MontageToPlay)
{
	if (!MyGameState || !PunchingCharacter || !MontageToPlay) return;

	for (APlayerState* PS : MyGameState->PlayerArray)
	{
		AFruitPlayerController* PC = Cast<AFruitPlayerController>(PS->GetPlayerController());
		if (PC)
		{
			PC->Multicast_PlayPunchMontage(PunchingCharacter, MontageToPlay);
		}
	}
}

/** (수정!) 펀치 '적중' 처리 함수 (래그돌 및 카메라 효과 제어) */
void AFruitGameMode::ProcessPunch(APlayerController* PuncherController, ACharacter* HitCharacter)
{
	if (!HitCharacter || !HitCharacter->GetController()) return;

	AFruitPlayerState* HitPlayerState = HitCharacter->GetController()->GetPlayerState<AFruitPlayerState>();
	AFruitGameState* GS = GetGameState<AFruitGameState>();

	if (!HitPlayerState || !GS || HitPlayerState->bIsKnockedDown)
	{
		return;
	}

	FVector PunchDirection = (HitCharacter->GetActorLocation() - PuncherController->GetPawn()->GetActorLocation()).GetSafeNormal();
	PunchDirection.Z = 0.2f;
	HitCharacter->GetCharacterMovement()->AddImpulse(PunchDirection * PunchPushForce, true);

	HitPlayerState->PunchHitCount++;

	if (HitPlayerState->PunchHitCount >= 10)
	{
		HitPlayerState->bIsKnockedDown = true;
		HitPlayerState->PunchHitCount = 0;

		ANoobGameCharacter* HitChar = Cast<ANoobGameCharacter>(HitCharacter);
		if (HitChar)
		{
			HitChar->SetRagdollState_Server(true);
		}

		if (AFruitPlayerController* HitPC = Cast<AFruitPlayerController>(HitCharacter->GetController()))
		{
			HitPC->Client_SetCameraEffect(true);
		}

		FTimerHandle RecoveryTimerHandle;
		FTimerDelegate RecoveryDelegate;
		RecoveryDelegate.BindUFunction(this, FName("RecoverCharacter"), HitCharacter);
		GetWorldTimerManager().SetTimer(RecoveryTimerHandle, RecoveryDelegate, KnockdownDuration, false);
	}
	else
	{
		ANoobGameCharacter* HitChar = Cast<ANoobGameCharacter>(HitCharacter);
		UAnimMontage* HitMontage = HitChar ? HitChar->HitReactionMontage : nullptr;

		if (GS && HitMontage)
		{
			for (APlayerState* PS : GS->PlayerArray)
			{
				AFruitPlayerController* PC = Cast<AFruitPlayerController>(PS->GetPlayerController());
				if (PC)
				{
					PC->Multicast_PlayHitReaction(HitCharacter, HitMontage);
				}
			}
		}
	}
}

/** (수정!) 캐릭터 회복 함수 (래그돌 해제만 지시) */
void AFruitGameMode::RecoverCharacter(ACharacter* CharacterToRecover)
{
	if (!CharacterToRecover || !CharacterToRecover->GetController()) return;

	AFruitPlayerState* PS = CharacterToRecover->GetController()->GetPlayerState<AFruitPlayerState>();
	if (PS && PS->bIsKnockedDown)
	{
		PS->bIsKnockedDown = false;

		ANoobGameCharacter* RecoverChar = Cast<ANoobGameCharacter>(CharacterToRecover);
		if (RecoverChar)
		{
			RecoverChar->SetRagdollState_Server(false);
		}

		// (삭제!) 카메라 효과 해제 로직을 NoobGameCharacter의 타이머로 이동시켰습니다.
		// if (AFruitPlayerController* RecoverPC = Cast<AFruitPlayerController>(CharacterToRecover->GetController()))
		// {
		// 	RecoverPC->Client_SetCameraEffect(false);
		// }
	}
}