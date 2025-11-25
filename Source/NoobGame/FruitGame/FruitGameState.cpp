#include "FruitGame/FruitGameState.h"
#include "Net/UnrealNetwork.h"
#include "FruitGame/FruitPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"

AFruitGameState::AFruitGameState()
{
	CurrentGamePhase = EFruitGamePhase::GP_WaitingToStart;
	CurrentActivePlayer = nullptr;
	ServerTimeAtTurnStart = 0.0f;
}

void AFruitGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); // 부모(Winner 등) 처리

	DOREPLIFETIME(AFruitGameState, CurrentGamePhase);
	DOREPLIFETIME(AFruitGameState, CurrentActivePlayer);
	DOREPLIFETIME(AFruitGameState, ServerTimeAtTurnStart);
}

void AFruitGameState::OnRep_GamePhase()
{
	OnGamePhaseChanged.Broadcast(CurrentGamePhase);

	if (CurrentGamePhase == EFruitGamePhase::GP_GameOver)
	{
		// [중요] Winner와 WinningCharacterType은 부모 클래스(ANoobGameStateBase)에 있음
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