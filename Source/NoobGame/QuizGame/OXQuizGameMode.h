#pragma once

#include "CoreMinimal.h"
#include "NoobGameModeBase.h"
#include "GameTypes.h"
#include "OXQuizGameMode.generated.h"

class AOXQuizGameState;
class AQuizObstacleBase;
class UDataTable;

UCLASS()
class NOOBGAME_API AOXQuizGameMode : public ANoobGameModeBase
{
    GENERATED_BODY()

public:
    // 초기화 및 기본 설정
    AOXQuizGameMode();
    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual bool IsGameInProgress() const override;

    // 플레이어 상호작용 및 난이도 설정
    void PlayerIsReady(AController* PlayerController);
    void SetGameDifficulty(EQuizDifficulty NewDifficulty);

    // 게임 종료 및 정리 로직
    virtual void EndGame(APlayerState* Winner) override;

protected:
    // 게임 내부 페이즈 관리
    void CheckBothPlayersReady_Instruction();
    void UpdatePlayingCountdown();
    void StartQuizSpawning();

    // 퀴즈 데이터 처리 및 스폰
    void LoadQuizListByDifficulty();
    void SpawnNextQuizObstacle();

    // 동기화 및 클린업
    virtual void AnnounceWinnerToClients(APlayerState* Winner) override;
    virtual void CleanupLevelActors() override;

    // 멤버 변수: 상태 및 에셋 참조
    UPROPERTY()
    AOXQuizGameState* MyGameState;

    UPROPERTY(EditDefaultsOnly, Category = "Quiz")
    TObjectPtr<UDataTable> QuizDataTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quiz")
    EQuizDifficulty CurrentGameDifficulty = EQuizDifficulty::Easy;

    UPROPERTY(EditDefaultsOnly, Category = "Quiz")
    TSubclassOf<AQuizObstacleBase> QuizObstacleClass_2Choice;

    UPROPERTY(EditDefaultsOnly, Category = "Quiz")
    TSubclassOf<AQuizObstacleBase> QuizObstacleClass_3Choice;

    UPROPERTY(EditDefaultsOnly, Category = "Quiz", meta = (MakeEditWidget = true))
    FTransform ObstacleSpawnTransform;

    // 타이밍 및 난이도 밸런스 설정
    UPROPERTY(EditDefaultsOnly, Category = "Quiz|Timing")
    int32 PlayingStartCountdownDuration = 5;

    UPROPERTY(EditDefaultsOnly, Category = "Quiz|Timing")
    float TimeBetweenSpawns = 10.f;

    UPROPERTY(EditDefaultsOnly, Category = "Quiz|Speed")
    TArray<float> SpeedLevels;

    // 핸들 및 내부 데이터 리스트
    FTimerHandle TimerHandle_SpawnQuiz;
    FTimerHandle TimerHandle_GamePhase;
    TArray<FQuizData> RemainingQuizList;

    float CurrentMoveSpeed = 0.f;
    int32 SpawnedQuizCount = 0;
    int32 CurrentSpeedLevelIndex = 0;
    int32 RemainingPlayingCountdown = 0;
};