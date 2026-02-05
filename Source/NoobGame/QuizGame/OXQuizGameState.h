#pragma once

#include "CoreMinimal.h"
#include "NoobGameStateBase.h" 
#include "GameTypes.h"
#include "OXQuizGameState.generated.h"

// OX 퀴즈의 상태 변화를 UI 등에 알리기 위한 델리게이트들
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

    // 네트워크 복제 설정
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // 동기화되는 게임 상태 변수들
    UPROPERTY(ReplicatedUsing = OnRep_GamePhase, BlueprintReadOnly, Category = "OX Game State")
    EQuizGamePhase CurrentGamePhase;

    UPROPERTY(ReplicatedUsing = OnRep_CurrentDifficulty, BlueprintReadOnly, Category = "OX Game State")
    EQuizDifficulty CurrentDifficulty;

    // 서버 전용 데이터 설정 함수 (Setters)
    void SetCurrentSpeedLevel(int32 NewLevel);
    void SetPlayingCountdown(int32 TimeLeft);
    void SetRepDifficulty(EQuizDifficulty NewDifficulty);

    // 외부에서 바인딩 가능한 이벤트 델리게이트들
    UPROPERTY(BlueprintAssignable, Category = "OX Game State")
    FOnOXGamePhaseChanged OnGamePhaseChanged;

    UPROPERTY(BlueprintAssignable, Category = "OX Game State")
    FOnSpeedLevelChanged OnSpeedLevelChanged;

    UPROPERTY(BlueprintAssignable, Category = "OX Game State")
    FOnPlayingCountdownChanged OnPlayingCountdownChanged;

    UPROPERTY(BlueprintAssignable, Category = "OX Game State")
    FOnDifficultyChanged OnDifficultyChanged;

    // 어디서든 GameState를 쉽게 가져오기 위한 유틸리티 함수
    UFUNCTION(BlueprintPure, Category = "Game Helper", meta = (WorldContext = "WorldContextObject"))
    static AOXQuizGameState* GetOXGameState(const UObject* WorldContextObject);

protected:
    // 내부 복제용 변수 (RepNotify 포함)
    UPROPERTY(ReplicatedUsing = OnRep_CurrentSpeedLevel)
    int32 CurrentSpeedLevel;

    UPROPERTY(ReplicatedUsing = OnRep_PlayingCountdown)
    int32 PlayingCountdown;

    // 서버로부터 값이 복제되었을 때 호출되는 응답 함수들
    UFUNCTION()
    void OnRep_GamePhase();

    UFUNCTION()
    void OnRep_CurrentSpeedLevel();

    UFUNCTION()
    void OnRep_PlayingCountdown();

    UFUNCTION()
    void OnRep_CurrentDifficulty();
};