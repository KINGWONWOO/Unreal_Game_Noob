#pragma once

#include "CoreMinimal.h"
#include "NoobGameStateBase.h"
#include "MazeGameState.generated.h"

// 미로 게임 전용 이벤트 알림 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMazeGamePhaseChanged, EMazeGamePhase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMazePlayingCountdownChanged, int32, TimeLeft);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMazeSeedChanged, int32, NewSeed);

UCLASS()
class NOOBGAME_API AMazeGameState : public ANoobGameStateBase
{
    GENERATED_BODY()

public:
    AMazeGameState();
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // 복제 대상 변수 및 RepNotify
    UPROPERTY(ReplicatedUsing = OnRep_GamePhase, BlueprintReadOnly, Category = "Maze")
    EMazeGamePhase CurrentGamePhase;

    UPROPERTY(ReplicatedUsing = OnRep_PlayingCountdown, BlueprintReadOnly, Category = "Maze")
    int32 PlayingCountdown;

    UPROPERTY(ReplicatedUsing = OnRep_MazeSeed, BlueprintReadOnly, Category = "Maze")
    int32 MazeSeed;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Maze")
    EMazeMapSize MapSize;

    // 클라이언트 UI 바인딩용 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "Maze")
    FOnMazeGamePhaseChanged OnGamePhaseChanged;

    UPROPERTY(BlueprintAssignable, Category = "Maze")
    FOnMazePlayingCountdownChanged OnPlayingCountdownChanged;

    UPROPERTY(BlueprintAssignable, Category = "Maze")
    FOnMazeSeedChanged OnMazeSeedChanged;

    // 미로 오브젝트(프롭/조명) 동기화 데이터
    UPROPERTY(ReplicatedUsing = OnRep_PropData, BlueprintReadOnly, Category = "Maze")
    TArray<FMazePropData> ReplicatedProps;

    UPROPERTY(ReplicatedUsing = OnRep_LightData, BlueprintReadOnly, Category = "Maze")
    TArray<FMazeLightData> ReplicatedLights;

    // 데이터 설정 함수 (서버 전용)
    void SetPlayingCountdown(int32 TimeLeft);
    void SetMazeSeed(int32 NewSeed);
    void SetMazePropData(const TArray<FMazePropData>& NewData);
    void SetMazeLightData(const TArray<FMazeLightData>& NewData);

protected:
    // 클라이언트 측 응답 로직
    UFUNCTION() void OnRep_GamePhase();
    UFUNCTION() void OnRep_PlayingCountdown();
    UFUNCTION() void OnRep_MazeSeed();
    UFUNCTION() void OnRep_PropData();
    UFUNCTION() void OnRep_LightData();
};