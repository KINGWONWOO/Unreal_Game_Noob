#pragma once

#include "CoreMinimal.h"
#include "NoobGameStateBase.h"
#include "MazeGameState.generated.h"

// [Delegates]
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMazeGamePhaseChanged, EMazeGamePhase, NewPhase);
// [New] 카운트다운 변경 알림 델리게이트 추가
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMazePlayingCountdownChanged, int32, TimeLeft);

UCLASS()
class NOOBGAME_API AMazeGameState : public ANoobGameStateBase
{
	GENERATED_BODY()

public:
	AMazeGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ──────────────────────────────────────────────────────────────────────────
	// Replicated Properties
	// ──────────────────────────────────────────────────────────────────────────
	UPROPERTY(ReplicatedUsing = OnRep_GamePhase, BlueprintReadOnly, Category = "Maze Game State")
	EMazeGamePhase CurrentGamePhase;

	// ──────────────────────────────────────────────────────────────────────────
	// Setters (Server Only)
	// ──────────────────────────────────────────────────────────────────────────
	// [New] 카운트다운 설정 함수
	void SetPlayingCountdown(int32 TimeLeft);

	// ──────────────────────────────────────────────────────────────────────────
	// Delegates (UI Binding)
	// ──────────────────────────────────────────────────────────────────────────
	UPROPERTY(BlueprintAssignable, Category = "Maze Game State")
	FOnMazeGamePhaseChanged OnGamePhaseChanged;

	// [New] UI 바인딩용 카운트다운 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Maze Game State")
	FOnMazePlayingCountdownChanged OnPlayingCountdownChanged;

protected:
	// [New] 리플리케이션 변수 (카운트다운)
	UPROPERTY(ReplicatedUsing = OnRep_PlayingCountdown, BlueprintReadOnly, Category = "Maze Game State")
	int32 PlayingCountdown;

	UFUNCTION()
	void OnRep_GamePhase();

	// [New] OnRep 함수
	UFUNCTION()
	void OnRep_PlayingCountdown();
};