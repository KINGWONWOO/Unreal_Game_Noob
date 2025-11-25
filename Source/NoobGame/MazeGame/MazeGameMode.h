#pragma once

#include "CoreMinimal.h"
#include "NoobGameModeBase.h"
#include "MazeGameState.h"
#include "MazeGameMode.generated.h"

class AMazeGameState;

UCLASS()
class NOOBGAME_API AMazeGameMode : public ANoobGameModeBase
{
	GENERATED_BODY()

public:
	AMazeGameMode();

	void PlayerIsReady(AController* PlayerController);
	void ProcessPlayerReachedGoal(AController* WinnerController);
	virtual void EndGame(APlayerState* Winner) override;

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual bool IsGameInProgress() const override;
	virtual void AnnounceWinnerToClients(APlayerState* Winner) override;

	void CheckBothPlayersReady();

	// [New] 카운트다운 타이머 함수 (OXQuiz 방식)
	void UpdatePlayingCountdown();

	// [New] 실제 이동 잠금 해제 함수
	void EnablePlayerMovement();

	UPROPERTY()
	AMazeGameState* MyGameState;

	// [New] 카운트다운 설정
	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	int32 PlayingStartCountdownDuration = 5; // 3초

	int32 RemainingPlayingCountdown = 0;
	FTimerHandle TimerHandle_GamePhase;
};