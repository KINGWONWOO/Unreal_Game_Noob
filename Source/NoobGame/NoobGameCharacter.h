// NoobGameCharacter.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NoobGameCharacter.generated.h"

class UAnimMontage;

UCLASS()
class NOOBGAME_API ANoobGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ANoobGameCharacter();

	/** (수정!) BeginPlay 함수 선언 추가 */
	virtual void BeginPlay() override;

	/** (신규!) OnConstruction 함수 선언 추가 */
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaTime) override;

	/** 래그돌 상태를 실제 켜고 끄는 함수 */
	void SetRagdoll(bool bEnable);

	/** 서버(GameMode)에서 래그돌 상태를 변경하기 위한 public 함수 */
	void SetRagdollState_Server(bool bEnable);

	// --- 몽타주 애셋 ---
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Montages")
	UAnimMontage* LeftPunchMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	UAnimMontage* RightPunchMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	UAnimMontage* HitReactionMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	UAnimMontage* KnockdownMontage; // (현재 래그돌 사용으로 미사용)

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	UAnimMontage* RecoverMontage; // (래그돌에서 일어날 때 사용됨)

protected:
	/** 래그돌 상태 (서버에서 변경되면 클라이언트로 복제됨) */
	UPROPERTY(ReplicatedUsing = OnRep_IsRagdolling)
	bool bIsRagdolling;

	/** bIsRagdolling 변수가 복제될 때 클라이언트에서 호출될 함수 */
	UFUNCTION()
	void OnRep_IsRagdolling();

	/** 리플리케이션(복제) 설정 함수 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** (신규!) 회복 애니메이션이 끝난 후 이동을 활성화하기 위한 타이머 */
	FTimerHandle RecoveryMovementTimerHandle;

	/** (신규!) 타이머가 만료되면 이동을 활성화하는 함수 */
	void EnableMovementAfterRecovery();
};