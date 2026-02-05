#include "MyNPC.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

// =============================================================
// 1. 초기화 및 네트워크 설정 (Initialization)
// =============================================================

AMyNPC::AMyNPC()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    SetReplicateMovement(true);
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    // 기본 대화 위젯 에셋 로드
    static ConstructorHelpers::FClassFinder<UUserWidget> TalkWidgetAsset(TEXT("/Game/UI/Lobby/WBP_NpcTalk"));
    if (TalkWidgetAsset.Succeeded()) NpcTalkWidgetClass = TalkWidgetAsset.Class;
}

void AMyNPC::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    // 대화 중 상태와 분노 상태를 클라이언트에 복제
    DOREPLIFETIME(AMyNPC, bIsOccupied);
    DOREPLIFETIME(AMyNPC, bIsAngryState);
}

void AMyNPC::BeginPlay()
{
    Super::BeginPlay();
    // 초기 시선 처리 위치 설정
    LookAtTargetLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;
}

// =============================================================
// 2. 매 프레임 업데이트 (Tick Logic)
// =============================================================

void AMyNPC::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsDialogueMode)
    {
        // 대화 중일 때는 플레이어를 향해 부드럽게 회전
        SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed));
    }
    else
    {
        // 평상시에는 근처 플레이어를 따라 시선(헤드 트래킹) 업데이트
        UpdateSmoothHeadTracking(DeltaTime);
    }
}

// =============================================================
// 3. 상호작용 및 대화 시스템 (Interaction & Dialogue)
// =============================================================

void AMyNPC::Interact_Implementation(APlayerController* InteractedPlayerController)
{
    if (bIsOccupied) return;
    if (HasAuthority()) bIsOccupied = true;

    // 상호작용 시작 시 AI의 자율 행동(비헤이비어 트리 등) 중지
    if (AAIController* AICon = Cast<AAIController>(GetController()))
    {
        AICon->StopMovement();
        if (AICon->GetBrainComponent()) AICon->GetBrainComponent()->StopLogic("Interaction Started");
    }

    // 현재 재생 중인 애니메이션 중지
    UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
    if (AnimInst) AnimInst->Montage_Stop(0.2f);
}

void AMyNPC::StartDialogue(APlayerController* PC)
{
    if (!PC || !PC->IsLocalController()) return;
    CurrentInteractingPC = PC;
    bIsDialogueMode = true;

    // 플레이어를 향한 타겟 회전값 계산
    FVector Dir = CurrentInteractingPC->GetPawn()->GetActorLocation() - GetActorLocation();
    Dir.Z = 0.0f;
    TargetRotation = FRotationMatrix::MakeFromX(Dir).Rotator();

    // 대화 위젯 생성 및 마우스 커서 활성화
    if (NpcTalkWidgetClass)
    {
        CurrentTalkWidget = CreateWidget<UUserWidget>(CurrentInteractingPC, NpcTalkWidgetClass);
        if (CurrentTalkWidget)
        {
            CurrentTalkWidget->AddToViewport();
            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(CurrentTalkWidget->TakeWidget());
            CurrentInteractingPC->SetInputMode(InputMode);
            CurrentInteractingPC->bShowMouseCursor = true;
        }
    }
}

void AMyNPC::ExitDialogue()
{
    if (CurrentTalkWidget)
    {
        CurrentTalkWidget->RemoveFromParent();
        CurrentTalkWidget = nullptr;
    }

    if (CurrentInteractingPC)
    {
        FInputModeGameOnly InputMode;
        CurrentInteractingPC->SetInputMode(InputMode);
        CurrentInteractingPC->bShowMouseCursor = false;
        CurrentInteractingPC = nullptr;
    }

    bIsOccupied = false;
    bIsDialogueMode = false;

    // 대화 종료 시 AI 로직 재개
    if (AAIController* AICon = Cast<AAIController>(GetController()))
    {
        if (AICon->GetBrainComponent()) AICon->GetBrainComponent()->RestartLogic();
    }
}

FString AMyNPC::GetNextDialogue()
{
    // 피격 직후라면 화난 대사 출력, 아니면 일반 대사 중 랜덤 출력
    if (bIsAngryState)
    {
        bIsAngryState = false;
        if (HitDialogues.Num() > 0) return HitDialogues[FMath::RandRange(0, HitDialogues.Num() - 1)];
    }
    return (NormalDialogues.Num() > 0) ? NormalDialogues[FMath::RandRange(0, NormalDialogues.Num() - 1)] : TEXT("반가워구리!");
}

// =============================================================
// 4. 전투 및 피격 효과 (Combat & Networking)
// =============================================================

float AMyNPC::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (bIsOccupied) return 0.0f; // 대화 중에는 무적

    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    bIsAngryState = true;

    if (DamageCauser && ActualDamage > 0.0f)
    {
        // 피격 방향 계산 (내적 활용)
        FVector Forward = GetActorForwardVector();
        FVector Right = GetActorRightVector();
        FVector DamageDir = (DamageCauser->GetActorLocation() - GetActorLocation()).GetSafeNormal();

        float ForwardDot = FVector::DotProduct(Forward, DamageDir);
        float RightDot = FVector::DotProduct(Right, DamageDir);

        UAnimMontage* SelectedMontage = nullptr;
        if (ForwardDot > 0.5f) SelectedMontage = HitMontage_Front;
        else if (ForwardDot < -0.5f) SelectedMontage = HitMontage_Back;
        else if (RightDot > 0.0f) SelectedMontage = HitMontage_Right;
        else SelectedMontage = HitMontage_Left;

        // 모든 클라이언트에게 연출 명령
        Multicast_PlayHitEffects(SelectedMontage);
    }

    return ActualDamage;
}

void AMyNPC::Multicast_PlayHitEffects_Implementation(UAnimMontage* TargetMontage)
{
    // 시각적 피드백: 모든 플레이어 화면에서 몽타주 재생
    if (TargetMontage)
    {
        PlayAnimMontage(TargetMontage);
    }

    // 청각적 피드백: 감쇄 설정을 포함한 사운드 재생
    if (HitSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this, HitSound, GetActorLocation(), 1.0f, 1.0f, 0.0f, HitAttenuation
        );
    }
}

// =============================================================
// 5. AI 보조 및 시선 처리 (AI Helpers)
// =============================================================

void AMyNPC::UpdateSmoothHeadTracking(float DeltaTime)
{
    APawn* ClosestPlayer = GetClosestPlayer();
    FVector GoalLocation = GetActorLocation() + GetActorForwardVector() * 200.0f;

    if (ClosestPlayer)
    {
        float Dist = FVector::Dist(GetActorLocation(), ClosestPlayer->GetActorLocation());
        FVector Dir = (ClosestPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(GetActorForwardVector(), Dir)));

        // 일정 거리와 각도 안에 플레이어가 있을 때만 시선 이동
        if (Dist < 400.0f && Angle < 60.0f) GoalLocation = ClosestPlayer->GetPawnViewLocation();
    }

    LookAtTargetLocation = FMath::VInterpTo(LookAtTargetLocation, GoalLocation, DeltaTime, 3.0f);
}

APawn* AMyNPC::GetClosestPlayer()
{
    TArray<AActor*> FoundPawns;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawn::StaticClass(), FoundPawns);

    APawn* Closest = nullptr;
    float MinDist = MAX_FLT;

    for (AActor* Actor : FoundPawns)
    {
        APawn* P = Cast<APawn>(Actor);
        if (!P || P == this) continue;
        float D = FVector::Dist(GetActorLocation(), P->GetActorLocation());
        if (D < MinDist) { MinDist = D; Closest = P; }
    }
    return Closest;
}