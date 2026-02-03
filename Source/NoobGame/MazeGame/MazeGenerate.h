#pragma once

#include "CoreMinimal.h"
#include "Maze.h" 
#include "MazeGameState.h" 
#include "MazeGenerate.generated.h"

UCLASS()
class NOOBGAME_API AMazeGenerate : public AMaze
{
    GENERATED_BODY()

public:
    void UpdateMazeWithSeed(int32 NewSeed);
    void SyncRandomProps(const TArray<FMazePropData>& PropData);
    void SyncRandomLights(const TArray<FMazeLightData>& LightData); // [추가]
    virtual FVector2D GetMaxCellSize() const override;

protected:
    void SpawnGoalTrigger(const FVector& MazeWorldLocation, const FVector& MazeScale);
    void GenerateRandomPropData(TArray<FMazePropData>& OutData);
    void GenerateRandomLightData(TArray<FMazeLightData>& OutData); // [추가]

    UPROPERTY(EditAnywhere, Category = "Maze Settings")
    TSubclassOf<AActor> GoalActorClass;

    UPROPERTY() AActor* SpawnedGoalActor;

    UPROPERTY(EditAnywhere, Category = "Maze Settings|Random Spawn")
    TArray<UStaticMesh*> RandomPropMeshes;

    UPROPERTY(EditAnywhere, Category = "Maze Settings|Random Spawn", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float SpawnProbability = 0.05f;

    UPROPERTY() TArray<UStaticMeshComponent*> SpawnedMeshComponents;

    // --- 조명 설정 ---
    UPROPERTY(EditAnywhere, Category = "Maze Settings|Light Spawn")
    TArray<TSubclassOf<AActor>> RandomLightClasses; // 블루프린트 액터 리스트

    UPROPERTY(EditAnywhere, Category = "Maze Settings|Light Spawn", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float LightSpawnProbability = 0.03f;

    UPROPERTY(EditAnywhere, Category = "Maze Settings|Light Spawn")
    float LightHeightOffset = 300.0f;

    UPROPERTY() TArray<AActor*> SpawnedLights;

    UPROPERTY(EditAnywhere, Category = "Maze Settings|Light Spawn")
    FVector LightScale = FVector(1.0f, 1.0f, 1.0f);
};