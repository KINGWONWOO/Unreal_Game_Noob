#include "OXQuizPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// =============================================================
// 1. 초기화 (Constructor)
// =============================================================

AOXQuizPlayerState::AOXQuizPlayerState()
{
    // 게임 시작 시 초기 상태 설정
    bIsReady_Instructions = false;
    PunchHitCount = 0;
    bIsKnockedDown = false;
    bIsNextPunchLeft = true; // 펀치 순서 초기화
    bIsRoomOwner = false;
}

// =============================================================
// 2. 네트워크 복제 규칙 정의 (Replication Setup)
// =============================================================

void AOXQuizPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    // 부모 클래스(ANoobPlayerState)의 복제 설정 포함
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // OX 퀴즈 관련 모든 상태 변수를 복제 목록에 등록
    DOREPLIFETIME(AOXQuizPlayerState, bIsReady_Instructions);
    DOREPLIFETIME(AOXQuizPlayerState, PunchHitCount);
    DOREPLIFETIME(AOXQuizPlayerState, bIsKnockedDown);
    DOREPLIFETIME(AOXQuizPlayerState, bIsNextPunchLeft);
    DOREPLIFETIME(AOXQuizPlayerState, bIsRoomOwner);
}

// =============================================================
// 3. 서버 측 상태 관리 (Server Logic)
// =============================================================

void AOXQuizPlayerState::SetInstructionReady_Server()
{
    // 서버 권한이 있을 때만 데이터를 수정하여 무결성 보장
    if (HasAuthority())
    {
        bIsReady_Instructions = true;
    }
}

// =============================================================
// 4. 블루프린트 편의 기능 (Static Helper)
// =============================================================

AOXQuizPlayerState* AOXQuizPlayerState::GetOXPlayerState(const UObject* WorldContextObject)
{
    // 현재 월드의 로컬 플레이어 컨트롤러를 찾아 그 안의 PlayerState를 반환
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
    {
        return Cast<AOXQuizPlayerState>(PC->PlayerState);
    }

    return nullptr;
}