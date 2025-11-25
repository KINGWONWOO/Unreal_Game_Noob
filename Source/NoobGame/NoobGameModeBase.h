#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameTypes.h"
#include "NoobGameModeBase.generated.h"

class ACharacter;
class UAnimMontage;
class ANoobPlayerState;
class ANoobPlayerController;

UCLASS()
class NOOBGAME_API ANoobGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANoobGameModeBase();

	// ──────────────────────────────────────────────────────────────────────────
	// Public Shared API
	// ──────────────────────────────────────────────────────────────────────────
	virtual void StartWinnerAnnouncement(APlayerState* Winner);

	UFUNCTION(BlueprintCallable, Category = "Game")
	virtual void EndGame(APlayerState* Winner);

	UFUNCTION(BlueprintCallable, Category = "Game")
	virtual void ProcessPunch(APlayerController* PuncherController, ACharacter* HitCharacter);

	UFUNCTION(BlueprintCallable, Category = "Game")
	void ProcessPunchAnimation(ACharacter* PunchingCharacter, UAnimMontage* MontageToPlay);

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	// 방장이 배정되었는지 확인하는 플래그
	bool bHasAssignedRoomOwner = false;

	// Helpers
	virtual bool IsGameInProgress() const { return true; }
	virtual void AnnounceWinnerToClients(APlayerState* Winner) {}
	virtual void CleanupLevelActors() {}

	void HandlePlayerDisconnect(AController* ExitingPlayer);

	UFUNCTION()
	void RecoverCharacter(ACharacter* CharacterToRecover);

	// ──────────────────────────────────────────────────────────────────────────
	// Configuration
	// ──────────────────────────────────────────────────────────────────────────
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float PunchPushForce = 50000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float KnockdownDuration = 4.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	float WinnerAnnouncementDuration = 3.0f;

	// -- GameOver Settings --
	UPROPERTY(EditDefaultsOnly, Category = "GameOver")
	FName WinnerSpawnTag = TEXT("Result_Spawn_Winner");

	UPROPERTY(EditDefaultsOnly, Category = "GameOver")
	FName LoserSpawnTag = TEXT("Result_Spawn_Defeat");

	UPROPERTY(EditDefaultsOnly, Category = "GameOver")
	FName EndingCameraTag = TEXT("EndingCamera");

	// Runtime
	FTimerHandle EndGameDelayTimerHandle;

	UPROPERTY()
	TMap<TWeakObjectPtr<ACharacter>, FTimerHandle> KnockdownTimers;
};