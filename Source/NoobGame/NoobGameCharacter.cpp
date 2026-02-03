#include "NoobGame/NoobGameCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

ANoobGameCharacter::ANoobGameCharacter()
{
	// 최적화: 기본 Tick은 꺼두거나 카메라 로직을 제거합니다.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -97.0f), FRotator(0.0f, -90.0f, 0.0f));
	bReplicates = true;

	CameraSensitivity = 1.0f;
	ReverseX = false;
	ReverseY = false;
	bIsDown = false;
	TargetCameraZ = 130.0f; // 기본 카메라 높이 초기값
}

void ANoobGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	// 블루프린트에서 생성된 카메라 컴포넌트 찾기
	FirstPersonCameraComponent = Cast<UCameraComponent>(GetComponentByClass(UCameraComponent::StaticClass()));
}

void ANoobGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANoobGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Turn", this, &ANoobGameCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ANoobGameCharacter::LookUp);
}

void ANoobGameCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANoobGameCharacter, bIsDown);
}

void ANoobGameCharacter::Turn(float Value)
{
	if (Value != 0.0f && Controller != nullptr && !bIsDown)
	{
		const float FinalValue = Value * CameraSensitivity * (ReverseX ? -1.0f : 1.0f);
		AddControllerYawInput(FinalValue);
	}
}

void ANoobGameCharacter::LookUp(float Value)
{
	if (Value != 0.0f && Controller != nullptr && !bIsDown)
	{
		const float FinalValue = Value * CameraSensitivity * (ReverseY ? -1.0f : 1.0f);
		AddControllerPitchInput(FinalValue);
	}
}

void ANoobGameCharacter::SetDownState_Server(bool bInDown)
{
	if (HasAuthority())
	{
		if (bIsDown == bInDown) return;
		bIsDown = bInDown;

		// [카메라 최적화 보간 시작]
		TargetCameraZ = bIsDown ? -40.0f : 60.0f; // 목표 높이 설정
		GetWorldTimerManager().ClearTimer(CameraInterpTimerHandle);
		// 0.01초 간격으로 업데이트 (Tick 대신 동작)
		GetWorldTimerManager().SetTimer(CameraInterpTimerHandle, this, &ANoobGameCharacter::UpdateCameraHeight, 0.01f, true);

		OnRep_IsDown();
	}
}

void ANoobGameCharacter::UpdateCameraHeight()
{
	if (!FirstPersonCameraComponent)
	{
		GetWorldTimerManager().ClearTimer(CameraInterpTimerHandle);
		return;
	}

	FVector CurrentLoc = FirstPersonCameraComponent->GetRelativeLocation();

	// 목표치에 거의 도달했는지 확인 (오차 범위 0.1)
	if (FMath::IsNearlyEqual(CurrentLoc.Z, TargetCameraZ, 0.1f))
	{
		FirstPersonCameraComponent->SetRelativeLocation(FVector(CurrentLoc.X, CurrentLoc.Y, TargetCameraZ));
		GetWorldTimerManager().ClearTimer(CameraInterpTimerHandle); // 도달 시 타이머 종료 (최적화)
		return;
	}

	// 부드러운 보간 (5.0f는 속도)
	float NewZ = FMath::FInterpTo(CurrentLoc.Z, TargetCameraZ, 0.01f, 1.0f);
	FirstPersonCameraComponent->SetRelativeLocation(FVector(CurrentLoc.X, CurrentLoc.Y, NewZ));
}

void ANoobGameCharacter::OnRep_IsDown()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	// 클라이언트에서도 카메라 보간을 동기화하기 위해 타겟 설정 및 타이머 시작
	TargetCameraZ = bIsDown ? 30.0f :130.0f;
	GetWorldTimerManager().ClearTimer(CameraInterpTimerHandle);
	GetWorldTimerManager().SetTimer(CameraInterpTimerHandle, this, &ANoobGameCharacter::UpdateCameraHeight, 0.01f, true);

	if (bIsDown)
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
		if (KnockdownMontage) AnimInstance->Montage_Play(KnockdownMontage);
		if (KnockdownSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				KnockdownSound,
				GetActorLocation(),
				1.0f, 1.0f, 0.0f,
				KnockdownAttenuation // 감쇄 설정 적용
			);
		}
	}
	else
	{
		if (GetUpMontage)
		{
			AnimInstance->Montage_Play(GetUpMontage);
			float Duration = GetUpMontage->GetPlayLength();
			GetWorldTimerManager().SetTimer(RecoveryTimerHandle, this, &ANoobGameCharacter::EnableMovementAfterRecovery, Duration, false);
		}
	}
}

void ANoobGameCharacter::EnableMovementAfterRecovery()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void ANoobGameCharacter::Multicast_PlayEndGameAnim_Implementation(bool bIsWinner)
{
	UAnimMontage* MontageToPlay = bIsWinner ? VictoryMontage : DefeatMontage;
	if (MontageToPlay && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay, 1.0f);
	}
}

float ANoobGameCharacter::TakeDamage(float DamageAmount,FDamageEvent const& DamageEvent,AController* EventInstigator,AActor* DamageCauser)
{
	if (HasAuthority())
	{
		Multicast_PlayHitSound(); // 서버라면 바로
	}
	else
	{
		Server_PlayHitSound();    // 클라라면 서버로 요청
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ANoobGameCharacter::Server_PlayHitSound_Implementation()
{
	Multicast_PlayHitSound();
}

void ANoobGameCharacter::Multicast_PlayHitSound_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Playing hit sound on %s"), *GetName());

	// 피격 사운드 재생
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			GetActorLocation()
		);
	}
}