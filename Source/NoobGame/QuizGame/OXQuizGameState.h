#pragma once

#include "CoreMinimal.h"
#include "NoobGameStateBase.h" // Parent
#include "GameTypes.h"
#include "OXQuizGameState.generated.h"

// OX 전용 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpeedLevelChanged, int32, NewSpeedLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayingCountdownChanged, int32, TimeLeft);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOXGamePhaseChanged, EQuizGamePhase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDifficultyChanged, EQuizDifficulty, NewDifficulty);

UCLASS()
class NOOBGAME_API AOXQuizGameState : public ANoobGameStateBase
{
	GENERATED_BODY()

public:
	AOXQuizGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ──────────────────────────────────────────────────────────────────────────
	// OX Quiz Specific Replicated Properties
	// ──────────────────────────────────────────────────────────────────────────
	UPROPERTY(ReplicatedUsing = OnRep_GamePhase, BlueprintReadOnly, Category = "OX Game State")
	EQuizGamePhase CurrentGamePhase;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentDifficulty, BlueprintReadOnly, Category = "OX Game State")
	EQuizDifficulty CurrentDifficulty;

	// ──────────────────────────────────────────────────────────────────────────
	// Setters (Server Only)
	// ──────────────────────────────────────────────────────────────────────────
	void SetCurrentSpeedLevel(int32 NewLevel);
	void SetPlayingCountdown(int32 TimeLeft);
	void SetRepDifficulty(EQuizDifficulty NewDifficulty);

	// ──────────────────────────────────────────────────────────────────────────
	// Delegates
	// ──────────────────────────────────────────────────────────────────────────
	UPROPERTY(BlueprintAssignable, Category = "OX Game State")
	FOnOXGamePhaseChanged OnGamePhaseChanged;

	UPROPERTY(BlueprintAssignable, Category = "OX Game State")
	FOnSpeedLevelChanged OnSpeedLevelChanged;

	UPROPERTY(BlueprintAssignable, Category = "OX Game State")
	FOnPlayingCountdownChanged OnPlayingCountdownChanged;

	UPROPERTY(BlueprintAssignable, Category = "OX Game State")
	FOnDifficultyChanged OnDifficultyChanged;

	// Helper
	UFUNCTION(BlueprintPure, Category = "Game Helper", meta = (WorldContext = "WorldContextObject"))
	static AOXQuizGameState* GetOXGameState(const UObject* WorldContextObject);

protected:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentSpeedLevel)
	int32 CurrentSpeedLevel;

	UPROPERTY(ReplicatedUsing = OnRep_PlayingCountdown)
	int32 PlayingCountdown;

	UFUNCTION()
	void OnRep_GamePhase();

	UFUNCTION()
	void OnRep_CurrentSpeedLevel();

	UFUNCTION()
	void OnRep_PlayingCountdown();

	UFUNCTION()
	void OnRep_CurrentDifficulty();
};