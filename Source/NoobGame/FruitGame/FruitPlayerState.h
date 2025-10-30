// FruitPlayerState.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FruitGame/FruitGameTypes.h"
#include "FruitPlayerState.generated.h"

// UI 바인딩을 위한 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReadyStateChanged); // Setup 단계 완료
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInstructionReadyChanged); // Instructions 단계 준비 완료

UCLASS()
class NOOBGAME_API AFruitPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AFruitPlayerState();

	// 리플리케이트할 변수들을 등록합니다.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// --- 정답 배열 (보안) ---

	// 정답 과일 배열 (UPROPERTY 없음! 복제(Replicate)되지 않음!)
	// 이 변수는 서버에만 존재합니다.
private:
	TArray<EFruitType> SecretFruitAnswers;

public:
	/** * [서버 전용] 플레이어의 정답 과일 배열을 설정합니다.
	 * @param Answers 플레이어가 선택한 정답 배열
	 */
	void SetSecretAnswers_Server(const TArray<EFruitType>& Answers);

	/** * [서버 전용] 플레이어의 정답 과일 배열을 가져옵니다.
	 * @return 정답 배열 (const 참조)
	 */
	const TArray<EFruitType>& GetSecretAnswers_Server() const;

	/** * (신규) [서버 전용] 이 플레이어를 Instructions 단계에서 준비 상태로 설정합니다.
	 */
	void SetInstructionReady_Server();


	// --- 준비 상태 변수 (복제됨) ---

	/** (기존) Setup 단계에서 정답을 제출했는지 여부 */
	UPROPERTY(ReplicatedUsing = OnRep_HasSubmittedFruits)
	bool bHasSubmittedFruits;

	/** (신규) Instructions 단계에서 준비를 눌렀는지 여부 */
	UPROPERTY(ReplicatedUsing = OnRep_IsReady_Instructions)
	bool bIsReady_Instructions;


	// --- UI 바인딩용 델리게이트 ---

	/** Setup 단계 정답 제출 상태 변경 시 호출 */
	UPROPERTY(BlueprintAssignable, Category = "Player State")
	FOnReadyStateChanged OnReadyStateChanged;

	/** Instructions 단계 준비 상태 변경 시 호출 */
	UPROPERTY(BlueprintAssignable, Category = "Player State")
	FOnInstructionReadyChanged OnInstructionReadyChanged;

protected:
	/** bHasSubmittedFruits 변수가 복제될 때 클라이언트에서 호출됨 */
	UFUNCTION()
	void OnRep_HasSubmittedFruits();

	/** bIsReady_Instructions 변수가 복제될 때 클라이언트에서 호출됨 */
	UFUNCTION()
	void OnRep_IsReady_Instructions();
};