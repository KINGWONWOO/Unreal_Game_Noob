#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "Sound/SoundBase.h"
#include "MazeGoalTrigger.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class AMazeGameMode;

UCLASS()
class NOOBGAME_API AMazeGoalTrigger : public AActor
{
    GENERATED_BODY()

public:
    AMazeGoalTrigger();

protected:
    // 게임 시작 시 초기화 및 바인딩
    virtual void BeginPlay() override;

    // 플레이어 감지를 위한 충돌 처리 함수
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult);

    // 물리적 충돌 및 외형 컴포넌트
    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UBoxComponent> TriggerBox;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UStaticMeshComponent> MeshComponent;

    // 도착 시 발생할 시각/청각 효과 에셋
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    TObjectPtr<UNiagaraSystem> GoalEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    TObjectPtr<USoundBase> GoalSound;

    // 게임 로직 연동을 위한 게임모드 캐싱
    UPROPERTY()
    TObjectPtr<AMazeGameMode> CachedGameMode;

private:
    // 한 명만 골인 지점을 밟았을 때 처리되도록 보장하는 플래그
    bool bHasTriggered = false;
};