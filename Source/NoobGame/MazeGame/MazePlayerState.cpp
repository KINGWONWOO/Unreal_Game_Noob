#include "MazePlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

AMazePlayerState::AMazePlayerState()
{
    bIsRoomOwner = false; // 기본값은 거짓
}

void AMazePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // bIsRoomOwner 변수가 모든 클라이언트에게 동기화되도록 등록
    DOREPLIFETIME(AMazePlayerState, bIsRoomOwner);
}

AMazePlayerState* AMazePlayerState::GetMazePlayerState(const UObject* WorldContextObject)
{
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
    {
        return Cast<AMazePlayerState>(PC->PlayerState);
    }
    return nullptr;
}