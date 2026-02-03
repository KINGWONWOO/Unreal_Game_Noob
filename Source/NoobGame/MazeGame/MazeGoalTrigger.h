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
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult);

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UBoxComponent> TriggerBox;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UStaticMeshComponent> MeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    TObjectPtr<UNiagaraSystem> GoalEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    TObjectPtr<USoundBase> GoalSound;

    UPROPERTY()
    TObjectPtr<AMazeGameMode> CachedGameMode;

private:
    bool bHasTriggered = false;  // 중복 트리거 방지
};