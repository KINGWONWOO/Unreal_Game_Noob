#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MazeStartPoint.generated.h"

class UCapsuleComponent;
class UArrowComponent;

UCLASS()
class NOOBGAME_API AMazeStartPoint : public AActor
{
	GENERATED_BODY()

public:
	AMazeStartPoint();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UCapsuleComponent> CapsuleComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UArrowComponent> ArrowComp;
};