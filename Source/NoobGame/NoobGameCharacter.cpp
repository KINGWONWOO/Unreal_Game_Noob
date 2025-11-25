// NoobGameCharacter.cpp

#include "NoobGame/NoobGameCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h" // (신규!) 입력 컴포넌트
#include "GameFramework/PlayerController.h"   // (신규!) 컨트롤러
#include "Animation/AnimInstance.h"
#include "Net/UnrealNetwork.h" // (신규!) 리플리케이션
#include "TimerManager.h" // (신규!) 타이머

ANoobGameCharacter::ANoobGameCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 캡슐 컴포넌트 설정
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// 캐릭터 무브먼트 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->bUseControllerDesiredRotation = false; // (중요) 래그돌 회복 시 필요
	bUseControllerRotationYaw = false; // (중요) 래그돌 회복 시 필요

	// 메시 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -97.0f), FRotator(0.0f, -90.0f, 0.0f));

	// (신규!) 카메라 변수 기본값 설정
	CameraSensitivity = 1.0f;
	ReverseX = false;
	ReverseY = false;

	// 물리 블렌딩 변수 기본값
	bIsRagdolling = false;
	bIsBlendingFromRagdoll = false;
	RagdollBlendWeight = 0.0f;
	RagdollBlendDuration = 0.5f; // 0.5초 동안 블렌딩
}

void ANoobGameCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ANoobGameCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ANoobGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 래그돌에서 애니메이션으로 블렌딩 중일 때 처리
	if (bIsBlendingFromRagdoll)
	{
		// ... (기존 블렌딩 로직 동일) ...
		RagdollBlendWeight = FMath::FInterpTo(RagdollBlendWeight, 0.0f, DeltaTime, 1.0f / RagdollBlendDuration);
		GetMesh()->SetPhysicsBlendWeight(RagdollBlendWeight);

		if (FMath::IsNearlyZero(RagdollBlendWeight))
		{
			bIsBlendingFromRagdoll = false;

			// 1. 캡슐(Actor)을 현재 메시(누워있는 곳) 위치로 이동
			// (일어날 때 제자리에서 일어나게 하기 위함)
			FVector NewCapsuleLocation = GetMesh()->GetSocketLocation(FName("root")); // 혹은 "Pelvis"

			// 바닥 높이 보정 (캡슐의 절반 높이만큼 위로 올려야 함)
			// 땅에 파묻히지 않게 하기 위해 LineTrace를 쓰기도 하지만, 간단히는 아래처럼 처리
			float CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			NewCapsuleLocation.Z += CapsuleHalfHeight;

			SetActorLocation(NewCapsuleLocation, false, nullptr, ETeleportType::TeleportPhysics);

			// 2. 래그돌 끄기
			SetRagdoll(false);

			// 3. [핵심 추가] 메시를 캡슐 내부의 원래 위치로 '즉시' 복구
			// 이 코드가 없으면 캡슐은 서 있어도 메시는 캡슐 옆에 누워있게 됩니다.
			GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

			// 생성자에서 설정했던 초기값(Z -97, Yaw -90 등)을 그대로 넣어주세요.
			GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -97.0f), FRotator(0.0f, -90.0f, 0.0f));

			// 4. 카메라 및 이동 복구 (기존 코드 유지)
			if (APlayerController* PC = Cast<APlayerController>(GetController()))
			{
				TargetControlRotation = GetActorRotation();
				PC->SetControlRotation(TargetControlRotation);
			}

			GetWorldTimerManager().SetTimer(RecoveryMovementTimerHandle, this, &ANoobGameCharacter::EnableMovementAfterRecovery, 0.2f, false);
		}
	}
}

/**
 * (신규!) 입력 컴포넌트 설정
 * 마우스 입력을 Turn/LookUp 함수에 바인딩합니다.
 */
void ANoobGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// (여기에 점프, 이동 등 기본 입력 바인딩)
	// PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	// PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	// (신규!) 마우스 카메라 입력 바인딩
	PlayerInputComponent->BindAxis("Turn", this, &ANoobGameCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ANoobGameCharacter::LookUp);
}

