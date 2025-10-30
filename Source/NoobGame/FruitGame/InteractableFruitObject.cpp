// InteractableFruitObject.cpp

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
	// (수정!) 기본값을 Apple로 설정
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
	// 시작 시 기본값(Apple)에 맞는 머티리얼 적용
	OnRep_CurrentFruit();
}

/** (수정) 이제 EFruitType을 반환하고, None을 건너뛰고 순환합니다. */
EFruitType AInteractableFruitObject::CycleFruit()
{
	// 서버에서만 실행
	if (!HasAuthority())
	{
		return CurrentFruit;
	}

	// (수정!) None(0)을 제외하고 1(Apple)부터 4(Orange)까지 순환
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
		// (선택 사항) 만약 CurrentFruit가 None일 때 기본 머티리얼을 보여주고 싶다면,
		// else if (CurrentFruit == EFruitType::FT_None) { MeshComponent->SetMaterial(0, DefaultMaterial); }
	}
}