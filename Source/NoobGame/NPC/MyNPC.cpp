#include "MyNPC.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"


AMyNPC::AMyNPC()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    SetReplicateMovement(true);
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    static ConstructorHelpers::FClassFinder<UUserWidget> TalkWidgetAsset(TEXT("/Game/MenuSystem/Lobby/WBP_NpcTalk"));
    if (TalkWidgetAsset.Succeeded()) NpcTalkWidgetClass = TalkWidgetAsset.Class;

    // 카메라 컴포넌트 생성 코드 삭제됨
}

void AMyNPC::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AMyNPC, bIsOccupied);
    DOREPLIFETIME(AMyNPC, bIsAngryState);
}

void AMyNPC::BeginPlay()
{
    Super::BeginPlay();
    LookAtTargetLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;
}

void AMyNPC::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bIsDialogueMode)
    {
        SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed));
    }
    else
    {
        UpdateSmoothHeadTracking(DeltaTime);
    }
}

void AMyNPC::Interact_Implementation(APlayerController* InteractedPlayerController)
{
    if (bIsOccupied) return;
    if (HasAuthority()) bIsOccupied = true;

    if (AAIController* AICon = Cast<AAIController>(GetController()))
    {
        AICon->StopMovement();
        if (AICon->GetBrainComponent()) AICon->GetBrainComponent()->StopLogic("Interaction Started");
    }

    UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
    if (AnimInst) AnimInst->Montage_Stop(0.2f);
}

void AMyNPC::StartDialogue(APlayerController* PC)
{
    if (!PC || !PC->IsLocalController()) return;
    CurrentInteractingPC = PC;
    bIsDialogueMode = true;

    // 카메라 뷰 타겟 변경 코드(SetViewTargetWithBlend) 삭제됨

    FVector Dir = CurrentInteractingPC->GetPawn()->GetActorLocation() - GetActorLocation();
    Dir.Z = 0.0f;
    TargetRotation = FRotationMatrix::MakeFromX(Dir).Rotator();

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

        // 카메라 뷰 타겟 복구 코드 삭제됨

        CurrentInteractingPC = nullptr;
    }

    bIsOccupied = false;
    bIsDialogueMode = false;

    if (AAIController* AICon = Cast<AAIController>(GetController()))
    {
        if (AICon->GetBrainComponent()) AICon->GetBrainComponent()->RestartLogic();
    }
}

float AMyNPC::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (bIsOccupied) return 0.0f;

    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    bIsAngryState = true;

    if (DamageCauser && ActualDamage > 0.0f)
    {
        // 1. 방향 계산 로직 (기존과 동일)
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

        // 2. 서버에서 직접 재생하지 않고, 멀티캐스트로 넘김
        Multicast_PlayHitEffects(SelectedMontage);
    }

    return ActualDamage;
}

FString AMyNPC::GetNextDialogue()
{
    if (bIsAngryState)
    {
        bIsAngryState = false;
        if (HitDialogues.Num() > 0) return HitDialogues[FMath::RandRange(0, HitDialogues.Num() - 1)];
    }
    return (NormalDialogues.Num() > 0) ? NormalDialogues[FMath::RandRange(0, NormalDialogues.Num() - 1)] : TEXT("반가워구리!");
}

void AMyNPC::UpdateSmoothHeadTracking(float DeltaTime)
{
    APawn* ClosestPlayer = GetClosestPlayer();
    FVector GoalLocation = GetActorLocation() + GetActorForwardVector() * 200.0f;

    if (ClosestPlayer)
    {
        float Dist = FVector::Dist(GetActorLocation(), ClosestPlayer->GetActorLocation());
        FVector Dir = (ClosestPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(GetActorForwardVector(), Dir)));

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

void AMyNPC::Multicast_PlayHitEffects_Implementation(UAnimMontage* TargetMontage)
{
    // [1] 애니메이션 재생 (모든 클라이언트)
    if (TargetMontage)
    {
        PlayAnimMontage(TargetMontage);
        UE_LOG(LogTemp, Log, TEXT("Playing Montage on Client/Server"));
    }

    // [2] 사운드 재생 (모든 클라이언트)
    if (HitSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this, HitSound, GetActorLocation(), 1.0f, 1.0f, 0.0f, HitAttenuation
        );
    }
}