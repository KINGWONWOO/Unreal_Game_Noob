// FruitPlayerController.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FruitGame/FruitGameTypes.h"
#include "FruitPlayerController.generated.h"

// UI 바인딩용 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGuessResultReceived, const TArray<EFruitType>&, Guess, int32, MatchCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOpponentGuessReceived, const TArray<EFruitType>&, Guess, int32, MatchCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameOver, bool, bYouWon);

// 전방 선언
class AInteractableFruitObject;

UCLASS()
class NOOBGAME_API AFruitPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	/** (신규) 클라이언트 본인만 아는, 본인의 정답 배열 (복제되지 않음!)
	 * 이 변수는 UPROPERTY()가 없으므로 C++ 전용이며, 네트워크로 전송되지 않습니다.
	 */
	TArray<EFruitType> MyLocalSecretAnswers;

public:
	// --- UI에서 호출할 함수들 (블루프린트에서 호출 가능) ---

	/** 1. (Instructions 단계) "준비 완료" 버튼 클릭 */
	UFUNCTION(BlueprintCallable, Category = "Fruit Game")
	void PlayerReady();

	/** 2. (Setup 단계) UI에서 정답 과일 5개 선택 완료 시 호출 */
	UFUNCTION(BlueprintCallable, Category = "Fruit Game")
	void SubmitSecretFruits(const TArray<EFruitType>& SecretFruits);

	/** 3. 캐릭터(Pawn)의 IA_Grab에서 호출할 함수 */
	UFUNCTION(BlueprintCallable, Category = "Fruit Game")
	void RequestInteract(AActor* HitActor);

	/** (신규) UI가 이 배열을 가져갈 수 있도록 BlueprintCallable 함수 추가 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fruit Game")
	const TArray<EFruitType>& GetMyLocalSecretAnswers() const;


	// --- 서버 -> 클라이언트 RPC (서버가 클라이언트에서 함수를 호출) ---
	UFUNCTION(Client, Reliable)
	void Client_StartTurn();

	UFUNCTION(Client, Reliable)
	void Client_ReceiveGuessResult(const TArray<EFruitType>& Guess, int32 MatchCount);

	UFUNCTION(Client, Reliable)
	void Client_OpponentGuessed(const TArray<EFruitType>& Guess, int32 MatchCount);

	UFUNCTION(Client, Reliable)
	void Client_GameOver(bool bYouWon);

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
	// --- 클라이언트 -> 서버 RPC (클라이언트가 서버에서 함수를 호출) ---

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PlayerReady();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SubmitSecretFruits(const TArray<EFruitType>& SecretFruits);

	/** (신규!) RequestInteract가 호출할 통합 RPC */
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestInteract(AActor* HitActor);
};