#pragma once

#include "CoreMinimal.h"
#include "NoobPlayerController.h"
#include "MazePlayerController.generated.h"

UCLASS()
class NOOBGAME_API AMazePlayerController : public ANoobPlayerController
{
	GENERATED_BODY()

public:
	// UI에서 호출하는 레디 함수
	UFUNCTION(BlueprintCallable, Category = "Maze Game")
	void PlayerReady();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PlayerReady();
};