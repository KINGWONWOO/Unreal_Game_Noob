#include "OXQuizGameState.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "OXQuizPlayerController.h"
#include "Kismet/GameplayStatics.h"

AOXQuizGameState::AOXQuizGameState()
{
	CurrentGamePhase = EQuizGamePhase::GP_WaitingToStart;
	Winner = nullptr;
	WinningCharacterType = ECharacterType::ECT_None;
	CurrentSpeedLevel = 0;
	PlayingCountdown = -1;
	// [New] 초기 난이도 설정
	CurrentDifficulty = EQuizDifficulty::Easy;
}

void AOXQuizGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOXQuizGameState, CurrentGamePhase);
	DOREPLIFETIME(AOXQuizGameState, Winner);
	DOREPLIFETIME(AOXQuizGameState, WinningCharacterType);
	DOREPLIFETIME(AOXQuizGameState, CurrentSpeedLevel);
	DOREPLIFETIME(AOXQuizGameState, PlayingCountdown);

	// [New] 난이도 변수 복제 등록
	DOREPLIFETIME(AOXQuizGameState, CurrentDifficulty);
}

void AOXQuizGameState::Multicast_AnnounceWinner_Implementation(const FString& WinnerName)
{
	OnWinnerAnnouncement.Broadcast(WinnerName);
}

void AOXQuizGameState::OnRep_GamePhase()
{
	OnGamePhaseChanged.Broadcast(CurrentGamePhase);

	if (CurrentGamePhase == EQuizGamePhase::GP_GameOver)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnRep_GamePhase: Detected GP_GameOver on Client."));

		AOXQuizPlayerController* PC = Cast<AOXQuizPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
		if (PC)
		{
			bool bAmIWinner = false;
			if (Winner && PC->PlayerState == Winner)
			{
				bAmIWinner = true;
			}
			PC->Event_SetupResultsScreen();
			PC->Event_ShowResultsScreen(WinningCharacterType, bAmIWinner);
		}
	}
}

void AOXQuizGameState::SetCurrentSpeedLevel(int32 NewLevel)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (CurrentSpeedLevel != NewLevel)
		{
			CurrentSpeedLevel = NewLevel;
			OnRep_CurrentSpeedLevel();
		}
	}
}
void AOXQuizGameState::OnRep_CurrentSpeedLevel()
{
	OnSpeedLevelChanged.Broadcast(CurrentSpeedLevel);
}

void AOXQuizGameState::SetPlayingCountdown(int32 TimeLeft)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		PlayingCountdown = TimeLeft;
		OnRep_PlayingCountdown();
	}
}
void AOXQuizGameState::OnRep_PlayingCountdown()
{
	OnPlayingCountdownChanged.Broadcast(PlayingCountdown);
}

// [New] 난이도 설정 (GameMode에서 호출)
void AOXQuizGameState::SetRepDifficulty(EQuizDifficulty NewDifficulty)
{
	if (HasAuthority())
	{
		if (CurrentDifficulty != NewDifficulty)
		{
			CurrentDifficulty = NewDifficulty;
			// 서버에서도 델리게이트 호출을 위해 OnRep 수동 실행
			OnRep_CurrentDifficulty();
		}
	}
}

AOXQuizGameState* AOXQuizGameState::GetOXGameState(const UObject* WorldContextObject)
{
	// 1. GameState를 가져와서
	AGameStateBase* GS = UGameplayStatics::GetGameState(WorldContextObject);

	// 2. 내 타입으로 캐스팅해서 반환 (실패 시 nullptr 자동 반환)
	return Cast<AOXQuizGameState>(GS);
}

// [New] 난이도 변경 감지 시 UI에 방송
void AOXQuizGameState::OnRep_CurrentDifficulty()
{
	OnDifficultyChanged.Broadcast(CurrentDifficulty);
}