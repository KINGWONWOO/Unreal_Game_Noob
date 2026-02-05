#include "DestroyObstacleVolume.h"
#include "Components/BoxComponent.h"
#include "QuizObstacleBase.h" // 장애물 부모 클래스 참조

// =============================================================
// 1. 컴포넌트 설정 및 최적화 (Constructor)
// =============================================================

ADestroyObstacleVolume::ADestroyObstacleVolume()
{
    // 최적화: 트리거 역할만 하므로 매 프레임 업데이트가 필요 없음
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;

    // 모든 동적 오브젝트를 감지하도록 콜리전 프로필 설정
    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    CollisionComponent->SetGenerateOverlapEvents(true);
}

// =============================================================
// 2. 초기화 로직 (Lifecycle)
// =============================================================

void ADestroyObstacleVolume::BeginPlay()
{
    Super::BeginPlay();

    // 서버 권한이 있는 경우에만 물리 이벤트를 감지하여 동기화 보장
    if (HasAuthority())
    {
        CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ADestroyObstacleVolume::OnOverlapBegin);
    }
}

// =============================================================
// 3. 장애물 파괴 로직 (Interaction Logic)
// =============================================================

void ADestroyObstacleVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 보안 및 동기화 확인: 서버가 아니면 중단
    if (!HasAuthority()) return;

    // 감지된 액터가 파괴 대상인 퀴즈 장애물인지 확인 (Casting)
    AQuizObstacleBase* Obstacle = Cast<AQuizObstacleBase>(OtherActor);
    if (Obstacle)
    {
        // 장애물 파괴 처리
        // 서버에서 파괴되면 해당 정보가 자동으로 모든 클라이언트에게 리플리케이션되어 제거됨
        Obstacle->Destroy();
    }
}