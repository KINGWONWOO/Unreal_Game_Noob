#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameTypes.h" // EQuizDifficulty
#include "OXQuizGameState.generated.h"

class APlayerState;

// ──────────────────────────────────────────────────────────────────────────
// Delegate Declarations
// ──────────────────────────────────────────────────────────────────────────
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpeedLevelChanged, int32, NewSpeedLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayingCountdownChanged, int32, TimeLeft);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOXGamePhaseChanged, EQuizGamePhase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuizWinnerAnnouncement, FString, WinnerName);

// [New] 난이도 변경 시 UI에 알리는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDifficultyChanged, EQuizDifficulty, NewDifficulty);

UCLASS()
class NOOBGAME_API AOXQuizGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AOXQuizGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ──────────────────────────────────────────────────────────────────────────
	// Replicated Properties
	// ──────────────────────────────────────────────────────────────────────────
	UPROPERTY(ReplicatedUsing = OnRep_GamePhase, BlueprintReadOnly, Category = "Game State")
	EQuizGamePhase CurrentGamePhase;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
	APlayerState* Winner;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
	ECharacterType WinningCharacterType;

	// [New] 현재 난이도 (동기화됨)
	UPROPERTY(ReplicatedUsing = OnRep_CurrentDifficulty, BlueprintReadOnly, Category = "Game State")
	EQuizDifficulty CurrentDifficulty;

	// ──────────────────────────────────────────────────────────────────────────
	// Public Setters & RPCs
	// ──────────────────────────────────────────────────────────────────────────
	void SetCurrentSpeedLevel(int32 NewLevel);
	void SetPlayingCountdown(int32 TimeLeft);

	// [New] 난이도 설정 (서버만 호출 가능)
	void SetRepDifficulty(EQuizDifficulty NewDifficulty);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AnnounceWinner(const FString& WinnerName);

	// ──────────────────────────────────────────────────────────────────────────
	// Delegates (UI Binding)
	// ──────────────────────────────────────────────────────────────────────────
	UPROPERTY(BlueprintAssignable, Category = "Game|State")
	FOnOXGamePhaseChanged OnGamePhaseChanged;

	UPROPERTY(BlueprintAssignable, Category = "Game|Speed")
	FOnSpeedLevelChanged OnSpeedLevelChanged;

	UPROPERTY(BlueprintAssignable, Category = "Game|Countdown")
	FOnPlayingCountdownChanged OnPlayingCountdownChanged;

	UPROPERTY(BlueprintAssignable, Category = "Game|State")
	FOnQuizWinnerAnnouncement OnWinnerAnnouncement;

	// [New] 난이도 변경 알림 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Game|Settings")
	FOnDifficultyChanged OnDifficultyChanged;

	/** * 루프린트 전용 헬퍼 함수
	 * - BlueprintPure: 실행 핀 없이 변수처럼 바로 가져옴
	 * - meta = (WorldContext...): 이 함수를 호출하는 곳의 World 정보를 자동으로 가져옴
	 */
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

	// [New] 난이도 변경 시 호출됨
	UFUNCTION()
	void OnRep_CurrentDifficulty();
};