// FruitGameMode.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FruitGame/FruitGameTypes.h"
#include "FruitGameMode.generated.h"

// --- 전방 선언 ---
class AFruitGameState;
class AFruitPlayerState;
class AFruitPlayerController;
class AActor;
class ACharacter;
class UAnimMontage; // ProcessPunchAnimation을 위해 필요

UCLASS()
class NOOBGAME_API AFruitGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFruitGameMode();

	// --- PlayerController가 호출하는 함수 ---
	void PlayerIsReady(AController* PlayerController);
	void PlayerSubmittedFruits(AController* PlayerController, const TArray<EFruitType>& SecretFruits);
	void ProcessPlayerGuess(AController* PlayerController, const TArray<EFruitType>& GuessedFruits);
	void PlayerRequestsStartTurn(AController* PlayerController);
	bool IsPlayerTurn(AController* PlayerController) const;
	void PlayerInteracted(AController* PlayerController, AActor* HitActor, EGamePhase CurrentPhase);

	/** (수정!) 펀치 '애니메이션'을 모든 클라이언트에 전파 */
	void ProcessPunchAnimation(ACharacter* PunchingCharacter, UAnimMontage* MontageToPlay);

	/** (기존) 펀치 '적중' 처리 */
	void ProcessPunch(APlayerController* PuncherController, ACharacter* HitCharacter);

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	// --- 게임 흐름 제어 함수 ---
	void CheckBothPlayersReady_Instructions();
	void CheckBothPlayersReady_Setup();
	void StartSpinnerPhase();
	void StartTurn();
	void EndTurn(bool bTimeOut);
	void EndGame(APlayerState* Winner);
	void OnTurnTimerExpired();
	void ProcessGuessFromWorldObjects(AController* PlayerController);

	/** 쓰러진 캐릭터를 일정 시간 후 일으키는 함수 */
	UFUNCTION()
	void RecoverCharacter(ACharacter* CharacterToRecover);

	/** 캐시된 GameState */
	UPROPERTY()
	AFruitGameState* MyGameState;

	/** 턴 타이머 핸들 (GP_PlayerTurn용) */
	FTimerHandle TurnTimerHandle;

	/** 턴당 제한 시간 (초) */
	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	float TurnDuration = 30.0f;

	/** Setup 준비 완료 인원 */
	int32 NumPlayersReady_Setup = 0;

	/** 서버에서 결정된 돌림판 결과 (0 또는 1) */
	UPROPERTY()
	int32 SpinnerResultIndex = -1;

	// --- 펀치 관련 변수 ---
	/** 밀치기 힘 강도 */
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float PunchPushForce = 50000.0f;

	/** 쓰러짐 지속 시간 */
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float KnockdownDuration = 4.0f;
};