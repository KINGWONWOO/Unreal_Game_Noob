#include "OXQuizPlayerController.h"
#include "OXQuizGameMode.h"
#include "Kismet/GameplayStatics.h"

// =============================================================
// 1. 로컬 인터페이스 구현 (Local Interface)
// =============================================================

void AOXQuizPlayerController::PlayerReady()
{
    // 로컬 호출을 서버 RPC로 전환
    Server_PlayerReady();
}

// =============================================================
// 2. 서버 RPC 구현 (Server RPC Implementations)
// =============================================================

// 준비 완료 요청 검증
bool AOXQuizPlayerController::Server_PlayerReady_Validate()
{
    return true;
}

void AOXQuizPlayerController::Server_PlayerReady_Implementation()
{
    // 서버에 존재하는 GameMode를 가져와 준비 로직 실행
    if (AOXQuizGameMode* GM = GetWorld()->GetAuthGameMode<AOXQuizGameMode>())
    {
        GM->PlayerIsReady(this);
    }
}

// 난이도 설정 요청 구현
void AOXQuizPlayerController::Server_RequestSetDifficulty_Implementation(EQuizDifficulty NewDifficulty)
{
    // 서버 측 GameMode에 접근하여 난이도 설정 위임
    if (AOXQuizGameMode* GM = Cast<AOXQuizGameMode>(UGameplayStatics::GetGameMode(this)))
    {
        GM->SetGameDifficulty(NewDifficulty);
        UE_LOG(LogTemp, Warning, TEXT("[Server] Difficulty Changed to %d"), (int32)NewDifficulty);
    }
}