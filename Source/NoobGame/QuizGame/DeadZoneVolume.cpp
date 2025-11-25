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
	if (!HasAuthority()) return;

	// 캐릭터 확인
	ANoobGameCharacter* VictimCharacter = Cast<ANoobGameCharacter>(OtherActor);
	if (VictimCharacter)
	{
		// GameMode가 캐싱되지 않았다면 다시 가져옴
		if (!CachedGameMode)
		{
			CachedGameMode = GetWorld() ? GetWorld()->GetAuthGameMode<ANoobGameModeBase>() : nullptr;
		}

		if (CachedGameMode)
		{
			// [로직 변경] HandlePlayerDeath가 삭제되었으므로, 여기서 승자를 판별해 넘겨줍니다.

			AController* VictimController = VictimCharacter->GetController();
			APlayerState* WinnerState = nullptr;

			// 전체 플레이어 중 '떨어진 사람'이 아닌 사람을 찾음
			for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
			{
				AController* OtherPC = It->Get();
				if (OtherPC && OtherPC != VictimController)
				{
					WinnerState = OtherPC->GetPlayerState<APlayerState>();
					break;
				}
			}

			// 승자 발표 시작 (승자가 없으면 무승부 처리됨)
			CachedGameMode->StartWinnerAnnouncement(WinnerState);

			// (선택 사항) 떨어진 캐릭터 파괴 또는 비활성화
			// VictimCharacter->Destroy(); 
		}
	}
}