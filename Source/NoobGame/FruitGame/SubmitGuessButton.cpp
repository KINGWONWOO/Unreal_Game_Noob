#include "SubmitGuessButton.h"

ASubmitGuessButton::ASubmitGuessButton()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true; // 맵에 배치된 액터이므로 복제 설정

	// 식별을 위한 루트 컴포넌트 및 콜리전 설정
	USceneComponent* DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = DummyRoot;

	UStaticMeshComponent* Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionObjectType(ECC_Visibility);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}