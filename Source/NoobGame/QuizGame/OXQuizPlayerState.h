// OXQuizPlayerState.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "OXQuizPlayerState.generated.h"

UCLASS()
class NOOBGAME_API AOXQuizPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	// ──────────────────────────────────────────────────────────────────────────
	// Constructor & Framework Overrides
	// ──────────────────────────────────────────────────────────────────────────
	AOXQuizPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ──────────────────────────────────────────────────────────────────────────
	// Public Server API (Setters called by GameMode/Controller)
	// ──────────────────────────────────────────────────────────────────────────

	// --- Ready Phase ---
	void SetInstructionReady_Server();

	// ──────────────────────────────────────────────────────────────────────────
	// Replicated Properties (Networked Variables)
	// ──────────────────────────────────────────────────────────────────────────
	UPROPERTY(Replicated)
	TSubclassOf<APawn> SelectedPawnClass;

	/** Instructions 단계 준비 완료 여부 */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
	bool bIsReady_Instructions;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
	bool bIsRoomOwner = false;

	// ──────────────────────────────────────────────────────────────────────────
	// Replicated Properties (Combat State)
	// ──────────────────────────────────────────────────────────────────────────

	/** 현재 펀치 맞은 횟수 (쓰러짐 판정용) */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat")
	int32 PunchHitCount;

	/** 현재 쓰러진 상태인지 여부 */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat")
	bool bIsKnockedDown;

	/** 다음 펀치가 왼쪽인지 여부 */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat")
	bool bIsNextPunchLeft;

	/** * [Helper] 현재 클라이언트(로컬 플레이어)의 PlayerState를 가져오는 함수
	 * 블루프린트에서 "Get OX Player State"로 검색해서 사용하세요.
	 */
	UFUNCTION(BlueprintPure, Category = "Game Helper", meta = (WorldContext = "WorldContextObject"))
	static AOXQuizPlayerState* GetOXPlayerState(const UObject* WorldContextObject);
};