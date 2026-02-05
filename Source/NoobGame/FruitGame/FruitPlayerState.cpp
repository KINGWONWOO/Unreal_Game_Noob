#include "FruitGame/FruitPlayerState.h"
#include "Net/UnrealNetwork.h"

// =============================================================
// 1. 초기화 (Initialization)
// =============================================================

AFruitPlayerState::AFruitPlayerState()
{
    // 부모 생성자(ANoobPlayerState)가 공통 변수는 이미 초기화함
    bHasSubmittedFruits = false;
}

// =============================================================
// 2. 네트워크 복제 규칙 정의 (Replication Setup)
// =============================================================

void AFruitPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    // 부모 변수(Score, PlayerName 등) 리플리케이션 포함
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Fruit 게임 전용 데이터 복제 등록
    DOREPLIFETIME(AFruitPlayerState, bHasSubmittedFruits);
    DOREPLIFETIME(AFruitPlayerState, SecretAnswers);
}

// =============================================================
// 3. 데이터 관리 로직 (Data Handling)
// =============================================================

void AFruitPlayerState::SetSecretAnswers_Server(const TArray<EFruitType>& SecretFruits)
{
    // 서버 권한이 있을 때만 데이터를 수정하도록 보장
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