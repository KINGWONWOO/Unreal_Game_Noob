// OXQuizPlayerState.cpp
#include "OXQuizPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AOXQuizPlayerState::AOXQuizPlayerState()
{
    bIsReady_Instructions = false;
    PunchHitCount = 0;
    bIsKnockedDown = false;
    bIsNextPunchLeft = true; // 첫 펀치는 왼쪽
    bIsRoomOwner = false;
}

void AOXQuizPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AOXQuizPlayerState, bIsReady_Instructions);
    DOREPLIFETIME(AOXQuizPlayerState, PunchHitCount);
    DOREPLIFETIME(AOXQuizPlayerState, bIsKnockedDown);
    DOREPLIFETIME(AOXQuizPlayerState, bIsNextPunchLeft);
    DOREPLIFETIME(AOXQuizPlayerState, bIsRoomOwner);
}

// --- Ready ---
void AOXQuizPlayerState::SetInstructionReady_Server()
{
    if (HasAuthority())
    {
        bIsReady_Instructions = true;
        // OnRep은 클라이언트에서 자동 호출
    }
}

AOXQuizPlayerState* AOXQuizPlayerState::GetOXPlayerState(const UObject* WorldContextObject)
{
    // 1. 현재 월드의 로컬 플레이어 컨트롤러(인덱스 0)를 가져옵니다.
    // (UI나 위젯은 항상 로컬 플레이어 소유이므로 Index 0이 나 자신입니다)
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
    {
        // 2. 그 컨트롤러의 PlayerState를 내 타입으로 캐스팅해서 반환
        return Cast<AOXQuizPlayerState>(PC->PlayerState);
    }

    // 컨트롤러를 못 찾았거나 PlayerState가 아직 없으면 nullptr 반환
    return nullptr;
}