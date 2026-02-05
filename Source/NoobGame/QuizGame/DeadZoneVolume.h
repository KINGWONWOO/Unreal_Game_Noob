#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeadZoneVolume.generated.h"

class UBoxComponent;
class ANoobGameModeBase;

UCLASS()
class NOOBGAME_API ADeadZoneVolume : public AActor
{
    GENERATED_BODY()

public:
    ADeadZoneVolume();

protected:
    // 물리적 감지 구역 설정을 위한 박스 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UBoxComponent> CollisionComponent;

    // 초기 바인딩 및 캐싱 로직
    virtual void BeginPlay() override;

    // 충돌 감지 시 호출되는 서버 사이드 로직
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    // 공통 게임모드 캐싱 (매번 찾지 않고 재사용하기 위함)
    UPROPERTY()
    TObjectPtr<ANoobGameModeBase> CachedGameMode;

    // 단 한 명의 승자만 결정되도록 보장하는 플래그
    bool bHasTriggered = false;
};