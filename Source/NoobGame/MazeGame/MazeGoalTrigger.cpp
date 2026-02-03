#include "MazeGoalTrigger.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MazeGameMode.h"
#include "NoobGame/NoobGameCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

AMazeGoalTrigger::AMazeGoalTrigger()
{
    PrimaryActorTick.bCanEverTick = false;

    // 루트 컴포넌트
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));
    TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
    RootComponent = TriggerBox;

    // 보이는 메쉬 (골인 지점 장식 등)
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);

    // 네트워크 복제 활성화 (Multicast가 제대로 동작하려면 필요)
    bReplicates = true;
}

void AMazeGoalTrigger::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMazeGoalTrigger::OnOverlapBegin);
        CachedGameMode = GetWorld()->GetAuthGameMode<AMazeGameMode>();
    }
}

void AMazeGoalTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!HasAuthority() || !CachedGameMode) return;

    // 이미 트리거된 경우 무시
    if (bHasTriggered) return;

    if (ANoobGameCharacter* PlayerChar = Cast<ANoobGameCharacter>(OtherActor))
    {
        bHasTriggered = true;

        // 기존 게임 로직 (절대 변경 금지)
        CachedGameMode->ProcessPlayerReachedGoal(PlayerChar->GetController());
    }
}
