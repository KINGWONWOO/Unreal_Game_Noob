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

	UFUNCTION(BlueprintCallable, Category = "Transition")
	void RequestLevelTransition(const FString& MapName);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestPunch(ACharacter* HitCharacter);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestPlayPunchMontage();

	UFUNCTION(Server, Reliable)
	void Server_RequestLevelTransition(const FString& MapName);

	/** 액터 대상 전용 펀치 함수 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Lobby")
	void RequestActorPunch(AActor* TargetActor);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestActorPunch(AActor* TargetActor);

	/** 카메라 쉐이크 활성화 여부 (UI 설정에서 변경 가능) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bEnableCameraShake = true;

	/** 펀치 타격 시 쉐이크 클래스 */
	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<class UCameraShakeBase> PunchCameraShakeClass;

	/** 카메라 쉐이크 실행 함수 */
	void PlayPunchCameraShake();

	UFUNCTION(Client, Reliable)
	void Client_PlayHitCameraShake();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetupEnding(bool bIsWinner, FVector TargetLocation, FRotator TargetRotation, ECharacterType WinnerType, ACameraActor* EndingCamera);

	UFUNCTION(Client, Reliable)
	void Client_SetCameraEffect(bool bEnableKnockdownEffect);

	UFUNCTION(Client, Reliable)
	void Client_SetUIOnlyInput(bool bYouWon, ECharacterType WinnerType, ACameraActor* EndingCamera, FVector TargetLocation, FRotator TargetRotation);
	
	UFUNCTION(Client, Reliable)
	void Client_EnableMovementAfterEnding();

	UFUNCTION(Client, Reliable)
	void Client_ShowLoadingScreen();

	UFUNCTION(Client, Reliable)
	void Client_HideLoadingScreen();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int OpenUICount;

	UFUNCTION(BlueprintCallable, Category = "UI")
	int ChangeOpenUICount(bool OpenUI);

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