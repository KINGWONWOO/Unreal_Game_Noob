#include "OXQuizGameMode.h"
#include "OXQuizGameState.h"
#include "OXQuizPlayerController.h"
#include "OXQuizPlayerState.h"
#include "QuizObstacleBase.h"
#include "NoobGameCharacter.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Engine/TargetPoint.h"
#include "TimerManager.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"

AOXQuizGameMode::AOXQuizGameMode()
{
	GameStateClass = AOXQuizGameState::StaticClass();
	PlayerStateClass = AOXQuizPlayerState::StaticClass();
	PlayerControllerClass = AOXQuizPlayerController::StaticClass();
	DefaultPawnClass = ANoobGameCharacter::StaticClass();
	MyGameState = nullptr;

	SpeedLevels = { 400.f, 500.f, 600.f, 700.f, 800.f };
}

// ... [PostLogin, Logout 등은 기존과 동일, HandlePlayerDeath부터 수정] ...

void AOXQuizGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// ─────────────────────────────────────────────────────────────────
	// [추천] 에디터(PIE)에서만 임시 이름을 붙이고, 
	// 실제 배포(Shipping/Steam)시에는 건드리지 않음
	// ─────────────────────────────────────────────────────────────────
#if WITH_EDITOR 
	if (APlayerState* PS = NewPlayer->GetPlayerState<APlayerState>())
	{
		// 에디터 테스트일 때만 Host/Client 이름 붙이기
		FString DebugName = (GetNumPlayers() == 1) ? TEXT("Cat") : TEXT("Dog");
		PS->SetPlayerName(DebugName);
	}
#endif
	// ─────────────────────────────────────────────────────────────────

	if (!MyGameState)
	{
		MyGameState = GetGameState<AOXQuizGameState>();
	}
	if (!MyGameState)
	{
		MyGameState = GetGameState<AOXQuizGameState>();
	}

	if (NewPlayer && MyGameState)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Server] PostLogin: Player %s joined."), *NewPlayer->GetName());
	}

	if (GetNumPlayers() == 1)
	{
		if (AOXQuizPlayerState* PS = NewPlayer->GetPlayerState<AOXQuizPlayerState>())
		{
			PS->bIsRoomOwner = true; // 너 방장 해라
			UE_LOG(LogTemp, Warning, TEXT("Player %s is assigned as Room Owner."), *NewPlayer->GetName());
		}
	}

	if (MyGameState && GetNumPlayers() == 2)
	{
		MyGameState->CurrentGamePhase = EQuizGamePhase::GP_Ready;
		for (APlayerState* PS : MyGameState->PlayerArray)
		{
			if (AOXQuizPlayerState* QPS = Cast<AOXQuizPlayerState>(PS))
			{
				QPS->bIsReady_Instructions = false;
				QPS->bIsKnockedDown = false;
				QPS->PunchHitCount = 0;
				QPS->bIsNextPunchLeft = true;
			}
		}
	}
}

void AOXQuizGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	if (MyGameState && (MyGameState->CurrentGamePhase == EQuizGamePhase::GP_Playing ||
		MyGameState->CurrentGamePhase == EQuizGamePhase::GP_Ready))
	{
		if (GetNumPlayers() <= 1) HandlePlayerDeath(Exiting);
	}
}

/* ────────────────────── Player Death / Ready ────────────────────── */
void AOXQuizGameMode::HandlePlayerDeath(AController* PlayerController)
{
	// 이미 게임 오버 상태라면 중복 호출 방지
	if (!MyGameState || MyGameState->CurrentGamePhase == EQuizGamePhase::GP_GameOver) return;

	// [중요] 이미 발표 타이머가 돌고 있다면(누군가 먼저 죽어서 처리 중이라면) 리턴
	if (GetWorldTimerManager().IsTimerActive(EndGameDelayTimerHandle)) return;

	APlayerState* LoserState = PlayerController ? PlayerController->GetPlayerState<APlayerState>() : nullptr;
	APlayerState* WinnerState = nullptr;

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (AController* Other = It->Get(); Other && Other != PlayerController)
		{
			WinnerState = Other->GetPlayerState<APlayerState>();
			break;
		}
	}

	// 즉시 EndGame을 호출하는 대신, 3초 발표 시퀀스를 시작합니다.
	StartWinnerAnnouncement(WinnerState);
}

