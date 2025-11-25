#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameTypes.h"
#include "NoobPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;
class ACharacter;
class UAnimMontage;
class ACameraActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNoobGameOver, bool, bYouWon);

UCLASS()
class NOOBGAME_API ANoobPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RequestPunch(ACharacter* HitCharacter);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RequestPlayPunchMontage();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestPunch(ACharacter* HitCharacter);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestPlayPunchMontage();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetupEnding(bool bIsWinner, FVector TargetLocation, FRotator TargetRotation, ECharacterType WinnerType, ACameraActor* EndingCamera);

	UFUNCTION(Client, Reliable)
	void Client_SetCameraEffect(bool bEnableKnockdownEffect);

	UFUNCTION(Client, Reliable)
	void Client_SetUIOnlyInput(bool bYouWon, ECharacterType WinnerType, ACameraActor* EndingCamera);

	UFUNCTION(Client, Reliable)
	void Client_EnableMovementAfterEnding();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayHitReaction(ACharacter* TargetCharacter, UAnimMontage* MontageToPlay);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayPunchMontage(ACharacter* PunchingCharacter, UAnimMontage* MontageToPlay);

	UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
	void ApplyKnockdownCameraEffect(bool bEnableEffect);

	UFUNCTION(BlueprintImplementableEvent, Category = "Game")
	void Event_ShowResultsScreen(ECharacterType WinnerType, bool bYouWon);

	UFUNCTION(BlueprintImplementableEvent, Category = "Game")
	void Event_SetupResultsScreen();

	UPROPERTY(BlueprintAssignable, Category = "Game")
	FOnNoobGameOver OnGameOver;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;
	UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;
	UPROPERTY(EditAnywhere, Category = "Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;
	TObjectPtr<UUserWidget> MobileControlsWidget;

	UFUNCTION()
	void OnEndingMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};