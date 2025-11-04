// NoobGameCharacter.cpp
#include "NoobGameCharacter.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "FruitGame/FruitPlayerController.h" 

ANoobGameCharacter::ANoobGameCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
	bIsRagdolling = false;

	// --- (수정!) 'Owner No See' 및 'Only Owner See' 강제 해제 ---
	if (USkeletalMeshComponent* MyMesh = GetMesh())
	{
		MyMesh->SetOwnerNoSee(false);
		MyMesh->SetOnlyOwnerSee(false);
	}
}

void ANoobGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	// (신규!) 블루프린트 설정을 무시하고 런타임에 강제로 덮어씁니다.
	if (USkeletalMeshComponent* MyMesh = GetMesh())
	{
		MyMesh->SetOwnerNoSee(false);
		MyMesh->SetOnlyOwnerSee(false);
	}
}

void ANoobGameCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// (신규!) 에디터에서 블루프린트를 컴파일할 때마다 이 값을 강제로 덮어씁니다.
	if (USkeletalMeshComponent* MyMesh = GetMesh())
	{
		MyMesh->SetOwnerNoSee(false);
		MyMesh->SetOnlyOwnerSee(false);
	}
}

/** 리플리케이션(복제)할 변수 등록 */
void ANoobGameCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANoobGameCharacter, bIsRagdolling);
}

/** 서버에서 래그돌 상태를 변경하는 함수 */
void ANoobGameCharacter::SetRagdollState_Server(bool bEnable)
{
	if (HasAuthority())
	{
		bIsRagdolling = bEnable;
		SetRagdoll(bIsRagdolling);
	}
}

/** bIsRagdolling 변수가 복제되었을 때 호출됨 (클라이언트) */
void ANoobGameCharacter::OnRep_IsRagdolling()
{
	SetRagdoll(bIsRagdolling);
}

/** (수정!) 회복 애니메이션 종료 후 호출되는 함수 */
void ANoobGameCharacter::EnableMovementAfterRecovery()
{
	// 1. 이동 활성화
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->SetMovementMode(EMovementMode::MOVE_Walking);
	}

	// 2. 카메라/비네팅 효과 해제
	if (AFruitPlayerController* PC = Cast<AFruitPlayerController>(GetController()))
	{
		PC->Client_SetCameraEffect(false);
	}
}

/** 래그돌 상태를 실제 켜고 끄는 함수 */
void ANoobGameCharacter::SetRagdoll(bool bEnable)
{
	USkeletalMeshComponent* MeshComp = GetMesh();
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (!MeshComp || !CapsuleComp || !MovementComp) return;

	if (bEnable)
	{
		// --- 1. 래그돌 켜기 (쓰러짐) ---

		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MovementComp->DisableMovement();
		MeshComp->SetCollisionProfileName(TEXT("Ragdoll"));
		MeshComp->SetSimulatePhysics(true);

		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			PC->SetControlRotation(FRotator(0.f, PC->GetControlRotation().Yaw, 0.f));
		}

		GetWorldTimerManager().ClearTimer(RecoveryMovementTimerHandle);
	}
	else
	{
		// --- 2. 래그돌 끄기 (일어남) ---

		// (신규!) "스냅" 현상 방지: 캡슐(액터)을 래그돌 메쉬의 위치로 먼저 이동시킵니다.
		if (MeshComp->IsSimulatingPhysics())
		{
			// 1. 래그돌 뼈의 위치(골반)를 가져옵니다. (피직스 애셋에 'pelvis' 뼈가 있어야 함)
			FVector PelvisWorldLocation = MeshComp->GetSocketLocation(FName("pelvis"));
			// 2. 캡슐의 절반 높이를 가져옵니다.
			float CapsuleHalfHeight = CapsuleComp->GetScaledCapsuleHalfHeight();
			// 3. 캡슐의 새 월드 위치(바닥)를 설정합니다. X,Y는 뼈를 따르고, Z는 뼈 위치에서 캡슐 절반 높이를 뺀 값입니다.
			FVector NewActorLocation = FVector(PelvisWorldLocation.X, PelvisWorldLocation.Y, PelvisWorldLocation.Z - CapsuleHalfHeight);

			// 4. 피직스를 먼저 끄고 (이동 시 래그돌이 영향받지 않도록)
			MeshComp->SetSimulatePhysics(false);

			// 5. 액터(캡슐)를 래그돌 위치로 순간이동시킵니다.
			SetActorLocation(NewActorLocation, false, nullptr, ETeleportType::TeleportPhysics);
		}
		else
		{
			MeshComp->SetSimulatePhysics(false);
		}

		MeshComp->SetCollisionProfileName(TEXT("CharacterMesh"));

		// (수정!) 캡슐과 메쉬가 이미 같은 위치에 있으므로, 이 코드는 더 이상 '스냅'이 아니라 '재연결'이 됩니다.
		MeshComp->AttachToComponent(CapsuleComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		MeshComp->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));

		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		float AnimDuration = 1.5f;

		if (RecoverMontage)
		{
			AnimDuration = PlayAnimMontage(RecoverMontage);
		}

		GetWorldTimerManager().ClearTimer(RecoveryMovementTimerHandle);
		GetWorldTimerManager().SetTimer(
			RecoveryMovementTimerHandle,
			this,
			&ANoobGameCharacter::EnableMovementAfterRecovery,
			AnimDuration,
			false
		);
	}
}

void ANoobGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// (래그돌 상태일 때 캡슐을 이동시키는 로직은 여기서 제거되었습니다)
}