#include "FruitGame/FruitGameState.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "FruitGame/FruitPlayerController.h"
#include "Kismet/GameplayStatics.h"

AFruitGameState::AFruitGameState()
{
	CurrentGamePhase = EFruitGamePhase::GP_WaitingToStart;
	CurrentActivePlayer = nullptr;
	ServerTimeAtTurnStart = 0.0f;
	Winner = nullptr;
	WinningCharacterType = ECharacterType::ECT_None;
}

void AFruitGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFruitGameState, CurrentGamePhase);
	DOREPLIFETIME(AFruitGameState, CurrentActivePlayer);
	DOREPLIFETIME(AFruitGameState, ServerTimeAtTurnStart);
	DOREPLIFETIME(AFruitGameState, Winner);
	DOREPLIFETIME(AFruitGameState, WinningCharacterType);
}

// [New] 멀티캐스트 함수 구현
void AFruitGameState::Multicast_AnnounceWinner_Implementation(const FString& WinnerName)
{
	// UI 위젯이 듣고 있을 델리게이트 실행 -> 화면 중앙 텍스트 출력
	OnWinnerAnnouncement.Broadcast(WinnerName);
}

void AFruitGameState::OnRep_GamePhase()
{
	OnGamePhaseChanged.Broadcast(CurrentGamePhase);

	if (CurrentGamePhase == EFruitGamePhase::GP_GameOver)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnRep_GamePhase: Detected GP_GameOver on Client."));

		AFruitPlayerController* PC = Cast<AFruitPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
		if (PC)
		{
			bool bAmIWinner = false;
			if (Winner && PC->PlayerState == Winner)
			{
				bAmIWinner = true;
			}

			UE_LOG(LogTemp, Warning, TEXT("OnRep_GamePhase: Calling Event_SetupResultsScreen..."));
			PC->Event_SetupResultsScreen();
			PC->Event_ShowResultsScreen(WinningCharacterType, bAmIWinner);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OnRep_GamePhase: FAILED to cast to AFruitPlayerController!"));
		}
	}
}

void AFruitGameState::OnRep_CurrentActivePlayer()
{
	// (기존 로직 유지)
	APlayerState* LocalPlayerState = nullptr;
	if (GetWorld() && GetWorld()->GetFirstPlayerController())
	{
		LocalPlayerState = GetWorld()->GetFirstPlayerController()->PlayerState;
	}

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