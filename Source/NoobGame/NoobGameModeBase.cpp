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

// =============================================================
// 1. 초기화 및 입장 처리 (Initialization)
// =============================================================

ANoobGameModeBase::ANoobGameModeBase()
{
    PrimaryActorTick.bCanEverTick = false;
    GameStateClass = ANoobGameStateBase::StaticClass();
    PlayerStateClass = ANoobPlayerState::StaticClass();
    PlayerControllerClass = ANoobPlayerController::StaticClass();
    bHasAssignedRoomOwner = false;
    bUseSeamlessTravel = true;
}

void ANoobGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (ANoobPlayerState* PS = NewPlayer->GetPlayerState<ANoobPlayerState>())
    {
#if WITH_EDITOR 
        // 에디터 테스트 시 가독성을 위해 이름 강제 부여
        FString DebugName = (GetNumPlayers() == 1) ? TEXT("Cat") : TEXT("Dog");
        PS->SetPlayerName(DebugName);
#endif
        // 최초 접속자에게 방장 권한 부여
        if (!bHasAssignedRoomOwner || GetNumPlayers() == 1)
        {
            PS->bIsRoomOwner = true;
            bHasAssignedRoomOwner = true;
            if (NewPlayer->IsLocalController())
            {
                PS->OnRep_IsRoomOwner();
            }
        }
        else
        {
            PS->bIsRoomOwner = false;
        }
    }
}

// =============================================================
// 2. 게임 흐름 제어 (Game Flow)
// =============================================================

void ANoobGameModeBase::Logout(AController* Exiting)
{
    Super::Logout(Exiting);
    // 게임 도중 나갔을 경우 예외 처리
    if (IsGameInProgress())
    {
        HandlePlayerDisconnect(Exiting);
    }
}

void ANoobGameModeBase::HandlePlayerDisconnect(AController* ExitingPlayer)
{
    if (GetWorldTimerManager().IsTimerActive(EndGameDelayTimerHandle)) return;

    // 나간 사람이 아닌 다른 플레이어를 승자로 지정
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
        // 태그 기반으로 캐릭터 이름 확정
        if (Winner && Winner->GetPawn())
        {
            if (Winner->GetPawn()->ActorHasTag(FName("Cat"))) WinnerName = TEXT("Cat");
            else if (Winner->GetPawn()->ActorHasTag(FName("Dog"))) WinnerName = TEXT("Dog");
        }
        GS->Multicast_AnnounceWinner(WinnerName);
    }

    // 일정 시간 뒤에 EndGame 호출
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

    // 승자와 패자 식별
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

    // 월드에서 엔딩 시퀀스용 액터(스폰 지점, 카메라) 찾기
    TArray<AActor*> WinSpawns, DefeatSpawns, Cameras;
    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATargetPoint::StaticClass(), WinnerSpawnTag, WinSpawns);
    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATargetPoint::StaticClass(), LoserSpawnTag, DefeatSpawns);
    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACameraActor::StaticClass(), EndingCameraTag, Cameras);

    AActor* WinPt = WinSpawns.IsValidIndex(0) ? WinSpawns[0] : nullptr;
    AActor* LosePt = DefeatSpawns.IsValidIndex(0) ? DefeatSpawns[0] : nullptr;
    ACameraActor* Cam = Cameras.IsValidIndex(0) ? Cast<ACameraActor>(Cameras[0]) : nullptr;

    // 플레이어들을 엔딩 카메라 뷰로 전환 및 위치 이동
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

// =============================================================
// 3. 전투 및 충돌 로직 (Combat)
// =============================================================

