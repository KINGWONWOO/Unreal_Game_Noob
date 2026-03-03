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
	// 1. 프레임워크 및 입력 설정
	virtual void SetupInputComponent() override;

	// 2. 전투 시스템 (Combat)
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RequestPunch(ACharacter* HitCharacter);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RequestPlayPunchMontage();

	UFUNCTION(BlueprintCallable, Category = "Combat|Lobby")
	void RequestActorPunch(AActor* TargetActor);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestPunch(ACharacter* HitCharacter);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestPlayPunchMontage();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestActorPunch(AActor* TargetActor);

	// 3. 카메라 및 시각 효과
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bEnableCameraShake = true;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<class UCameraShakeBase> PunchCameraShakeClass;

	void PlayPunchCameraShake();

	UFUNCTION(Client, Reliable)
	void Client_PlayHitCameraShake();

	UFUNCTION(Client, Reliable)
	void Client_SetCameraEffect(bool bEnableKnockdownEffect);

	UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
	void ApplyKnockdownCameraEffect(bool bEnableEffect);

	// 4. 게임 종료 및 엔딩 시퀀스
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetupEnding(bool bIsWinner, FVector TargetLocation, FRotator TargetRotation, ECharacterType WinnerType, ACameraActor* EndingCamera);

	UFUNCTION(Client, Reliable)
	void Client_SetUIOnlyInput(bool bYouWon, ECharacterType WinnerType, ACameraActor* EndingCamera, FVector TargetLocation, FRotator TargetRotation);

	UFUNCTION(Client, Reliable)
	void Client_EnableMovementAfterEnding();

	UFUNCTION(BlueprintImplementableEvent, Category = "Game")
	void Event_ShowResultsScreen(ECharacterType WinnerType, bool bYouWon);

	UFUNCTION(BlueprintImplementableEvent, Category = "Game")
	void Event_SetupResultsScreen();

	UPROPERTY(BlueprintAssignable, Category = "Game")
	FOnNoobGameOver OnGameOver;

	// 5. 로딩 및 레벨 전환
	UFUNCTION(BlueprintCallable, Category = "Transition")
	void RequestLevelTransition(const FString& MapName);

	UFUNCTION(Server, Reliable)
	void Server_RequestLevelTransition(const FString& MapName);

	UFUNCTION(Client, Reliable)
	void Client_ShowLoadingScreen();

	UFUNCTION(Client, Reliable)
	void Client_HideLoadingScreen();

	// 6. 애니메이션 동기화
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayHitReaction(ACharacter* TargetCharacter, UAnimMontage* MontageToPlay);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayPunchMontage(ACharacter* PunchingCharacter, UAnimMontage* MontageToPlay);

	// 7. UI 상태 관리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int OpenUICount;

	UFUNCTION(BlueprintCallable, Category = "UI")
	int ChangeOpenUICount(bool OpenUI);

protected:
	// 생명주기 및 내부 로직
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	UPROPERTY(EditAnywhere, Category = "Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	// 서버 전용 Fail-Safe 타이머 핸들
    FTimerHandle AttackSafeTimerHandle;

	TObjectPtr<UUserWidget> MobileControlsWidget;

	UFUNCTION()
	void OnEndingMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
