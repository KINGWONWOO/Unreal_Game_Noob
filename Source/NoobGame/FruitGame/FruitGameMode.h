#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameTypes.h"
#include "GameFramework/Pawn.h"
#include "FruitGameMode.generated.h"

class AFruitGameState;
class AFruitPlayerState;
class AFruitPlayerController;
class AActor;
class ACharacter;
class UAnimMontage;

UCLASS()
class NOOBGAME_API AFruitGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFruitGameMode();

	// ──────────────────────────────────────────────────────────────────────────
	// Public Game Flow API
	// ──────────────────────────────────────────────────────────────────────────
	void PlayerIsReady(AController* PlayerController);
	void PlayerSubmittedFruits(AController* PlayerController, const TArray<EFruitType>& SecretFruits);
	void PlayerRequestsStartTurn(AController* PlayerController);

	bool IsPlayerTurn(AController* PlayerController) const;

	void PlayerInteracted(AController* PlayerController, AActor* HitActor, EFruitGamePhase CurrentPhase);
	void ProcessPlayerGuess(AController* PlayerController, const TArray<EFruitType>& GuessedFruits);

	/** [New] 승자 발표 시퀀스 시작 (3초 대기 후 EndGame) */
	void StartWinnerAnnouncement(APlayerState* Winner);

	UFUNCTION(BlueprintCallable, Category = "Game")
	void EndGame(APlayerState* Winner);

	// ──────────────────────────────────────────────────────────────────────────
	// Public Combat API
	// ──────────────────────────────────────────────────────────────────────────
	UFUNCTION(BlueprintCallable, Category = "Game")
	void ProcessPunch(APlayerController* PuncherController, ACharacter* HitCharacter);

	UFUNCTION(BlueprintCallable, Category = "Game")
	void ProcessPunchAnimation(ACharacter* PunchingCharacter, UAnimMontage* MontageToPlay);

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	// Internal Game Flow Logic
	void CheckBothPlayersReady_Instructions();
	void CheckBothPlayersReady_Setup();
	void StartSpinnerPhase();
	void StartTurn();
	void EndTurn(bool bTimeOut);
	void OnTurnTimerExpired();
	void ProcessGuessFromWorldObjects(AController* PlayerController);

	void OnGuessResultDelayExpired();

	UFUNCTION()
	void RecoverCharacter(ACharacter* CharacterToRecover);

	// ──────────────────────────────────────────────────────────────────────────
	// Configuration Properties
	// ──────────────────────────────────────────────────────────────────────────
	UPROPERTY(EditDefaultsOnly, Category = "PlayerPawn")
	TSubclassOf<APawn> HostPawnClass;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerPawn")
	TSubclassOf<APawn> ClientPawnClass;

	// -- Game Rules --
	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	float TurnDuration = 30.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	float GuessResultDisplayTime = 3.0f;

	/** [New] 승자 발표 텍스트 유지 시간 */
	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	float WinnerAnnouncementDuration = 3.0f;

	// -- Combat Settings --
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float PunchPushForce = 50000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float KnockdownDuration = 4.0f;

	// -- GameOver Settings --
	UPROPERTY(EditDefaultsOnly, Category = "Quiz|GameOver")
	FName WinnerSpawnTag = TEXT("Result_Spawn_Winner");

	UPROPERTY(EditDefaultsOnly, Category = "Quiz|GameOver")
	FName LoserSpawnTag = TEXT("Result_Spawn_Defeat");

	UPROPERTY(EditDefaultsOnly, Category = "Quiz|GameOver")
	FName EndingCameraTag = TEXT("EndingCamera");

	// ──────────────────────────────────────────────────────────────────────────
	// Runtime State
	// ──────────────────────────────────────────────────────────────────────────
	UPROPERTY()
	AFruitGameState* MyGameState;

	FTimerHandle TurnTimerHandle;
	FTimerHandle GuessResultTimerHandle;
	FTimerHandle EndGameDelayTimerHandle;

	TMap<TWeakObjectPtr<ACharacter>, FTimerHandle> KnockdownTimers;

	int32 NumPlayersReady_Setup = 0;

	UPROPERTY()
	int32 SpinnerResultIndex = -1;
};