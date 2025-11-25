#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MazeGoalTrigger.generated.h"

class UBoxComponent;
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
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> TriggerBox;

	UPROPERTY()
	TObjectPtr<AMazeGameMode> CachedGameMode;
};