// OXQuizPlayerState.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NoobPlayerState.h"
#include "OXQuizPlayerState.generated.h"

UCLASS()
class NOOBGAME_API AOXQuizPlayerState : public ANoobPlayerState
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

	/** * [Helper] 현재 클라이언트(로컬 플레이어)의 PlayerState를 가져오는 함수
	 * 블루프린트에서 "Get OX Player State"로 검색해서 사용하세요.
	 */
	UFUNCTION(BlueprintPure, Category = "Game Helper", meta = (WorldContext = "WorldContextObject"))
	static AOXQuizPlayerState* GetOXPlayerState(const UObject* WorldContextObject);
};