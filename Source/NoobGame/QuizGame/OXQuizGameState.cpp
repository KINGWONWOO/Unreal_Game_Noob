#include "OXQuizGameState.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

AOXQuizGameState::AOXQuizGameState()
{
	CurrentGamePhase = EQuizGamePhase::GP_WaitingToStart;
	CurrentDifficulty = EQuizDifficulty::Easy;
	CurrentSpeedLevel = 0;
	PlayingCountdown = 0;
}

void AOXQuizGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); // 부모(Winner) 처리

	DOREPLIFETIME(AOXQuizGameState, CurrentGamePhase);
	DOREPLIFETIME(AOXQuizGameState, CurrentDifficulty);
	DOREPLIFETIME(AOXQuizGameState, CurrentSpeedLevel);
	DOREPLIFETIME(AOXQuizGameState, PlayingCountdown);
}

void AOXQuizGameState::SetCurrentSpeedLevel(int32 NewLevel)
{
	if (HasAuthority())
	{
		CurrentSpeedLevel = NewLevel;
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

AOXQuizGameState* AOXQuizGameState::GetOXGameState(const UObject* WorldContextObject)
{
	return Cast<AOXQuizGameState>(UGameplayStatics::GetGameState(WorldContextObject));
}