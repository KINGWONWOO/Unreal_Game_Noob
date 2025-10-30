// FruitGameState.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FruitGame/FruitGameTypes.h"
#include "GameFramework/PlayerState.h"
#include "FruitGameState.generated.h"

// UI 바인딩을 위한 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePhaseChanged, EGamePhase, NewPhase);
/** Delegate to notify UI when the starting player is determined */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFirstTurnPlayerDetermined, int32, StartingPlayerState);

UCLASS()
class NOOBGAME_API AFruitGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AFruitGameState();

	// 리플리케이트할 변수들을 등록합니다.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 현재 게임 단계 (모두가 알아야 함)
	UPROPERTY(ReplicatedUsing = OnRep_GamePhase)
	EGamePhase CurrentGamePhase;

	// (수정!) CurrentActivePlayer가 복제될 때 OnRep 함수 호출
	UPROPERTY(ReplicatedUsing = OnRep_CurrentActivePlayer, BlueprintReadOnly, Category = "Game State")
	APlayerState* CurrentActivePlayer;

	/** (신규) 턴이 시작된 '서버'의 절대 시간입니다. */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
	float ServerTimeAtTurnStart;

	// 승자 (모두가 알아야 함)
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
	APlayerState* Winner;

	// 게임 단계가 변경될 때 UI에 알리기 위한 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Game State")
	FOnGamePhaseChanged OnGamePhaseChanged;

	UPROPERTY(BlueprintAssignable, Category = "Game State")
	FOnFirstTurnPlayerDetermined OnFirstTurnPlayerDetermined;

protected:
	// CurrentGamePhase가 클라이언트에서 복제될 때 호출됩니다.
	UFUNCTION()
	void OnRep_GamePhase();

	// (신규!) CurrentActivePlayer가 클라이언트에서 복제될 때 호출
	UFUNCTION()
	void OnRep_CurrentActivePlayer();
};