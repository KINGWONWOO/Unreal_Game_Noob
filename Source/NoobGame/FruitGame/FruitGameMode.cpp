#include "FruitGame/FruitGameMode.h"
#include "FruitGame/FruitGameState.h"
#include "FruitGame/FruitPlayerState.h"
#include "FruitGame/FruitPlayerController.h"
#include "GameFramework/Controller.h"
#include "FruitGame/InteractableFruitObject.h"
#include "FruitGame/SubmitGuessButton.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "NoobGame/NoobGameCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Engine/TargetPoint.h"
#include "Camera/CameraActor.h"

AFruitGameMode::AFruitGameMode()
{
	GameStateClass = AFruitGameState::StaticClass();
	PlayerStateClass = AFruitPlayerState::StaticClass();
	PlayerControllerClass = AFruitPlayerController::StaticClass();
	PrimaryActorTick.bCanEverTick = false;
	MyGameState = nullptr;
	NumPlayersReady_Setup = 0;
	SpinnerResultIndex = -1;
}

// ... [PostLogin, PlayerIsReady, Setup, Spinner 관련 함수들은 기존과 동일] ...

void AFruitGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

#if WITH_EDITOR 
	if (APlayerState* PS = NewPlayer->GetPlayerState<APlayerState>())
	{
		// 에디터 테스트일 때만 Host/Client 이름 붙이기 (Cat/Dog)
		FString DebugName = (GetNumPlayers() == 1) ? TEXT("Cat") : TEXT("Dog");
		PS->SetPlayerName(DebugName);
	}
#endif

	if (!MyGameState) MyGameState = GetGameState<AFruitGameState>();

	if (NewPlayer && MyGameState)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Server] PostLogin: Player %s joined."), *NewPlayer->GetName());
	}

	if (MyGameState && GetNumPlayers() == 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Server] PostLogin: 2 players detected. GP_Instructions."));
		MyGameState->CurrentGamePhase = EFruitGamePhase::GP_Instructions;
	}
}

// ... [OnTurnTimerExpired, StartTurn, PlayerInteracted, Instructions, Setup 로직 생략(동일)] ...

void AFruitGameMode::OnTurnTimerExpired() { EndTurn(true); }

void AFruitGameMode::StartTurn()
{
	if (!MyGameState || !MyGameState->CurrentActivePlayer) return;
	MyGameState->ServerTimeAtTurnStart = GetWorld()->GetTimeSeconds();

	GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &AFruitGameMode::OnTurnTimerExpired, TurnDuration, false);

	AFruitPlayerController* ActivePC = Cast<AFruitPlayerController>(MyGameState->CurrentActivePlayer->GetPlayerController());
	if (ActivePC) ActivePC->Client_StartTurn();
}

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

// ... [PlayerIsReady ~ StartSpinnerPhase 로직 동일] ...
void AFruitGameMode::PlayerIsReady(AController* PlayerController)
{
	if (!MyGameState || MyGameState->CurrentGamePhase != EFruitGamePhase::GP_Instructions) return;
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
		if (FruitPS && FruitPS->bIsReady_Instructions) ReadyPlayers++;
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

bool AFruitGameMode::IsPlayerTurn(AController* PlayerController) const
{
	if (!MyGameState || !PlayerController || !PlayerController->PlayerState) return false;
	return (MyGameState->CurrentActivePlayer == PlayerController->PlayerState);
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


// --- [수정] ProcessPlayerGuess: 5문제 정답 시 StartWinnerAnnouncement 호출 ---
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
		// [New] 정답! -> 승자 발표 시작
		StartWinnerAnnouncement(PlayerController->PlayerState);
	}
	else
	{
		// 오답: 딜레이 후 턴 종료
		GetWorldTimerManager().SetTimer(
			GuessResultTimerHandle,
			this,
			&AFruitGameMode::OnGuessResultDelayExpired,
			GuessResultDisplayTime,
			false
		);
	}
}

