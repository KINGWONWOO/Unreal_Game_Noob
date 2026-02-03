#include "DeadZoneVolume.h"
#include "Components/BoxComponent.h"
#include "NoobGameModeBase.h" // [변경] 부모 게임모드 헤더 포함
#include "NoobGame/NoobGameCharacter.h"
#include "GameFramework/PlayerState.h"

ADeadZoneVolume::ADeadZoneVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionComponent->SetGenerateOverlapEvents(true);
}

void ADeadZoneVolume::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ADeadZoneVolume::OnOverlapBegin);
		// [변경] 부모 클래스로 캐스팅하여 저장
		CachedGameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ANoobGameModeBase>() : nullptr;
	}
}

void ADeadZoneVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 1. 서버 권한 및 중복 실행 방지 체크
    if (!HasAuthority() || bHasTriggered) return;

    // 캐릭터 확인
    ANoobGameCharacter* VictimCharacter = Cast<ANoobGameCharacter>(OtherActor);
    if (VictimCharacter)
    {
        // 2. 캐릭터 확인 직후 즉시 플래그를 true로 설정 (중복 진입 방지)
        bHasTriggered = true;

        if (!CachedGameMode)
        {
            CachedGameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ANoobGameModeBase>() : nullptr;
        }

        if (CachedGameMode)
        {
            AController* VictimController = VictimCharacter->GetController();
            APlayerState* WinnerState = nullptr;

            for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
            {
                AController* OtherPC = It->Get();
                if (OtherPC && OtherPC != VictimController)
                {
                    WinnerState = OtherPC->GetPlayerState<APlayerState>();
                    break;
                }
            }

            // 승자 발표 시작
            CachedGameMode->StartWinnerAnnouncement(WinnerState);
        }
    }
}