void AOXQuizGameMode::StartWinnerAnnouncement(APlayerState* Winner)
{
	if (!MyGameState) return;

	UE_LOG(LogTemp, Warning, TEXT("[Server GM] StartWinnerAnnouncement. Winner: %s. Waiting %.1f sec..."),
		Winner ? *Winner->GetPlayerName() : TEXT("None"), WinnerAnnouncementDuration);

	// 1. 더 이상 퀴즈 스폰되지 않도록 타이머 정지
	GetWorldTimerManager().ClearTimer(TimerHandle_SpawnQuiz);
	GetWorldTimerManager().ClearTimer(TimerHandle_GamePhase);

	// 2. 클라이언트들에게 UI 띄우라고 지시 (Phase 변경 없이 NetMulticast 사용)
	FString WinnerName = Winner ? Winner->GetPlayerName() : TEXT("Draw");
	MyGameState->Multicast_AnnounceWinner(WinnerName);

	// 3. 3초 후 실제 EndGame이 실행되도록 타이머 설정
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("EndGame"), Winner);

	GetWorldTimerManager().SetTimer(EndGameDelayTimerHandle, TimerDel, WinnerAnnouncementDuration, false);
}

void AOXQuizGameMode::EndGame(APlayerState* Winner)
{
	if (!MyGameState || MyGameState->CurrentGamePhase == EQuizGamePhase::GP_GameOver) return;

	UE_LOG(LogTemp, Warning, TEXT("[Server GM] EndGame Executing. Transitioning to GP_GameOver."));

	MyGameState->CurrentGamePhase = EQuizGamePhase::GP_GameOver;
	MyGameState->Winner = Winner;

	// 기존 타이머 클리어
	GetWorldTimerManager().ClearTimer(EndGameDelayTimerHandle); // 혹시 모르니
	GetWorldTimerManager().ClearTimer(TimerHandle_SpawnQuiz);

	for (auto& Pair : KnockdownTimers)
	{
		GetWorldTimerManager().ClearTimer(Pair.Value);
	}
	KnockdownTimers.Empty();

	// Loser 찾기
	APlayerState* Loser = nullptr;
	for (APlayerState* PS : MyGameState->PlayerArray)
	{
		if (PS && PS != Winner)
		{
			Loser = PS;
			break;
		}
	}

	// 승자 타입 설정 (UI 표시용)
	AOXQuizPlayerController* WinnerPC = Winner ? Cast<AOXQuizPlayerController>(Winner->GetPlayerController()) : nullptr;
	ANoobGameCharacter* WinnerPawn = WinnerPC ? Cast<ANoobGameCharacter>(WinnerPC->GetPawn()) : nullptr;
	if (WinnerPawn)
	{
		if (WinnerPawn->ActorHasTag(FName("Cat"))) MyGameState->WinningCharacterType = ECharacterType::ECT_Cat;
		else if (WinnerPawn->ActorHasTag(FName("Dog"))) MyGameState->WinningCharacterType = ECharacterType::ECT_Dog;
	}

	// 태그로 Actor 찾기
	TArray<AActor*> WinSpawns, LoseSpawns, CamActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATargetPoint::StaticClass(), FName("Result_Spawn_Winner"), WinSpawns);
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATargetPoint::StaticClass(), FName("Result_Spawn_Defeat"), LoseSpawns);
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACameraActor::StaticClass(), FName("EndingCamera"), CamActors);

	AActor* WinPoint = WinSpawns.IsValidIndex(0) ? WinSpawns[0] : nullptr;
	AActor* LosePoint = LoseSpawns.IsValidIndex(0) ? LoseSpawns[0] : nullptr;
	ACameraActor* EndCam = CamActors.IsValidIndex(0) ? Cast<ACameraActor>(CamActors[0]) : nullptr;

	// EndGame 카메라/캐릭터 배치 처리
	AOXQuizPlayerController* LoserPC = Loser ? Cast<AOXQuizPlayerController>(Loser->GetPlayerController()) : nullptr;
	ECharacterType WinType = MyGameState->WinningCharacterType;

	if (WinnerPC && WinPoint && EndCam)
	{
		WinnerPC->Server_SetupEnding(true, WinPoint->GetActorLocation(), WinPoint->GetActorRotation(), WinType, EndCam);
	}

	if (LoserPC && LosePoint && EndCam)
	{
		LoserPC->Server_SetupEnding(false, LosePoint->GetActorLocation(), LosePoint->GetActorRotation(), WinType, EndCam);
	}

	// 장애물 파괴
	TArray<AActor*> Obstacles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AQuizObstacleBase::StaticClass(), Obstacles);
	for (AActor* A : Obstacles) A->Destroy();
}

void AOXQuizGameMode::PlayerIsReady(AController* PlayerController)
{
	if (!MyGameState || MyGameState->CurrentGamePhase != EQuizGamePhase::GP_Ready) return;
	if (AOXQuizPlayerState* PS = PlayerController->GetPlayerState<AOXQuizPlayerState>())
	{
		PS->SetInstructionReady_Server();
		CheckBothPlayersReady_Instruction();
	}
}