// --- [New] 승자 발표 시작 (OXQuiz와 동일한 로직) ---
void AFruitGameMode::StartWinnerAnnouncement(APlayerState* Winner)
{
	if (!MyGameState) return;

	// 1. 승자 이름 결정 (Cat / Dog / Draw)
	FString WinnerName = TEXT("Draw");
	if (Winner)
	{
		WinnerName = Winner->GetPlayerName(); // 기본값

		// Pawn의 태그 확인 (Cat, Dog)
		if (AController* WinnerPC = Winner->GetPlayerController())
		{
			if (APawn* WinnerPawn = WinnerPC->GetPawn())
			{
				if (WinnerPawn->ActorHasTag(FName("Cat"))) WinnerName = TEXT("Cat");
				else if (WinnerPawn->ActorHasTag(FName("Dog"))) WinnerName = TEXT("Dog");
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[Server GM] StartWinnerAnnouncement. Winner: %s. Waiting %.1f sec..."),
		*WinnerName, WinnerAnnouncementDuration);

	// 2. 진행 중인 턴 타이머, 결과 딜레이 타이머 모두 정지
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	GetWorldTimerManager().ClearTimer(GuessResultTimerHandle);
	GetWorldTimerManager().ClearTimer(EndGameDelayTimerHandle); // 혹시 모를 중복 방지

	// 3. 클라이언트들에게 UI(승자 이름) 표시 지시
	MyGameState->Multicast_AnnounceWinner(WinnerName);

	// 4. 3초 후 실제 EndGame 호출
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("EndGame"), Winner);
	GetWorldTimerManager().SetTimer(EndGameDelayTimerHandle, TimerDel, WinnerAnnouncementDuration, false);
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
	GetWorldTimerManager().ClearTimer(EndGameDelayTimerHandle);

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

void AFruitGameMode::ProcessPunch(APlayerController* PuncherController, ACharacter* HitCharacter)
{
	// (기존 로직 유지)
	if (!HitCharacter || !HitCharacter->GetController() || !PuncherController || !PuncherController->GetPawn()) return;

	AFruitPlayerState* HitPlayerState = HitCharacter->GetController()->GetPlayerState<AFruitPlayerState>();
	if (!MyGameState || !HitPlayerState || HitPlayerState->bIsKnockedDown || MyGameState->CurrentGamePhase == EFruitGamePhase::GP_GameOver) return;

	const FVector PunchDirection = (HitCharacter->GetActorLocation() - PuncherController->GetPawn()->GetActorLocation()).GetSafeNormal();
	HitCharacter->GetCharacterMovement()->AddImpulse(PunchDirection * PunchPushForce, true);
	HitPlayerState->PunchHitCount++;

	if (HitPlayerState->PunchHitCount >= 10)
	{
		HitPlayerState->bIsKnockedDown = true;
		HitPlayerState->PunchHitCount = 0;
		ANoobGameCharacter* HitChar = Cast<ANoobGameCharacter>(HitCharacter);
		if (HitChar) HitChar->SetRagdollState_Server(true);
		if (AFruitPlayerController* HitPC = Cast<AFruitPlayerController>(HitCharacter->GetController()))
		{
			HitPC->Client_SetCameraEffect(true);
		}
		FTimerHandle KnockdownTimer;
		FTimerDelegate TimerDel = FTimerDelegate::CreateUObject(this, &AFruitGameMode::RecoverCharacter, HitCharacter);
		GetWorldTimerManager().SetTimer(KnockdownTimer, TimerDel, KnockdownDuration, false);
		KnockdownTimers.Add(HitCharacter, KnockdownTimer);
	}
	else
	{
		ANoobGameCharacter* HitChar = Cast<ANoobGameCharacter>(HitCharacter);
		if (!HitChar) return;
		const FVector HitVector = (HitCharacter->GetActorLocation() - PuncherController->GetPawn()->GetActorLocation()).GetSafeNormal();
		const FVector ActorForward = HitChar->GetActorForwardVector();
		const FVector ActorRight = HitChar->GetActorRightVector();
		float ForwardDot = FVector::DotProduct(HitVector, ActorForward);
		float RightDot = FVector::DotProduct(HitVector, ActorRight);
		UAnimMontage* SelectedMontage = nullptr;
		if (FMath::Abs(ForwardDot) > FMath::Abs(RightDot)) SelectedMontage = (ForwardDot > 0) ? HitChar->HitReaction_Back : HitChar->HitReaction_Front;
		else SelectedMontage = (RightDot > 0) ? HitChar->HitReaction_Right : HitChar->HitReaction_Left;
		if (!SelectedMontage) SelectedMontage = HitChar->HitReaction_Front;
		if (MyGameState && SelectedMontage)
		{
			for (APlayerState* PS : MyGameState->PlayerArray)
			{
				AFruitPlayerController* PC = Cast<AFruitPlayerController>(PS->GetPlayerController());
				if (PC) PC->Multicast_PlayHitReaction(HitCharacter, SelectedMontage);
			}
		}
	}
}

void AFruitGameMode::EndGame(APlayerState* Winner)
{
	// [수정] GP_GameOver 상태면 중복 실행 방지
	if (!MyGameState || MyGameState->CurrentGamePhase == EFruitGamePhase::GP_GameOver) return;

	UE_LOG(LogTemp, Warning, TEXT("[Server GM] EndGame Started. Winner: %s"), Winner ? *Winner->GetPlayerName() : TEXT("None"));

	MyGameState->CurrentGamePhase = EFruitGamePhase::GP_GameOver;
	MyGameState->Winner = Winner;

	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	GetWorldTimerManager().ClearTimer(GuessResultTimerHandle);
	GetWorldTimerManager().ClearTimer(EndGameDelayTimerHandle);
	MyGameState->ServerTimeAtTurnStart = 0.0f;

	for (auto& TimerPair : KnockdownTimers)
	{
		GetWorldTimerManager().ClearTimer(TimerPair.Value);
	}
	KnockdownTimers.Empty();

	APlayerState* Loser = nullptr;
	for (APlayerState* PS : MyGameState->PlayerArray)
	{
		if (PS && PS != Winner)
		{
			Loser = PS;
			break;
		}
	}

	TArray<AActor*> WinnerSpawns, DefeatSpawns, CameraActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATargetPoint::StaticClass(), FName("Result_Spawn_Winner"), WinnerSpawns);
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATargetPoint::StaticClass(), FName("Result_Spawn_Defeat"), DefeatSpawns);
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACameraActor::StaticClass(), FName("EndingCamera"), CameraActors);

	AActor* WinnerSpawnPoint = (WinnerSpawns.Num() > 0) ? WinnerSpawns[0] : nullptr;
	AActor* DefeatSpawnPoint = (DefeatSpawns.Num() > 0) ? DefeatSpawns[0] : nullptr;
	ACameraActor* EndingCamera = (CameraActors.Num() > 0) ? Cast<ACameraActor>(CameraActors[0]) : nullptr;

	AFruitPlayerController* WinnerPC = Winner ? Cast<AFruitPlayerController>(Winner->GetPlayerController()) : nullptr;
	ANoobGameCharacter* WinnerPawn = WinnerPC ? Cast<ANoobGameCharacter>(WinnerPC->GetPawn()) : nullptr;
	if (WinnerPawn)
	{
		if (WinnerPawn->ActorHasTag(FName("Cat"))) MyGameState->WinningCharacterType = ECharacterType::ECT_Cat;
		else if (WinnerPawn->ActorHasTag(FName("Dog"))) MyGameState->WinningCharacterType = ECharacterType::ECT_Dog;
	}

	AFruitPlayerController* LoserPC = Loser ? Cast<AFruitPlayerController>(Loser->GetPlayerController()) : nullptr;

	if (WinnerPC && WinnerSpawnPoint && EndingCamera)
	{
		WinnerPC->Server_SetupEnding(true, WinnerSpawnPoint->GetActorLocation(), WinnerSpawnPoint->GetActorRotation(), MyGameState->WinningCharacterType, EndingCamera);
	}

	if (LoserPC && DefeatSpawnPoint && EndingCamera)
	{
		LoserPC->Server_SetupEnding(false, DefeatSpawnPoint->GetActorLocation(), DefeatSpawnPoint->GetActorRotation(), MyGameState->WinningCharacterType, EndingCamera);
	}

	TArray<AActor*> Obstacles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractableFruitObject::StaticClass(), Obstacles);
	// 과일 게임에서는 오브젝트를 파괴할 필요가 없다면 아래 코드는 제거해도 됩니다. (예: 배경 요소라면 유지)
	// for (AActor* A : Obstacles) A->Destroy();
}

