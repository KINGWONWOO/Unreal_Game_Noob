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
    // 캡슐 컴포넌트
    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UCapsuleComponent> CapsuleComp;

    // 바라볼 방향을 화살표로 시각화해주는 컴포넌트
    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UArrowComponent> ArrowComp;
};