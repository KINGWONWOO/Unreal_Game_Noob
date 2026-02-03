#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NoobGameStateBase.h"
#include "MazeGameState.generated.h"

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

    UPROPERTY(ReplicatedUsing = OnRep_GamePhase, BlueprintReadOnly, Category = "Maze")
    EMazeGamePhase CurrentGamePhase;

    UPROPERTY(ReplicatedUsing = OnRep_PlayingCountdown, BlueprintReadOnly, Category = "Maze")
    int32 PlayingCountdown;

    UPROPERTY(ReplicatedUsing = OnRep_MazeSeed, BlueprintReadOnly, Category = "Maze")
    int32 MazeSeed;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Maze")
    EMazeMapSize MapSize;

    UPROPERTY(BlueprintAssignable, Category = "Maze")
    FOnMazeGamePhaseChanged OnGamePhaseChanged;

    UPROPERTY(BlueprintAssignable, Category = "Maze")
    FOnMazePlayingCountdownChanged OnPlayingCountdownChanged;

    UPROPERTY(BlueprintAssignable, Category = "Maze")
    FOnMazeSeedChanged OnMazeSeedChanged;

    // 프롭 동기화
    UPROPERTY(ReplicatedUsing = OnRep_PropData, BlueprintReadOnly, Category = "Maze")
    TArray<FMazePropData> ReplicatedProps;
    UFUNCTION() void OnRep_PropData();
    void SetMazePropData(const TArray<FMazePropData>& NewData);

    // 조명 동기화
    UPROPERTY(ReplicatedUsing = OnRep_LightData, BlueprintReadOnly, Category = "Maze")
    TArray<FMazeLightData> ReplicatedLights;
    UFUNCTION() void OnRep_LightData();
    void SetMazeLightData(const TArray<FMazeLightData>& NewData);

    void SetPlayingCountdown(int32 TimeLeft);
    void SetMazeSeed(int32 NewSeed);

    UFUNCTION() void OnRep_GamePhase();
    UFUNCTION() void OnRep_PlayingCountdown();
    UFUNCTION() void OnRep_MazeSeed();
};