void AOXQuizGameMode::CheckBothPlayersReady_Instruction()
{
	if (!MyGameState) return;
	int32 ReadyCnt = 0;
	for (APlayerState* PS : MyGameState->PlayerArray)
		if (AOXQuizPlayerState* QPS = Cast<AOXQuizPlayerState>(PS))
			if (QPS->bIsReady_Instructions) ++ReadyCnt;

	if (ReadyCnt == 2) {
		MyGameState->CurrentGamePhase = EQuizGamePhase::GP_Playing;
		RemainingPlayingCountdown = PlayingStartCountdownDuration;
		if (MyGameState) MyGameState->SetPlayingCountdown(RemainingPlayingCountdown);

		GetWorldTimerManager().SetTimer(
			TimerHandle_GamePhase,
			this,
			&AOXQuizGameMode::UpdatePlayingCountdown,
			1.f,
			true);
	}
}

void AOXQuizGameMode::UpdatePlayingCountdown()
{
	--RemainingPlayingCountdown;
	if (MyGameState) MyGameState->SetPlayingCountdown(RemainingPlayingCountdown);
	if (RemainingPlayingCountdown <= 0)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_GamePhase);
		StartQuizSpawning();
	}
}


void AOXQuizGameMode::SpawnNextQuizObstacle()
{
	if (!MyGameState || MyGameState->CurrentGamePhase != EQuizGamePhase::GP_Playing) return;

	FQuizData Quiz;
	if (GetRandomQuiz(Quiz))
	{
		// (기존 스폰 로직 동일)
		TSubclassOf<AQuizObstacleBase> ObstacleClass = (Quiz.Answers.Num() == 2) ? QuizObstacleClass_2Choice : QuizObstacleClass_3Choice;
		if (!ObstacleClass) return;

		FActorSpawnParameters SP;
		SP.Owner = this;
		SP.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (AQuizObstacleBase* NewObs = GetWorld()->SpawnActor<AQuizObstacleBase>(ObstacleClass, ObstacleSpawnTransform, SP))
		{
			NewObs->InitializeObstacle(Quiz, CurrentMoveSpeed);
			++SpawnedQuizCount;
			if (SpawnedQuizCount % 5 == 0 && SpeedLevels.IsValidIndex(++CurrentSpeedLevelIndex))
			{
				CurrentMoveSpeed = SpeedLevels[CurrentSpeedLevelIndex];
				if (MyGameState) MyGameState->SetCurrentSpeedLevel(CurrentSpeedLevelIndex + 1);
			}
			GetWorldTimerManager().SetTimer(TimerHandle_SpawnQuiz, this, &AOXQuizGameMode::SpawnNextQuizObstacle, TimeBetweenSpawns, false);
		}
	}
	else
	{
		// [Changed] 퀴즈가 다 떨어졌을 때, 다시 난이도에 맞춰서 리필
		UE_LOG(LogTemp, Log, TEXT("Ran out of quizzes. Refilling based on difficulty..."));
		LoadQuizListByDifficulty();

		// 0.1초 뒤 재시도
		GetWorldTimerManager().SetTimer(TimerHandle_SpawnQuiz, this, &AOXQuizGameMode::SpawnNextQuizObstacle, 0.1f, false);
	}
}

bool AOXQuizGameMode::GetRandomQuiz(FQuizData& OutQuiz)
{
	if (RemainingQuizList.IsEmpty()) return false;
	const int32 Idx = FMath::RandRange(0, RemainingQuizList.Num() - 1);
	OutQuiz = RemainingQuizList[Idx];
	RemainingQuizList.RemoveAt(Idx);
	return true;
}

void AOXQuizGameMode::ProcessPunch(APlayerController* PuncherController, ACharacter* HitCharacter)
{
	if (!HitCharacter || !HitCharacter->GetController() || !PuncherController || !PuncherController->GetPawn()) return;
	AOXQuizPlayerState* HitPlayerState = HitCharacter->GetController()->GetPlayerState<AOXQuizPlayerState>();

	if (!MyGameState || !HitPlayerState || HitPlayerState->bIsKnockedDown || MyGameState->CurrentGamePhase != EQuizGamePhase::GP_Playing) return;

	const FVector PunchDirection = (HitCharacter->GetActorLocation() - PuncherController->GetPawn()->GetActorLocation()).GetSafeNormal();
	HitCharacter->GetCharacterMovement()->AddImpulse(PunchDirection * PunchPushForce, true);
	HitPlayerState->PunchHitCount++;

	if (HitPlayerState->PunchHitCount >= 10)
	{
		HitPlayerState->bIsKnockedDown = true;
		HitPlayerState->PunchHitCount = 0;
		ANoobGameCharacter* HitChar = Cast<ANoobGameCharacter>(HitCharacter);
		if (HitChar) HitChar->SetRagdollState_Server(true);
		if (AOXQuizPlayerController* HitPC = Cast<AOXQuizPlayerController>(HitCharacter->GetController()))
		{
			HitPC->Client_SetCameraEffect(true);
		}
		FTimerHandle KnockdownTimer;
		FTimerDelegate TimerDel = FTimerDelegate::CreateUObject(this, &AOXQuizGameMode::RecoverCharacter, HitCharacter);
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
				AOXQuizPlayerController* PC = Cast<AOXQuizPlayerController>(PS->GetPlayerController());
				if (PC) PC->Multicast_PlayHitReaction(HitCharacter, SelectedMontage);
			}
		}
	}
}

