// FruitGameMode.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FruitGame/FruitGameTypes.h"
#include "FruitGameMode.generated.h"

class AFruitGameState;
class AFruitPlayerState;
class AFruitPlayerController;
class AActor; // 전방 선언

UCLASS()
class NOOBGAME_API AFruitGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFruitGameMode();

	// --- PlayerController가 호출하는 함수 ---

	/** 1. Instructions 단계에서 준비 완료 */
	void PlayerIsReady(AController* PlayerController);

	/** 2. Setup 단계에서 (UI로부터) 정답 제출 */
	void PlayerSubmittedFruits(AController* PlayerController, const TArray<EFruitType>& SecretFruits);

	/** 3. 턴 진행 중 (월드로부터) 추측 제출 */
	void ProcessPlayerGuess(AController* PlayerController, const TArray<EFruitType>& GuessedFruits);

	/** 현재 턴인 플레이어가 맞는지 확인 */
	bool IsPlayerTurn(AController* PlayerController) const;

	/** (수정) PlayerController의 RPC가 호출하는 (GP_PlayerTurn 전용) 상호작용 핸들러 */
	void PlayerInteracted(AController* PlayerController, AActor* HitActor, EGamePhase CurrentPhase);

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	// --- 게임 흐름 제어 함수 ---
	void CheckBothPlayersReady_Instructions();
	void CheckBothPlayersReady_Setup();
	void StartCoinToss();
	void StartTurn();
	void EndTurn(bool bTimeOut);
	void EndGame(APlayerState* Winner);
	void OnTurnTimerExpired();

	/** 추측 턴에 월드 오브젝트로부터 추측 배열을 생성하고 제출합니다. */
	void ProcessGuessFromWorldObjects(AController* PlayerController);

	/** 캐시된 GameState */
	UPROPERTY()
	AFruitGameState* MyGameState;

	/** 턴 타이머 핸들 */
	FTimerHandle TurnTimerHandle;

	/** 턴당 제한 시간 (초) */
	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	float TurnDuration = 30.0f;

	int32 FirstPlayerIndex;

	/** Setup 준비 완료 인원 */
	int32 NumPlayersReady_Setup = 0;
};