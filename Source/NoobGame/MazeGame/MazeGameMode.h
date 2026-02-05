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

    // 플레이어 접속 및 준비 관련 인터페이스
    virtual void PostLogin(APlayerController* NewPlayer) override;
    void PlayerIsReady(AController* PlayerController);

    // 게임 진행 및 승리 판정
    void ProcessPlayerReachedGoal(AController* WinnerController);
    virtual void EndGame(APlayerState* Winner) override;

protected:
    // 게임 상태 확인 및 네트워크 알림
    virtual bool IsGameInProgress() const override;
    virtual void AnnounceWinnerToClients(APlayerState* Winner) override;

    // 내부 게임 단계 제어 로직
    void CheckBothPlayersReady();
    void StartPlayingPhase();
    void UpdatePlayingCountdown();
    void EnablePlayerMovement();

    // 게임 관리 변수 및 타이머
    UPROPERTY()
    AMazeGameState* MyGameState;

    UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
    int32 PlayingStartCountdownDuration = 3;

    int32 RemainingPlayingCountdown = 0;
    FTimerHandle TimerHandle_GamePhase;
};