void AFruitGameMode::ProcessPunchAnimation(ACharacter* PunchingCharacter, UAnimMontage* MontageToPlay)
{
	if (!MyGameState || !PunchingCharacter || !MontageToPlay) return;
	for (APlayerState* PS : MyGameState->PlayerArray)
	{
		AFruitPlayerController* PC = Cast<AFruitPlayerController>(PS->GetPlayerController());
		if (PC) PC->Multicast_PlayPunchMontage(PunchingCharacter, MontageToPlay);
	}
}

void AFruitGameMode::RecoverCharacter(ACharacter* CharacterToRecover)
{
	if (!CharacterToRecover || !CharacterToRecover->GetController()) return;
	KnockdownTimers.Remove(CharacterToRecover);
	if (MyGameState && MyGameState->CurrentGamePhase == EFruitGamePhase::GP_GameOver) return;

	AFruitPlayerState* PS = CharacterToRecover->GetController()->GetPlayerState<AFruitPlayerState>();
	if (PS && PS->bIsKnockedDown)
	{ 
		PS->bIsKnockedDown = false;
		ANoobGameCharacter* RecoverChar = Cast<ANoobGameCharacter>(CharacterToRecover);
		if (RecoverChar) RecoverChar->SetRagdollState_Server(false);
		if (AFruitPlayerController* RecoverPC = Cast<AFruitPlayerController>(CharacterToRecover->GetController()))
		{
			RecoverPC->Client_SetCameraEffect(false);
		}
	}
}