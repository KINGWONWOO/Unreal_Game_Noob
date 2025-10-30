// FruitGameState.cpp

#include "FruitGame/FruitGameState.h"
#include "Net/UnrealNetwork.h"

AFruitGameState::AFruitGameState()
{
	// 기본값 초기화
	CurrentGamePhase = EGamePhase::GP_WaitingToStart;
	CurrentActivePlayer = nullptr;
	ServerTimeAtTurnStart = 0.0f; // (신규)
	Winner = nullptr;
}

void AFruitGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 변수들을 복제 등록
	DOREPLIFETIME(AFruitGameState, CurrentGamePhase);
	DOREPLIFETIME(AFruitGameState, CurrentActivePlayer);
	DOREPLIFETIME(AFruitGameState, ServerTimeAtTurnStart); // (신규)
	DOREPLIFETIME(AFruitGameState, Winner);
}

void AFruitGameState::OnRep_GamePhase()
{
	// 게임 단계가 변경되었음을 UI(블루프린트)에 알립니다.
	OnGamePhaseChanged.Broadcast(CurrentGamePhase);
}

void AFruitGameState::OnRep_CurrentActivePlayer()
{
	APlayerState* LocalPlayerState = nullptr; // 로컬 플레이어 상태를 저장할 포인터
	if (GetWorld() && GetWorld()->GetFirstPlayerController())
	{
		LocalPlayerState = GetWorld()->GetFirstPlayerController()->PlayerState; // 일반 포인터로 받음
		if (LocalPlayerState)
		{
			// (수정!) LocalPlayerState는 일반 포인터이므로 -> 사용, .Get() 없음
			UE_LOG(LogTemp, Warning, TEXT("[OnRep_CurrentActivePlayer] -- Client: %s --"), *LocalPlayerState->GetPlayerName());
		}
	}

	if (CurrentGamePhase == EGamePhase::GP_PlayerTurn && CurrentActivePlayer) // CurrentActivePlayer는 TObjectPtr
	{
		// (수정!) TObjectPtr에서 멤버 함수 호출 시 -> 사용
		// (수정!) UE_LOG의 %p에는 .Get() 사용
		UE_LOG(LogTemp, Warning, TEXT("   CurrentActivePlayer: %s (Addr: %p)"), *CurrentActivePlayer->GetPlayerName(), CurrentActivePlayer);

		int32 StartingPlayerIndex = INDEX_NONE;

		UE_LOG(LogTemp, Warning, TEXT("   PlayerArray Order:"));
		for (int32 i = 0; i < PlayerArray.Num(); ++i)
		{
			if (PlayerArray[i]) // PlayerArray[i]는 TObjectPtr
			{
				// (수정!) TObjectPtr에서 멤버 함수 호출 시 -> 사용
				// (수정!) UE_LOG의 %p에는 .Get() 사용
				UE_LOG(LogTemp, Warning, TEXT("     [%d]: %s (Addr: %p)"), i, *PlayerArray[i]->GetPlayerName(), PlayerArray[i].Get());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("     [%d]: nullptr"), i);
			}
		}

		// 인덱스 계산 (변경 없음 - TObjectPtr끼리 == 비교 가능)
		for (int32 i = 0; i < PlayerArray.Num(); ++i)
		{
			if (PlayerArray[i] == CurrentActivePlayer)
			{
				StartingPlayerIndex = i;
				break;
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("   Calculated StartingPlayerIndex: %d"), StartingPlayerIndex);

		if (StartingPlayerIndex != INDEX_NONE)
		{
			OnFirstTurnPlayerDetermined.Broadcast(StartingPlayerIndex);
			UE_LOG(LogTemp, Warning, TEXT("   Broadcasted Index: %d"), StartingPlayerIndex);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("   ERROR: CurrentActivePlayer not found in PlayerArray!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[OnRep_CurrentActivePlayer] CurrentGamePhase is not GP_PlayerTurn or CurrentActivePlayer is invalid."));
	}
}