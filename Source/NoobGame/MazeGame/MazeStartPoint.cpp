#include "MazeStartPoint.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"

AMazeStartPoint::AMazeStartPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapsuleComp->InitCapsuleSize(40.0f, 90.0f);
	CapsuleComp->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = CapsuleComp;

	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	ArrowComp->SetupAttachment(RootComponent);
	ArrowComp->ArrowColor = FColor::Red;
	ArrowComp->ArrowSize = 1.0f;
}