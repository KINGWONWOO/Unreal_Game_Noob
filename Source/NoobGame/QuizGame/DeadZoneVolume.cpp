#include "DeadZoneVolume.h"
#include "Components/BoxComponent.h"
#include "NoobGameModeBase.h"
#include "NoobGame/NoobGameCharacter.h"
#include "GameFramework/PlayerState.h"

// =============================================================
// 1. 초기 생성 및 설정 (Constructor)
// =============================================================

ADeadZoneVolume::ADeadZoneVolume()
{
    // 최적화: 충돌 감지용이므로 매 프레임 업데이트가 필요 없음
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;

    // 오버랩 이벤트 설정
    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    CollisionComponent->SetGenerateOverlapEvents(true);
}

// =============================================================
// 2. 초기화 및 바인딩 (Lifecycle)
// =============================================================

void ADeadZoneVolume::BeginPlay()
{
    Super::BeginPlay();

    // 서버 권한 확인 후 충돌 이벤트 바인딩 및 게임모드 캐싱
    if (HasAuthority())
    {
        CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ADeadZoneVolume::OnOverlapBegin);
        CachedGameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ANoobGameModeBase>() : nullptr;
    }
}

// =============================================================
// 3. 승패 판정 로직 (Collision Logic)
// =============================================================

void ADeadZoneVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 서버가 아니거나 이미 결과가 나왔다면 무시
    if (!HasAuthority() || bHasTriggered) return;

    // 충돌한 대상이 캐릭터인지 확인
    ANoobGameCharacter* VictimCharacter = Cast<ANoobGameCharacter>(OtherActor);
    if (VictimCharacter)
    {
        // 중복 판정 방지를 위해 즉시 플래그 설정
        bHasTriggered = true;

        // 게임모드 캐싱 재검사 (BeginPlay에서 실패했을 경우 대비)
        if (!CachedGameMode)
        {
            CachedGameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ANoobGameModeBase>() : nullptr;
        }

        if (CachedGameMode)
        {
            AController* VictimController = VictimCharacter->GetController();
            APlayerState* WinnerState = nullptr;

            // 월드의 플레이어를 순회하며 탈락자가 아닌 다른 플레이어(승자) 탐색
            for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
            {
                AController* OtherPC = It->Get();
                if (OtherPC && OtherPC != VictimController)
                {
                    WinnerState = OtherPC->GetPlayerState<APlayerState>();
                    break;
                }
            }

            // 부모 게임모드의 공용 함수를 통해 승리자 발표
            CachedGameMode->StartWinnerAnnouncement(WinnerState);
        }
    }
}