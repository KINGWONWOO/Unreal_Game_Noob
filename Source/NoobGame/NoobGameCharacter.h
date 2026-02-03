#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NoobGameCharacter.generated.h"

class UAnimMontage;
class USoundBase;
class USoundAttenuation;

UCLASS()
class NOOBGAME_API ANoobGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ANoobGameCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetDownState_Server(bool bInDown);

	FORCEINLINE bool GetIsDown() const { return bIsDown; }

	UFUNCTION(Server, Reliable)
	void Server_PlayHitSound();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayEndGameAnim(bool bIsWinner);

	// --- 몽타주 애셋 ---
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	UAnimMontage* LeftPunchMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	UAnimMontage* RightPunchMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Hit")
	UAnimMontage* HitReaction_Front;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Hit")
	UAnimMontage* HitReaction_Back;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Hit")
	UAnimMontage* HitReaction_Left;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Hit")
	UAnimMontage* HitReaction_Right;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|State")
	UAnimMontage* KnockdownMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|State")
	UAnimMontage* GetUpMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Result")
	UAnimMontage* VictoryMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Result")
	UAnimMontage* DefeatMontage;

	// --- 카메라 설정 ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraSensitivity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	bool ReverseX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	bool ReverseY;

	// 엔진의 기본 TakeDamage 함수를 오버라이드합니다.
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	// 피격 사운드를 모든 클라이언트에서 재생하기 위한 멀티캐스트 RPC
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayHitSound();

	// [추가] 피격 시 재생할 사운드 에셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* HitSound;

	// [추가] 사운드 감쇄 설정 (거리별 볼륨 조절)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundAttenuation* HitAttenuation;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_IsDown)
	bool bIsDown;

	UFUNCTION()
	void OnRep_IsDown();

	void EnableMovementAfterRecovery();
	void Turn(float Value);
	void LookUp(float Value);

	// --- [신규] 최적화된 카메라 이동 관련 ---
	FTimerHandle CameraInterpTimerHandle;
	float TargetCameraZ;
	void UpdateCameraHeight(); // 타이머가 호출할 보간 함수

	FTimerHandle RecoveryTimerHandle;

	// 카메라 컴포넌트 참조 (FirstPersonCameraComponent가 있다고 가정)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FirstPersonCameraComponent;

	// [추가] 기절 시 재생할 사운드 에셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* KnockdownSound;

	// [추가] 사운드 감쇄 설정 (거리별 볼륨 조절)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundAttenuation* KnockdownAttenuation;

};