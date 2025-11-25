#include "MazeGoalTrigger.h"
#include "Components/BoxComponent.h"
#include "MazeGameMode.h"
#include "NoobGame/NoobGameCharacter.h"

AMazeGoalTrigger::AMazeGoalTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = TriggerBox;
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

void AMazeGoalTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority() || !CachedGameMode) return;

	// 캐릭터가 닿았는지 확인
	if (ANoobGameCharacter* PlayerChar = Cast<ANoobGameCharacter>(OtherActor))
	{
		CachedGameMode->ProcessPlayerReachedGoal(PlayerChar->GetController());
	}
}