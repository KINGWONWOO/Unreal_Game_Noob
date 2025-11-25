#pragma once

#include "CoreMinimal.h"
#include "../NoobPlayerController.h" // Parent
#include "OXQuizPlayerController.generated.h"

UCLASS()
class NOOBGAME_API AOXQuizPlayerController : public ANoobPlayerController
{
	GENERATED_BODY()

public:
	// -- OX Quiz Specific API --
	UFUNCTION(BlueprintCallable, Category = "Quiz")
	void PlayerReady();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PlayerReady();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Game Logic")
	void Server_RequestSetDifficulty(EQuizDifficulty NewDifficulty);

protected:
	// (Optional) Quiz Specific UI references could go here
};