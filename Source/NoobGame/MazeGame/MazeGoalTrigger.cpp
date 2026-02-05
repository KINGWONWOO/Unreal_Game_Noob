#include "MazeGoalTrigger.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MazeGameMode.h"
#include "NoobGame/NoobGameCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

// =============================================================
// 1. 초기 생성 및 설정 (Constructor)
// =============================================================

AMazeGoalTrigger::AMazeGoalTrigger()
{
    PrimaryActorTick.bCanEverTick = false; // 매 프레임 연산 제거 (성능 최적화)

    // 트리거 박스 설정: 루트 컴포넌트로 지정
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));
    TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
    RootComponent = TriggerBox;

    // 외형 메쉬 설정
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);

    // 멀티플레이어 동기화 설정 (서버의 상태 변화를 클라이언트에 전달)
    bReplicates = true;
}

// =============================================================
// 2. 런타임 초기화 (Lifecycle)
// =============================================================

void AMazeGoalTrigger::BeginPlay()
{
    Super::BeginPlay();

    // 서버 권한이 있는 경우에만 충돌 이벤트를 감지하도록 설정
    if (HasAuthority())
    {
        TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMazeGoalTrigger::OnOverlapBegin);
        CachedGameMode = GetWorld()->GetAuthGameMode<AMazeGameMode>();
    }
}

// =============================================================
// 3. 충돌 감지 및 승리 판정 (Logic)
// =============================================================

void AMazeGoalTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 1. 유효성 검사: 서버 권한 확인 및 게임모드 존재 여부
    if (!HasAuthority() || !CachedGameMode) return;

    // 2. 중복 처리 방지: 첫 번째 플레이어만 인정
    if (bHasTriggered) return;

    // 3. 대상 확인: 충돌한 액터가 플레이어 캐릭터인지 확인
    if (ANoobGameCharacter* PlayerChar = Cast<ANoobGameCharacter>(OtherActor))
    {
        bHasTriggered = true; // 이후 중복 발생 차단

        // 4. 게임모드에 승리자 정보 전달 (게임 종료 로직 시작)
        CachedGameMode->ProcessPlayerReachedGoal(PlayerChar->GetController());
    }
}