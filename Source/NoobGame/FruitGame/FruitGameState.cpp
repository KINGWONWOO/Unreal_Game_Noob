#include "FruitGame/FruitGameState.h"
#include "Net/UnrealNetwork.h"
#include "FruitGame/FruitPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"

// =============================================================
// 1. 생성자 및 초기화 (Initialization)
// =============================================================

AFruitGameState::AFruitGameState()
{
    CurrentGamePhase = EFruitGamePhase::GP_WaitingToStart;
    CurrentActivePlayer = nullptr;
    ServerTimeAtTurnStart = 0.0f;
}

// =============================================================
// 2. 네트워크 복제 규칙 정의 (Replication Setup)
// =============================================================

void AFruitGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    // 부모 클래스(ANoobGameStateBase)의 복제 변수(Winner 등) 처리
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Fruit 게임 전용 변수 복제 등록
    DOREPLIFETIME(AFruitGameState, CurrentGamePhase);
    DOREPLIFETIME(AFruitGameState, CurrentActivePlayer);
    DOREPLIFETIME(AFruitGameState, ServerTimeAtTurnStart);
}

// =============================================================
// 3. 클라이언트 동기화 이벤트 (RepNotify Logic)
// =============================================================

void AFruitGameState::OnRep_GamePhase()
{
    // 게임 단계 변경을 알림 (주로 UI 업데이트용)
    OnGamePhaseChanged.Broadcast(CurrentGamePhase);

    // 게임 종료 시 결과 화면 처리
    if (CurrentGamePhase == EFruitGamePhase::GP_GameOver)
    {
        // 부모 클래스로부터 Winner 정보를 받아 승리 여부 판단
        AFruitPlayerController* PC = Cast<AFruitPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
        if (PC)
        {
            bool bAmIWinner = (Winner && PC->PlayerState == Winner);
            PC->Event_SetupResultsScreen();
            PC->Event_ShowResultsScreen(WinningCharacterType, bAmIWinner);
        }
    }
}

void AFruitGameState::OnRep_CurrentActivePlayer()
{
    // 턴이 시작되었을 때 어떤 플레이어의 차례인지 판단하여 알림
    if (CurrentGamePhase == EFruitGamePhase::GP_PlayerTurn && CurrentActivePlayer)
    {
        int32 StartingPlayerIndex = INDEX_NONE;
        for (int32 i = 0; i < PlayerArray.Num(); ++i)
        {
            if (PlayerArray[i] == CurrentActivePlayer)
            {
                StartingPlayerIndex = i;
                break;
            }
        }

        if (StartingPlayerIndex != INDEX_NONE)
        {
            OnFirstTurnPlayerDetermined.Broadcast(StartingPlayerIndex);
        }
    }
}