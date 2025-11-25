#include "MazeGameState.h"
#include "Net/UnrealNetwork.h"

AMazeGameState::AMazeGameState()
{
	CurrentGamePhase = EMazeGamePhase::GP_WaitingToStart;
	PlayingCountdown = 0;
}

void AMazeGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMazeGameState, CurrentGamePhase);
	DOREPLIFETIME(AMazeGameState, PlayingCountdown); // [New] 등록
}

void AMazeGameState::SetPlayingCountdown(int32 TimeLeft)
{
	if (HasAuthority())
	{
		PlayingCountdown = TimeLeft;
		// 서버에서도 델리게이트 호출 (UI 업데이트 등을 위함)
		OnPlayingCountdownChanged.Broadcast(PlayingCountdown);
	}
}

void AMazeGameState::OnRep_GamePhase()
{
	OnGamePhaseChanged.Broadcast(CurrentGamePhase);
}

void AMazeGameState::OnRep_PlayingCountdown()
{
	// 클라이언트에서 변수 업데이트 시 델리게이트 실행
	OnPlayingCountdownChanged.Broadcast(PlayingCountdown);
}