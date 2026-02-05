#include "MazeStartPoint.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"

// =============================================================
// 1. 초기 생성 및 컴포넌트 구성 (Constructor)
// =============================================================

AMazeStartPoint::AMazeStartPoint()
{
    PrimaryActorTick.bCanEverTick = false;

    // 캡슐 컴포넌트 생성 및 루트 설정
    CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
    CapsuleComp->InitCapsuleSize(40.0f, 90.0f);
    CapsuleComp->SetCollisionProfileName(TEXT("NoCollision"));
    RootComponent = CapsuleComp;

    // 방향 표시용 화살표 설정
    ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
    ArrowComp->SetupAttachment(RootComponent);
    ArrowComp->ArrowColor = FColor::Red;
    ArrowComp->ArrowSize = 1.0f;
}