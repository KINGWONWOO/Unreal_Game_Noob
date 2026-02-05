#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestroyObstacleVolume.generated.h"

class UBoxComponent;

UCLASS()
class NOOBGAME_API ADestroyObstacleVolume : public AActor
{
    GENERATED_BODY()

public:
    ADestroyObstacleVolume();

protected:
    // 장애물 감지를 위한 충돌 박스 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UBoxComponent> CollisionComponent;

    // 초기 이벤트 바인딩 로직
    virtual void BeginPlay() override;

    // 감지 구역에 들어온 장애물을 처리하는 함수
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);
};