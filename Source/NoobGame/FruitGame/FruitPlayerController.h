#pragma once

#include "CoreMinimal.h"
#include "NoobPlayerController.h" 
#include "FruitPlayerController.generated.h"

class AInteractableFruitObject;

// --- Fruit Game 전용 델리게이트 (UI 업데이트 통지용) ---
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGuessResultReceived, const TArray<EFruitType>&, Guess, int32, MatchCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOpponentGuessReceived, const TArray<EFruitType>&, Guess, int32, MatchCount);

UCLASS()
class NOOBGAME_API AFruitPlayerController : public ANoobPlayerController
{
    GENERATED_BODY()

public:
    // --- 1. Blueprint/UI에서 호출하는 로컬 인터페이스 ---
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

    // --- 2. 블루프린트 비주얼 이벤트 ---
    UFUNCTION(BlueprintImplementableEvent, Category = "Fruit Game|Animation")
    void PlaySpinnerAnimationEvent(int32 WinningPlayerIndex);

    // --- 3. Server RPC (클라이언트 -> 서버 요청) ---
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_PlayerReady();

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_SubmitSecretFruits(const TArray<EFruitType>& SecretFruits);

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_RequestInteract(AActor* HitActor);

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_RequestStartPlayerTurn();

    // --- 4. Client RPC (서버 -> 클라이언트 통지) ---
    UFUNCTION(Client, Reliable)
    void Client_StartTurn();

    UFUNCTION(Client, Reliable)
    void Client_ReceiveGuessResult(const TArray<EFruitType>& Guess, int32 MatchCount);

    UFUNCTION(Client, Reliable)
    void Client_OpponentGuessed(const TArray<EFruitType>& Guess, int32 MatchCount);

    UFUNCTION(Client, Reliable)
    void Client_PlaySpinnerAnimation(int32 WinningPlayerIndex);

    // --- 5. 델리게이트 인스턴스 ---
    UPROPERTY(BlueprintAssignable, Category = "Fruit Game")
    FOnTurnStarted OnTurnStarted;

    UPROPERTY(BlueprintAssignable, Category = "Fruit Game")
    FOnGuessResultReceived OnGuessResultReceived;

    UPROPERTY(BlueprintAssignable, Category = "Fruit Game")
    FOnOpponentGuessReceived OnOpponentGuessReceived;

private:
    TArray<EFruitType> MyLocalSecretAnswers;
};