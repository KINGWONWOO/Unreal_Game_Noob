#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameTypes.h" // FQuizData, EQuizDifficulty 포함 필수
#include "GameFramework/Pawn.h"
#include "OXQuizGameMode.generated.h"

// (전방 선언 클래스들은 그대로 유지)
class AOXQuizGameState;
class AOXQuizPlayerState;
class AOXQuizPlayerController;
class AQuizObstacleBase;
class UDataTable;
class ANoobGameCharacter;
class AActor;
class ACharacter;
class UAnimMontage;

UCLASS()
class NOOBGAME_API AOXQuizGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOXQuizGameMode();

	// ... (기존 Public 함수들 유지: PlayerIsReady, HandlePlayerDeath 등) ...
	void PlayerIsReady(AController* PlayerController);
	void HandlePlayerDeath(AController* PlayerController);
	void StartWinnerAnnouncement(APlayerState* Winner);

	UFUNCTION(BlueprintCallable, Category = "Game")
	void EndGame(APlayerState* Winner);

	UFUNCTION(BlueprintCallable, Category = "Game")
	void ProcessPunch(APlayerController* PuncherController, ACharacter* HitCharacter);

	UFUNCTION(BlueprintCallable, Category = "Game")
	void ProcessPunchAnimation(ACharacter* PunchingCharacter, UAnimMontage* MontageToPlay);

	/** [New] 게임 시작 전, 외부(UI/GameInstance)에서 난이도를 설정하는 함수 */
	UFUNCTION(BlueprintCallable, Category = "Quiz|Settings")
	void SetGameDifficulty(EQuizDifficulty NewDifficulty);

protected:
	// ... (기존 Framework Overrides, Internal Logic 유지) ...
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	void CheckBothPlayersReady_Instruction();

	UFUNCTION()
	void UpdatePlayingCountdown();

	UFUNCTION()
	void StartQuizSpawning();

	UFUNCTION()
	void SpawnNextQuizObstacle();

	bool GetRandomQuiz(FQuizData& OutQuiz);

	// ──────────────────────────────────────────────────────────────────────────
	// [New] 난이도 관련 내부 로직
	// ──────────────────────────────────────────────────────────────────────────
	/** 현재 설정된 난이도에 맞는 퀴즈만 RemainingQuizList에 채워넣는 함수 */
	void LoadQuizListByDifficulty();

	UFUNCTION()
	void RecoverCharacter(ACharacter* CharacterToRecover);

	// ... (Configuration Properties 유지) ...
	UPROPERTY(EditDefaultsOnly, Category = "PlayerPawn")
	TSubclassOf<APawn> HostPawnClass;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerPawn")
	TSubclassOf<APawn> ClientPawnClass;

	// -- Quiz Settings --
	UPROPERTY(EditDefaultsOnly, Category = "Quiz")
	TObjectPtr<UDataTable> QuizDataTable;

	// [New] 현재 게임의 난이도 (기본값: Easy)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quiz")
	EQuizDifficulty CurrentGameDifficulty = EQuizDifficulty::Easy;

	UPROPERTY(EditDefaultsOnly, Category = "Quiz")
	TSubclassOf<AQuizObstacleBase> QuizObstacleClass_2Choice;

	UPROPERTY(EditDefaultsOnly, Category = "Quiz")
	TSubclassOf<AQuizObstacleBase> QuizObstacleClass_3Choice;

	UPROPERTY(EditDefaultsOnly, Category = "Quiz", meta = (MakeEditWidget = true))
	FTransform ObstacleSpawnTransform;

	// ... (나머지 Timing, Combat, GameOver 설정 및 변수들 그대로 유지) ...
	UPROPERTY(EditDefaultsOnly, Category = "Quiz|Timing")
	int32 PlayingStartCountdownDuration = 5;
	UPROPERTY(EditDefaultsOnly, Category = "Quiz|Timing")
	float TimeBetweenSpawns = 10.f;
	UPROPERTY(EditDefaultsOnly, Category = "Quiz|Timing")
	float WinnerAnnouncementDuration = 3.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Quiz|Speed")
	TArray<float> SpeedLevels;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float PunchPushForce = 300.f;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float KnockdownDuration = 4.f;

	UPROPERTY(EditDefaultsOnly, Category = "Quiz|GameOver")
	FName WinnerSpawnTag = TEXT("Result_Spawn_Winner");
	UPROPERTY(EditDefaultsOnly, Category = "Quiz|GameOver")
	FName LoserSpawnTag = TEXT("Result_Spawn_Defeat");
	UPROPERTY(EditDefaultsOnly, Category = "Quiz|GameOver")
	FName EndingCameraTag = TEXT("EndingCamera");

	UPROPERTY()
	AOXQuizGameState* MyGameState;

	FTimerHandle EndGameDelayTimerHandle;
	FTimerHandle TimerHandle_GamePhase;
	FTimerHandle TimerHandle_SpawnQuiz;

	UPROPERTY()
	TMap<TWeakObjectPtr<ACharacter>, FTimerHandle> KnockdownTimers;

	UPROPERTY()
	TArray<FQuizData> RemainingQuizList;

	UPROPERTY()
	float CurrentMoveSpeed = 0.f;
	UPROPERTY()
	int32 SpawnedQuizCount = 0;
	UPROPERTY()
	int32 CurrentSpeedLevelIndex = 0;
	UPROPERTY()
	int32 RemainingPlayingCountdown = 0;
};