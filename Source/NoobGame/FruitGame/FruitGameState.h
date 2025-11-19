#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameTypes.h"
#include "GameFramework/PlayerState.h"
#include "FruitGameState.generated.h"

// ──────────────────────────────────────────────────────────────────────────
// Delegate Declarations
// ──────────────────────────────────────────────────────────────────────────
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePhaseChanged, EFruitGamePhase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFirstTurnPlayerDetermined, int32, StartingPlayerState);

// [New] 승자 발표 UI 출력을 위한 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFruitWinnerAnnouncement, FString, WinnerName);

UCLASS()
class NOOBGAME_API AFruitGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AFruitGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ──────────────────────────────────────────────────────────────────────────
	// Replicated Properties
	// ──────────────────────────────────────────────────────────────────────────
	UPROPERTY(ReplicatedUsing = OnRep_GamePhase, BlueprintReadOnly, Category = "Game State")
	EFruitGamePhase CurrentGamePhase;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentActivePlayer, BlueprintReadOnly, Category = "Game State")
	APlayerState* CurrentActivePlayer;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
	float ServerTimeAtTurnStart;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
	APlayerState* Winner;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
	ECharacterType WinningCharacterType;

	// ──────────────────────────────────────────────────────────────────────────
	// Public RPCs
	// ──────────────────────────────────────────────────────────────────────────
	/** [New] 서버가 호출하면 모든 클라이언트에서 실행되어 UI 델리게이트를 방송합니다. */
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AnnounceWinner(const FString& WinnerName);

	// ──────────────────────────────────────────────────────────────────────────
	// Delegates (UI Binding)
	// ──────────────────────────────────────────────────────────────────────────
	UPROPERTY(BlueprintAssignable, Category = "Game State")
	FOnGamePhaseChanged OnGamePhaseChanged;

	UPROPERTY(BlueprintAssignable, Category = "Game State")
	FOnFirstTurnPlayerDetermined OnFirstTurnPlayerDetermined;

	/** [New] UI 위젯에서 이 이벤트를 바인딩하여 승자 텍스트를 띄우세요. */
	UPROPERTY(BlueprintAssignable, Category = "Game State")
	FOnFruitWinnerAnnouncement OnWinnerAnnouncement;

protected:
	UFUNCTION()
	void OnRep_GamePhase();

	UFUNCTION()
	void OnRep_CurrentActivePlayer();
};