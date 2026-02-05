#include "MazePlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

// =============================================================
// 1. 초기화 (Initialization)
// =============================================================

AMazePlayerState::AMazePlayerState()
{
    bIsRoomOwner = false;
}

// =============================================================
// 2. 네트워크 동기화 설정 (Replication)
// =============================================================

void AMazePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // bIsRoomOwner 변수를 서버에서 클라이언트로 동기화하도록 등록
    DOREPLIFETIME(AMazePlayerState, bIsRoomOwner);
}

// =============================================================
// 3. 유틸리티 및 헬퍼 로직 (Utilities)
// =============================================================

AMazePlayerState* AMazePlayerState::GetMazePlayerState(const UObject* WorldContextObject)
{
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
    {
        return Cast<AMazePlayerState>(PC->PlayerState);
    }
    return nullptr;
}