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
    // 미로 생성 및 데이터 동기화 인터페이스
    void UpdateMazeWithSeed(int32 NewSeed);
    void SyncRandomProps(const TArray<FMazePropData>& PropData);
    void SyncRandomLights(const TArray<FMazeLightData>& LightData);

    // 미로 셀 크기 계산 (부모 클래스 오버라이드)
    virtual FVector2D GetMaxCellSize() const override;

protected:
    // 서버 전용 데이터 생성 함수
    void GenerateRandomPropData(TArray<FMazePropData>& OutData);
    void GenerateRandomLightData(TArray<FMazeLightData>& OutData);

    // 골 지점 액터 스폰 로직
    void SpawnGoalTrigger(const FVector& MazeWorldLocation, const FVector& MazeScale);

    // 골인 지점 클래스 및 인스턴스 관리
    UPROPERTY(EditAnywhere, Category = "Maze Settings")
    TSubclassOf<AActor> GoalActorClass;

    UPROPERTY()
    AActor* SpawnedGoalActor;

    // 장애물(Prop) 설정 및 컴포넌트 관리
    UPROPERTY(EditAnywhere, Category = "Maze Settings|Random Spawn")
    TArray<UStaticMesh*> RandomPropMeshes;

    UPROPERTY(EditAnywhere, Category = "Maze Settings|Random Spawn", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float SpawnProbability = 0.05f;

    UPROPERTY()
    TArray<UStaticMeshComponent*> SpawnedMeshComponents;

    // 조명 관련 설정 및 관리 변수
    UPROPERTY(EditAnywhere, Category = "Maze Settings|Light Spawn")
    TArray<TSubclassOf<AActor>> RandomLightClasses;

    UPROPERTY(EditAnywhere, Category = "Maze Settings|Light Spawn", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float LightSpawnProbability = 0.03f;

    UPROPERTY(EditAnywhere, Category = "Maze Settings|Light Spawn")
    float LightHeightOffset = 300.0f;

    UPROPERTY(EditAnywhere, Category = "Maze Settings|Light Spawn")
    FVector LightScale = FVector(1.0f, 1.0f, 1.0f);

    UPROPERTY()
    TArray<AActor*> SpawnedLights;
};