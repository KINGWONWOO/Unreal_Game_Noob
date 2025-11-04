// FruitPlayerController.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FruitGame/FruitGameTypes.h"
#include "FruitPlayerController.generated.h"

// --- 전방 선언 ---
class AInteractableFruitObject;
class ACharacter;
class UAnimMontage;

// --- 델리게이트 선언 ---
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGuessResultReceived, const TArray<EFruitType>&, Guess, int32, MatchCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOpponentGuessReceived, const TArray<EFruitType>&, Guess, int32, MatchCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameOver, bool, bYouWon);


UCLASS()
class NOOBGAME_API AFruitPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	// 로컬 정답 저장용 변수
	TArray<EFruitType> MyLocalSecretAnswers;

public:
	// --- UI 및 캐릭터에서 호출할 함수들 (BlueprintCallable) ---
	UFUNCTION(BlueprintCallable, Category = "Fruit Game")
	void PlayerReady();
	UFUNCTION(BlueprintCallable, Category = "Fruit Game")
	void SubmitSecretFruits(const TArray<EFruitType>& SecretFruits);
	UFUNCTION(BlueprintCallable, Category = "Fruit Game")
	void RequestInteract(AActor* HitActor);
	UFUNCTION(BlueprintCallable, Category = "Fruit Game")
	void RequestStartPlayerTurn();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fruit Game")
	const TArray<EFruitType>& GetMyLocalSecretAnswers() const;
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RequestPunch(ACharacter* HitCharacter);
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RequestPlayPunchMontage();

	// --- 블루프린트 구현 이벤트 ---
	UFUNCTION(BlueprintImplementableEvent, Category = "Fruit Game|Animation")
	void PlaySpinnerAnimationEvent(int32 WinningPlayerIndex);
	UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
	void PlayHitReactionOnCharacter(ACharacter* TargetCharacter);
	UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
	void PlayPunchEvent(ACharacter* PunchingCharacter, bool bIsLeftPunch);

	// --- 서버 -> 클라이언트 RPC ---
	UFUNCTION(Client, Reliable)
	void Client_StartTurn();
	UFUNCTION(Client, Reliable)
	void Client_ReceiveGuessResult(const TArray<EFruitType>& Guess, int32 MatchCount);
	UFUNCTION(Client, Reliable)
	void Client_OpponentGuessed(const TArray<EFruitType>& Guess, int32 MatchCount);
	UFUNCTION(Client, Reliable)
	void Client_GameOver(bool bYouWon);
	UFUNCTION(Client, Reliable)
	void Client_PlaySpinnerAnimation(int32 WinningPlayerIndex);

	/** (수정!) NetMulticast -> Client RPC로 변경 */
	UFUNCTION(Client, Reliable)
	void Client_PlayPunchMontage(ACharacter* PunchingCharacter, bool bIsLeftPunch);

	/** (수정!) NetMulticast -> Client RPC로 변경 */
	UFUNCTION(Client, Reliable)
	void Client_PlayHitReaction(ACharacter* TargetCharacter);

	// --- UI 바인딩용 델리게이트 ---
	UPROPERTY(BlueprintAssignable, Category = "Fruit Game")
	FOnTurnStarted OnTurnStarted;
	UPROPERTY(BlueprintAssignable, Category = "Fruit Game")
	FOnGuessResultReceived OnGuessResultReceived;
	UPROPERTY(BlueprintAssignable, Category = "Fruit Game")
	FOnOpponentGuessReceived OnOpponentGuessReceived;
	UPROPERTY(BlueprintAssignable, Category = "Fruit Game")
	FOnGameOver OnGameOver;

protected:
	// (삭제!) 몽타주 변수들을 컨트롤러에서 삭제

	// --- 클라이언트 -> 서버 RPC ---
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PlayerReady();
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SubmitSecretFruits(const TArray<EFruitType>& SecretFruits);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestInteract(AActor* HitActor);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestStartPlayerTurn();
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestPunch(ACharacter* HitCharacter);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestPlayPunchMontage();

	// (삭제!) Multicast RPC 2개 선언 삭제
};