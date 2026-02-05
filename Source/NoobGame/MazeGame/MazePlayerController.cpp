#include "MazePlayerController.h"
#include "MazeGameMode.h"

// =============================================================
// 1. 로컬 입력 처리 (Local Interface)
// =============================================================

void AMazePlayerController::PlayerReady()
{
    // 서버 RPC 호출
    Server_PlayerReady();
}

// =============================================================
// 2. 서버 RPC 구현 (Server-Side Implementation)
// =============================================================

// 요청의 유효성을 검사 (현재는 항상 true 반환)
bool AMazePlayerController::Server_PlayerReady_Validate()
{
    return true;
}

void AMazePlayerController::Server_PlayerReady_Implementation()
{
    // 서버에만 존재하는 GameMode에 접근하여 플레이어 준비 로직 실행
    if (AMazeGameMode* GM = GetWorld()->GetAuthGameMode<AMazeGameMode>())
    {
        GM->PlayerIsReady(this);
    }
}