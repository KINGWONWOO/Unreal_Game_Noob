#pragma once

#include "CoreMinimal.h"
#include "NoobPlayerController.h" // Parent
#include "FruitPlayerController.generated.h"

class AInteractableFruitObject;

// 과일 게임 전용 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGuessResultReceived, const TArray<EFruitType>&, Guess, int32, MatchCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOpponentGuessReceived, const TArray<EFruitType>&, Guess, int32, MatchCount);

UCLASS()
class NOOBGAME_API AFruitPlayerController : public ANoobPlayerController
{
	GENERATED_BODY()

public:
	// -- Fruit Game Specific API --
	UFUNCTION(BlueprintCallable, Category = "Fruit Game")
	void PlayerReady(); // (GameMode Phase 전환용)

	UFUNCTION(BlueprintCallable, Category = "Fruit Game")
	void SubmitSecretFruits(const TArray<EFruitType>& SecretFruits);

	UFUNCTION(BlueprintCallable, Category = "Fruit Game")
	void RequestInteract(AActor* HitActor);

	UFUNCTION(BlueprintCallable, Category = "Fruit Game")
	void RequestStartPlayerTurn();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fruit Game")
	const TArray<EFruitType>& GetMyLocalSecretAnswers() const;

	// -- Blueprint Events (Visuals) --
	UFUNCTION(BlueprintImplementableEvent, Category = "Fruit Game|Animation")
	void PlaySpinnerAnimationEvent(int32 WinningPlayerIndex);

	// -- Server RPCs --
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PlayerReady();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SubmitSecretFruits(const TArray<EFruitType>& SecretFruits);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestInteract(AActor* HitActor);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestStartPlayerTurn();

	// -- Client RPCs --
	UFUNCTION(Client, Reliable)
	void Client_StartTurn();

	UFUNCTION(Client, Reliable)
	void Client_ReceiveGuessResult(const TArray<EFruitType>& Guess, int32 MatchCount);

	UFUNCTION(Client, Reliable)
	void Client_OpponentGuessed(const TArray<EFruitType>& Guess, int32 MatchCount);

	UFUNCTION(Client, Reliable)
	void Client_PlaySpinnerAnimation(int32 WinningPlayerIndex);

	// -- Delegates --
	UPROPERTY(BlueprintAssignable, Category = "Fruit Game")
	FOnTurnStarted OnTurnStarted;

	UPROPERTY(BlueprintAssignable, Category = "Fruit Game")
	FOnGuessResultReceived OnGuessResultReceived;

	UPROPERTY(BlueprintAssignable, Category = "Fruit Game")
	FOnOpponentGuessReceived OnOpponentGuessReceived;

private:
	TArray<EFruitType> MyLocalSecretAnswers;
};