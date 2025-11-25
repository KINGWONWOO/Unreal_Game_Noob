#pragma once

#include "CoreMinimal.h"
#include "NoobGameStateBase.h" // Parent
#include "GameTypes.h"
#include "FruitGameState.generated.h"

// Fruit 전용 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePhaseChanged, EFruitGamePhase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFirstTurnPlayerDetermined, int32, StartingPlayerState);

UCLASS()
class NOOBGAME_API AFruitGameState : public ANoobGameStateBase
{
	GENERATED_BODY()

public:
	AFruitGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ──────────────────────────────────────────────────────────────────────────
	// Fruit Specific Replicated Properties
	// ──────────────────────────────────────────────────────────────────────────
	UPROPERTY(ReplicatedUsing = OnRep_GamePhase, BlueprintReadOnly, Category = "Fruit Game State")
	EFruitGamePhase CurrentGamePhase;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentActivePlayer, BlueprintReadOnly, Category = "Fruit Game State")
	APlayerState* CurrentActivePlayer;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Fruit Game State")
	float ServerTimeAtTurnStart;

	// ──────────────────────────────────────────────────────────────────────────
	// Delegates
	// ──────────────────────────────────────────────────────────────────────────
	UPROPERTY(BlueprintAssignable, Category = "Fruit Game State")
	FOnGamePhaseChanged OnGamePhaseChanged;

	UPROPERTY(BlueprintAssignable, Category = "Fruit Game State")
	FOnFirstTurnPlayerDetermined OnFirstTurnPlayerDetermined;

protected:
	UFUNCTION()
	void OnRep_GamePhase();

	UFUNCTION()
	void OnRep_CurrentActivePlayer();
};