#pragma once

#include "CoreMinimal.h"
#include "NoobGameModeBase.h"
#include "FruitGameMode.generated.h"

class AFruitGameState;

UCLASS()
class NOOBGAME_API AFruitGameMode : public ANoobGameModeBase
{
    GENERATED_BODY()

public:
    // 초기화 및 기본 설정
    AFruitGameMode();
    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual bool IsGameInProgress() const override;

    // 게임 단계(Phase) 관리
    void PlayerIsReady(AController* PlayerController);                                      // 준비 완료 확인
    void PlayerSubmittedFruits(AController* PlayerController, const TArray<EFruitType>& SecretFruits); // 과일 설정
    void PlayerRequestsStartTurn(AController* PlayerController);                            // 턴 시작 요청

    // 상호작용 및 게임 로직
    void PlayerInteracted(AController* PlayerController, AActor* HitActor, EFruitGamePhase CurrentPhase);
    void ProcessPlayerGuess(AController* PlayerController, const TArray<EFruitType>& GuessedFruits);
    bool IsPlayerTurn(AController* PlayerController) const;

    // 게임 종료 처리
    virtual void EndGame(APlayerState* Winner) override;

protected:
    // 내부 단계 처리 함수
    void CheckBothPlayersReady_Instructions();
    void CheckBothPlayersReady_Setup();
    void StartSpinnerPhase();
    void StartTurn();
    void EndTurn(bool bTimeOut);

    // 타이머 및 데이터 처리
    void OnTurnTimerExpired();
    void ProcessGuessFromWorldObjects(AController* PlayerController);
    void OnGuessResultDelayExpired();
    virtual void AnnounceWinnerToClients(APlayerState* Winner) override;

    // 멤버 변수
    UPROPERTY()
    AFruitGameState* MyGameState;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Logic")
    bool bIsProcessingGuess;

    UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
    float TurnDuration = 30.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
    float GuessResultDisplayTime = 3.0f;

    FTimerHandle TurnTimerHandle;
    FTimerHandle GuessResultTimerHandle;

    int32 NumPlayersReady_Setup = 0;
    int32 SpinnerResultIndex = -1;
};