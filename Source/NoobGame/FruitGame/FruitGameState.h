#pragma once

#include "CoreMinimal.h"
#include "NoobGameStateBase.h" 
#include "GameTypes.h"
#include "FruitGameState.generated.h"

// Fruit 전용 델리게이트 (UI 및 이벤트 통지용)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePhaseChanged, EFruitGamePhase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFirstTurnPlayerDetermined, int32, StartingPlayerState);

UCLASS()
class NOOBGAME_API AFruitGameState : public ANoobGameStateBase
{
    GENERATED_BODY()

public:
    AFruitGameState();

    // 네트워크 복제 설정
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // 동기화 변수 (Replicated Properties)
    UPROPERTY(ReplicatedUsing = OnRep_GamePhase, BlueprintReadOnly, Category = "Fruit Game State")
    EFruitGamePhase CurrentGamePhase;

    UPROPERTY(ReplicatedUsing = OnRep_CurrentActivePlayer, BlueprintReadOnly, Category = "Fruit Game State")
    APlayerState* CurrentActivePlayer;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Fruit Game State")
    float ServerTimeAtTurnStart;

    // 델리게이트 인스턴스
    UPROPERTY(BlueprintAssignable, Category = "Fruit Game State")
    FOnGamePhaseChanged OnGamePhaseChanged;

    UPROPERTY(BlueprintAssignable, Category = "Fruit Game State")
    FOnFirstTurnPlayerDetermined OnFirstTurnPlayerDetermined;

protected:
    // 클라이언트 측 응답 함수 (RepNotify)
    UFUNCTION()
    void OnRep_GamePhase();

    UFUNCTION()
    void OnRep_CurrentActivePlayer();
};