void ANoobGameModeBase::ProcessPunch(APlayerController* PuncherController, ACharacter* HitCharacter)
{
    if (!IsGameInProgress() || !HitCharacter || !PuncherController) return;

    ANoobPlayerState* HitPS = HitCharacter->GetController() ? HitCharacter->GetController()->GetPlayerState<ANoobPlayerState>() : nullptr;
    if (!HitPS || HitPS->bIsKnockedDown) return;

    // 충격 전달 (Impulse)
    const FVector PunchDir = (HitCharacter->GetActorLocation() - PuncherController->GetPawn()->GetActorLocation()).GetSafeNormal();
    HitCharacter->GetCharacterMovement()->AddImpulse(PunchDir * PunchPushForce, true);

    // 피격 횟수 증가 및 넉다운 판정
    HitPS->PunchHitCount++;
    if (HitPS->PunchHitCount >= 10)
    {
        HitPS->bIsKnockedDown = true;
        HitPS->PunchHitCount = 0;

        if (ANoobGameCharacter* HitChar = Cast<ANoobGameCharacter>(HitCharacter))
        {
            HitChar->SetDownState_Server(true);
        }

        if (ANoobPlayerController* HitPC = Cast<ANoobPlayerController>(HitCharacter->GetController()))
        {
            HitPC->Client_SetCameraEffect(true);
        }

        // 회복 대기 타이머 설정
        FTimerHandle KDTimer;
        FTimerDelegate Del = FTimerDelegate::CreateUObject(this, &ANoobGameModeBase::RecoverCharacter, HitCharacter);
        GetWorldTimerManager().SetTimer(KDTimer, Del, KnockdownDuration, false);
        KnockdownTimers.Add(HitCharacter, KDTimer);
    }
    else
    {
        // 일반 피격 시 카메라 쉐이크 및 방향별 애니메이션 판정
        if (ANoobPlayerController* NoobPC = Cast<ANoobPlayerController>(HitCharacter->GetController()))
        {
            NoobPC->Client_PlayHitCameraShake();
        }

        ANoobGameCharacter* HitChar = Cast<ANoobGameCharacter>(HitCharacter);
        if (!HitChar) return;

        // 내적(Dot Product)을 활용한 피격 방향 계산
        const FVector HitVector = (HitCharacter->GetActorLocation() - PuncherController->GetPawn()->GetActorLocation()).GetSafeNormal();
        float ForwardDot = FVector::DotProduct(HitVector, HitChar->GetActorForwardVector());
        float RightDot = FVector::DotProduct(HitVector, HitChar->GetActorRightVector());

        UAnimMontage* Selected = nullptr;
        if (FMath::Abs(ForwardDot) > FMath::Abs(RightDot))
            Selected = (ForwardDot > 0) ? HitChar->HitReaction_Back : HitChar->HitReaction_Front;
        else
            Selected = (RightDot > 0) ? HitChar->HitReaction_Right : HitChar->HitReaction_Left;

        UAnimMontage* FinalMontage = Selected ? Selected : HitChar->HitReaction_Front;
        ProcessPunchAnimation(HitChar, FinalMontage);
    }
}

void ANoobGameModeBase::ProcessPunchAnimation(ACharacter* PunchingCharacter, UAnimMontage* MontageToPlay)
{
    if (!PunchingCharacter || !MontageToPlay) return;

    // 모든 플레이어에게 애니메이션 동기화 (멀티캐스트)
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (ANoobPlayerController* NPC = Cast<ANoobPlayerController>(It->Get()))
        {
            NPC->Multicast_PlayPunchMontage(PunchingCharacter, MontageToPlay);
        }
    }
}

// 5. 유틸리티 및 기타 로직 (Utilities)

void ANoobGameModeBase::RecoverCharacter(ACharacter* CharacterToRecover)
{
    KnockdownTimers.Remove(CharacterToRecover);
    if (!IsGameInProgress() || !CharacterToRecover || !CharacterToRecover->GetController()) return;

    ANoobPlayerState* NoobPS = CharacterToRecover->GetController()->GetPlayerState<ANoobPlayerState>();
    if (NoobPS && NoobPS->bIsKnockedDown)
    {
        NoobPS->bIsKnockedDown = false;

        // 애니메이션 기반 상태 해제
        if (ANoobGameCharacter* C = Cast<ANoobGameCharacter>(CharacterToRecover))
        {
            C->SetDownState_Server(false);
        }

        if (ANoobPlayerController* PC = Cast<ANoobPlayerController>(CharacterToRecover->GetController()))
        {
            PC->Client_SetCameraEffect(false);
        }
    }
}

void ANoobGameModeBase::Server_TransitionToSelectedMap(FString MapName)
{
    // 맵 이동 전 로딩 화면 표시 요청
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (ANoobPlayerController* PC = Cast<ANoobPlayerController>(It->Get()))
        {
            PC->Client_ShowLoadingScreen();
        }
    }
}