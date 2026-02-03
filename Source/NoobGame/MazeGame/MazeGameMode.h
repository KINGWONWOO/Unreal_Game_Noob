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

	/** 플레이어 준비 상태 확인 (설명 단계) */
	void PlayerIsReady(AController* PlayerController);

	/** 플레이어 도착 처리 */
	void ProcessPlayerReachedGoal(AController* WinnerController);

	virtual void EndGame(APlayerState* Winner) override;

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual bool IsGameInProgress() const override;
	virtual void AnnounceWinnerToClients(APlayerState* Winner) override;

	/** 2명 준비 완료 시 맵 선택 단계로 전환 */
	void CheckBothPlayersReady();

	/** 실제 미로 레벨 로드 시 게임 시작 처리 */
	void StartPlayingPhase();

	void UpdatePlayingCountdown();
	void EnablePlayerMovement();

	UPROPERTY()
	AMazeGameState* MyGameState;

	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	int32 PlayingStartCountdownDuration = 3;

	int32 RemainingPlayingCountdown = 0;
	FTimerHandle TimerHandle_GamePhase;
};