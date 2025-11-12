#include "FruitGame/InteractableFruitObject.h"
#include "Net/UnrealNetwork.h"

AInteractableFruitObject::AInteractableFruitObject()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	MeshComponent->SetCollisionObjectType(ECC_Visibility);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	GuessIndex = 0;
	CurrentFruit = EFruitType::FT_Apple;
}

void AInteractableFruitObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AInteractableFruitObject, CurrentFruit);
}

void AInteractableFruitObject::BeginPlay()
{
	Super::BeginPlay();
	// 기본 머티리얼 설정을 위한 1회 호출
	OnRep_CurrentFruit();
}

EFruitType AInteractableFruitObject::CycleFruit()
{
	// 서버에서만 실행
	if (!HasAuthority())
	{
		return CurrentFruit;
	}

	// None(0)을 제외하고 1(Apple)부터 4(Orange)까지 순환
	uint8 CurrentValue = static_cast<uint8>(CurrentFruit);

	// 현재 값이 0(None)이거나 4(Orange)보다 크면 1(Apple)로 리셋
	if (CurrentValue == 0 || CurrentValue >= 4)
	{
		CurrentValue = 1; // Apple
	}
	else // 1, 2, 3일 경우 다음 값으로 증가
	{
		CurrentValue++; // Apple -> Banana, Banana -> Cherry, Cherry -> Orange
	}

	CurrentFruit = static_cast<EFruitType>(CurrentValue);

	// 서버에서도 즉시 머티리얼 변경
	OnRep_CurrentFruit();

	return CurrentFruit;
}

void AInteractableFruitObject::OnRep_CurrentFruit()
{
	// 클라이언트에서 머티리얼 변경
	if (MeshComponent)
	{
		UMaterialInterface** FoundMaterial = FruitMaterials.Find(CurrentFruit);
		if (FoundMaterial)
		{
			MeshComponent->SetMaterial(0, *FoundMaterial);
		}
	}
}