void AOXQuizGameMode::RecoverCharacter(ACharacter* CharacterToRecover)
{
	if (!CharacterToRecover || !CharacterToRecover->GetController()) return;
	KnockdownTimers.Remove(CharacterToRecover);
	if (MyGameState && MyGameState->CurrentGamePhase == EQuizGamePhase::GP_GameOver) return;
	AOXQuizPlayerState* PS = CharacterToRecover->GetController()->GetPlayerState<AOXQuizPlayerState>();
	if (PS && PS->bIsKnockedDown)
	{
		PS->bIsKnockedDown = false;
		ANoobGameCharacter* RecoverChar = Cast<ANoobGameCharacter>(CharacterToRecover);
		if (RecoverChar) RecoverChar->SetRagdollState_Server(false);
		if (AOXQuizPlayerController* RecoverPC = Cast<AOXQuizPlayerController>(CharacterToRecover->GetController()))
		{
			RecoverPC->Client_SetCameraEffect(false);
		}
	}
}

void AOXQuizGameMode::SetGameDifficulty(EQuizDifficulty NewDifficulty)
{
	CurrentGameDifficulty = NewDifficulty;

	// [New] GameState를 통해 모든 클라이언트에게 변경 사항 전파
	if (MyGameState)
	{
		MyGameState->SetRepDifficulty(NewDifficulty);
	}

	UE_LOG(LogTemp, Log, TEXT("Game Difficulty Set to: %d"), (int32)CurrentGameDifficulty);
}

void AOXQuizGameMode::StartQuizSpawning()
{
	if (!QuizDataTable)
	{
		MyGameState->CurrentGamePhase = EQuizGamePhase::GP_GameOver;
		return;
	}

	// [Changed] 무조건 다 불러오는 대신, 난이도 필터링 함수 호출
	LoadQuizListByDifficulty();

	SpawnedQuizCount = 0;
	CurrentSpeedLevelIndex = 0;
	CurrentMoveSpeed = SpeedLevels.IsValidIndex(0) ? SpeedLevels[0] : 400.f;

	if (MyGameState) MyGameState->SetCurrentSpeedLevel(CurrentSpeedLevelIndex + 1);

	SpawnNextQuizObstacle();
}

void AOXQuizGameMode::LoadQuizListByDifficulty()
{
	if (!QuizDataTable) return;

	RemainingQuizList.Empty();

	TArray<FQuizData*> AllRows;
	static const FString ContextString(TEXT("LoadQuizListByDifficulty"));
	QuizDataTable->GetAllRows(ContextString, AllRows);

	for (FQuizData* Row : AllRows)
	{
		// [핵심] 데이터의 난이도와 현재 설정된 게임 난이도가 같을 때만 추가
		if (Row && Row->Difficulty == CurrentGameDifficulty)
		{
			RemainingQuizList.Add(*Row);
		}
	}

	// 만약 해당 난이도의 문제가 하나도 없다면? (예외 처리)
	if (RemainingQuizList.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No quizzes found for Difficulty %d! Loading ALL quizzes as fallback."), (int32)CurrentGameDifficulty);
		for (FQuizData* Row : AllRows)
		{
			if (Row) RemainingQuizList.Add(*Row);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Loaded %d quizzes for Difficulty %d"), RemainingQuizList.Num(), (int32)CurrentGameDifficulty);
}

void AOXQuizGameMode::ProcessPunchAnimation(ACharacter* PunchingCharacter, UAnimMontage* MontageToPlay)
{
	if (!MyGameState || !PunchingCharacter || !MontageToPlay) return;
	for (APlayerState* PS : MyGameState->PlayerArray)
	{
		AOXQuizPlayerController* PC = Cast<AOXQuizPlayerController>(PS->GetPlayerController());
		if (PC) PC->Multicast_PlayPunchMontage(PunchingCharacter, MontageToPlay);
	}
}