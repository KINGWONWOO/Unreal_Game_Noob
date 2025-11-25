#pragma once

#include "CoreMinimal.h"
#include "NoobPlayerState.h" // Parent
#include "GameTypes.h"       // EFruitType
#include "FruitPlayerState.generated.h"

UCLASS()
class NOOBGAME_API AFruitPlayerState : public ANoobPlayerState
{
	GENERATED_BODY()

public:
	AFruitPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ──────────────────────────────────────────────────────────────────────────
	// Fruit Game Specific API
	// ──────────────────────────────────────────────────────────────────────────
	void SetSecretAnswers_Server(const TArray<EFruitType>& SecretFruits);
	const TArray<EFruitType>& GetSecretAnswers_Server() const;

	// ──────────────────────────────────────────────────────────────────────────
	// Fruit Game Specific Properties
	// ──────────────────────────────────────────────────────────────────────────

	/** Setup 단계 과일 제출 완료 여부 */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Fruit Game")
	bool bHasSubmittedFruits;

	/** 이 플레이어의 비밀 정답 (서버 저장 및 복제) */
	UPROPERTY(Replicated)
	TArray<EFruitType> SecretAnswers;
};