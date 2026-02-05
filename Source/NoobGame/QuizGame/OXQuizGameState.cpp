#include "OXQuizGameState.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

// =============================================================
// 1. 초기화 및 네트워크 설정 (Initialization)
// =============================================================

AOXQuizGameState::AOXQuizGameState()
{
    CurrentGamePhase = EQuizGamePhase::GP_WaitingToStart;
    CurrentDifficulty = EQuizDifficulty::Easy;
    CurrentSpeedLevel = 0;
    PlayingCountdown = 0;
}

void AOXQuizGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    // 부모 클래스(ANoobGameStateBase)의 복제 변수 처리
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // OX 퀴즈 전용 변수 복제 등록
    DOREPLIFETIME(AOXQuizGameState, CurrentGamePhase);
    DOREPLIFETIME(AOXQuizGameState, CurrentDifficulty);
    DOREPLIFETIME(AOXQuizGameState, CurrentSpeedLevel);
    DOREPLIFETIME(AOXQuizGameState, PlayingCountdown);
}

// =============================================================
// 2. 서버 측 데이터 업데이트 로직 (Server Setters)
// =============================================================

void AOXQuizGameState::SetCurrentSpeedLevel(int32 NewLevel)
{
    if (HasAuthority())
    {
        CurrentSpeedLevel = NewLevel;
        // 서버에서도 즉각적으로 델리게이트 호출
        OnSpeedLevelChanged.Broadcast(CurrentSpeedLevel);
    }
}

void AOXQuizGameState::SetPlayingCountdown(int32 TimeLeft)
{
    if (HasAuthority())
    {
        PlayingCountdown = TimeLeft;
        OnPlayingCountdownChanged.Broadcast(PlayingCountdown);
    }
}

void AOXQuizGameState::SetRepDifficulty(EQuizDifficulty NewDifficulty)
{
    if (HasAuthority())
    {
        CurrentDifficulty = NewDifficulty;
        OnDifficultyChanged.Broadcast(CurrentDifficulty);
    }
}

// =============================================================
// 3. 클라이언트 측 동기화 이벤트 (RepNotify Handlers)
// =============================================================

void AOXQuizGameState::OnRep_GamePhase()
{
    OnGamePhaseChanged.Broadcast(CurrentGamePhase);
}

void AOXQuizGameState::OnRep_CurrentSpeedLevel()
{
    OnSpeedLevelChanged.Broadcast(CurrentSpeedLevel);
}

void AOXQuizGameState::OnRep_PlayingCountdown()
{
    OnPlayingCountdownChanged.Broadcast(PlayingCountdown);
}

void AOXQuizGameState::OnRep_CurrentDifficulty()
{
    OnDifficultyChanged.Broadcast(CurrentDifficulty);
}

// =============================================================
// 4. 유틸리티 및 헬퍼 (Static Helper)
// =============================================================

AOXQuizGameState* AOXQuizGameState::GetOXGameState(const UObject* WorldContextObject)
{
    return Cast<AOXQuizGameState>(UGameplayStatics::GetGameState(WorldContextObject));
}