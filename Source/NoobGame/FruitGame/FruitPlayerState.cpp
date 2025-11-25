#include "FruitGame/FruitPlayerState.h"
#include "Net/UnrealNetwork.h"

AFruitPlayerState::AFruitPlayerState()
{
	// 부모 생성자(ANoobPlayerState)가 공통 변수는 이미 초기화함
	bHasSubmittedFruits = false;
}

void AFruitPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); // 부모 변수 리플리케이션 포함

	// Fruit 고유 변수만 추가 등록
	DOREPLIFETIME(AFruitPlayerState, bHasSubmittedFruits);
	DOREPLIFETIME(AFruitPlayerState, SecretAnswers);
}

void AFruitPlayerState::SetSecretAnswers_Server(const TArray<EFruitType>& SecretFruits)
{
	if (HasAuthority())
	{
		SecretAnswers = SecretFruits;
		bHasSubmittedFruits = true;
	}
}

const TArray<EFruitType>& AFruitPlayerState::GetSecretAnswers_Server() const
{
	return SecretAnswers;
}