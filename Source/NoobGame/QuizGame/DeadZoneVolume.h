#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeadZoneVolume.generated.h"

class UBoxComponent;
class ANoobGameModeBase; // [변경] 부모 게임모드 전방 선언

UCLASS()
class NOOBGAME_API ADeadZoneVolume : public AActor
{
	GENERATED_BODY()

public:
	ADeadZoneVolume();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> CollisionComponent;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// [변경] 특정 게임모드 대신 부모 게임모드를 캐싱하여 재사용성 높임
	UPROPERTY()
	TObjectPtr<ANoobGameModeBase> CachedGameMode;
};