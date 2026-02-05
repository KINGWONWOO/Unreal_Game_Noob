#include "NoobGameStateBase.h"
#include "Net/UnrealNetwork.h"

// =============================================================
// 1. 초기화 (Initialization)
// =============================================================

ANoobGameStateBase::ANoobGameStateBase()
{
    Winner = nullptr;
    WinningCharacterType = ECharacterType::ECT_None;
}

// =============================================================
// 2. 네트워크 복제 설정 (Replication)
// =============================================================

void ANoobGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // 승자 정보 및 승리 캐릭터 타입을 모든 클라이언트에게 동기화
    DOREPLIFETIME(ANoobGameStateBase, Winner);
    DOREPLIFETIME(ANoobGameStateBase, WinningCharacterType);
}

// =============================================================
// 3. 네트워크 통신 구현 (Multicast RPC)
// =============================================================

void ANoobGameStateBase::Multicast_AnnounceWinner_Implementation(const FString& WinnerName)
{
    // 서버에서 호출되면 모든 클라이언트에서 델리게이트를 방송하여 UI를 갱신
    OnWinnerAnnouncement.Broadcast(WinnerName);
}