/**
 * (신규!) 마우스 좌우 (Turn) 입력 처리
 */
void ANoobGameCharacter::Turn(float Value)
{
	if (Value != 0.0f && Controller != nullptr)
	{
		// 감도 및 X축 반전 적용
		const float FinalValue = Value * CameraSensitivity * (ReverseX ? -1.0f : 1.0f);
		AddControllerYawInput(FinalValue);
	}
}

/**
 * (신규!) 마우스 상하 (LookUp) 입력 처리
 */
void ANoobGameCharacter::LookUp(float Value)
{
	if (Value != 0.0f && Controller != nullptr)
	{
		// 감도 및 Y축 반전 적용
		const float FinalValue = Value * CameraSensitivity * (ReverseY ? -1.0f : 1.0f);
		AddControllerPitchInput(FinalValue);
	}
}

/**
 * 리플리케이션(복제) 설정
 */
void ANoobGameCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// bIsRagdolling 변수를 모든 클라이언트에 복제합니다.
	DOREPLIFETIME(ANoobGameCharacter, bIsRagdolling);
}

/**
 * 래그돌 상태를 실제로 켜고 끄는 로직
 */
void ANoobGameCharacter::SetRagdoll(bool bEnable)
{
	if (bEnable)
	{
		// 래그돌 활성화
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->SetComponentTickEnabled(false);

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		bUseControllerRotationYaw = false;
	}
	else
	{
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		bUseControllerRotationYaw = false;
	}
}

/**
 * 서버에서 래그돌 상태를 설정하고 클라이언트에 전파
 */
void ANoobGameCharacter::SetRagdollState_Server(bool bEnable)
{
	if (HasAuthority())
	{
		if (bIsRagdolling == bEnable) return; // 이미 같은 상태면 무시

		bIsRagdolling = bEnable;

		if (bIsRagdolling)
		{
			// 래그돌 켜기 (즉시)
			OnRep_IsRagdolling();
		}
		else
		{
			// 래그돌 끄기 (블렌딩 시작)
			// 클라이언트에서 OnRep이 호출되어 처리하도록 함
		}

		// 서버 자신도 OnRep을 호출하여 로직을 실행
		// (bEnable이 false일 때 즉시 끄지 않고, OnRep에서 블렌딩을 시작하도록 유도)
		OnRep_IsRagdolling();
	}
}

/**
 * 래그돌 상태가 클라이언트에 복제되었을 때 호출됨
 */
void ANoobGameCharacter::OnRep_IsRagdolling()
{
	if (bIsRagdolling)
	{
		// 래그돌 켜기 (즉시)
		bIsBlendingFromRagdoll = false;
		GetMesh()->SetPhysicsBlendWeight(1.0f);
		SetRagdoll(true);
	}
	else
	{
		// 래그돌 끄기 (블렌딩 시작)
		bIsBlendingFromRagdoll = true;
		RagdollBlendWeight = 1.0f; // 1.0 (물리)에서 0.0 (애니)으로 블렌딩 시작
	}
}

/**
 * 래그돌 회복 후 이동 활성화
 */
void ANoobGameCharacter::EnableMovementAfterRecovery()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetCharacterMovement()->SetComponentTickEnabled(true);
}

/**
 * (구현 필요) 카메라 회전 보간
 */
void ANoobGameCharacter::UpdateCameraRecovery()
{
	// (필요시 구현: 예: APlayerController* PC = ...)
	// FMath::RInterpTo()를 사용하여 현재 ControlRotation을 TargetControlRotation으로 보간
}

/**
 * 게임 종료 애니메이션 멀티캐스트 구현부
 */
void ANoobGameCharacter::Multicast_PlayEndGameAnim_Implementation(bool bIsWinner)
{
	UAnimMontage* MontageToPlay = bIsWinner ? VictoryMontage : DefeatMontage;

	if (MontageToPlay)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(MontageToPlay, 1.0f);
		}
	}
}