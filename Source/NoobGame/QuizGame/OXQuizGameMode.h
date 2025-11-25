// 파일명: Source/NoobGame/OXQuizGameMode.h

#pragma once

#include "CoreMinimal.h"
#include "NoobGameModeBase.h" // [중요] 부모 헤더 포함
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
	AOXQuizGameMode();

	// [Fix] HandlePlayerDeath 삭제됨 (부모 클래스의 Logout -> HandlePlayerDisconnect가 자동 처리)

	void PlayerIsReady(AController* PlayerController);
	void SetGameDifficulty(EQuizDifficulty NewDifficulty);

	virtual void EndGame(APlayerState* Winner) override;

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual bool IsGameInProgress() const override;
	virtual void AnnounceWinnerToClients(APlayerState* Winner) override;
	virtual void CleanupLevelActors() override;

	void CheckBothPlayersReady_Instruction();
	void StartQuizSpawning();
	void SpawnNextQuizObstacle();
	void LoadQuizListByDifficulty();
	void UpdatePlayingCountdown();

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

	UPROPERTY(EditDefaultsOnly, Category = "Quiz|Timing")
	int32 PlayingStartCountdownDuration = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Quiz|Timing")
	float TimeBetweenSpawns = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Quiz|Speed")
	TArray<float> SpeedLevels;

	FTimerHandle TimerHandle_SpawnQuiz;
	FTimerHandle TimerHandle_GamePhase;
	TArray<FQuizData> RemainingQuizList;

	float CurrentMoveSpeed = 0.f;
	int32 SpawnedQuizCount = 0;
	int32 CurrentSpeedLevelIndex = 0;
	int32 RemainingPlayingCountdown = 0;
};