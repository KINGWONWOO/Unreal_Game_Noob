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
	AFruitGameMode();

	bool IsPlayerTurn(AController* PlayerController) const;

	void PlayerIsReady(AController* PlayerController);
	void PlayerSubmittedFruits(AController* PlayerController, const TArray<EFruitType>& SecretFruits);
	void PlayerRequestsStartTurn(AController* PlayerController);
	void ProcessPlayerGuess(AController* PlayerController, const TArray<EFruitType>& GuessedFruits);
	void PlayerInteracted(AController* PlayerController, AActor* HitActor, EFruitGamePhase CurrentPhase);

	virtual void EndGame(APlayerState* Winner) override;

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual bool IsGameInProgress() const override;
	virtual void AnnounceWinnerToClients(APlayerState* Winner) override;

	void CheckBothPlayersReady_Instructions();
	void CheckBothPlayersReady_Setup();
	void StartSpinnerPhase();
	void StartTurn();
	void EndTurn(bool bTimeOut);
	void OnTurnTimerExpired();
	void ProcessGuessFromWorldObjects(AController* PlayerController);
	void OnGuessResultDelayExpired();

	UPROPERTY()
	AFruitGameState* MyGameState;

	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	float TurnDuration = 30.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	float GuessResultDisplayTime = 3.0f;

	FTimerHandle TurnTimerHandle;
	FTimerHandle GuessResultTimerHandle;

	int32 NumPlayersReady_Setup = 0;
	int32 SpinnerResultIndex = -1;
};