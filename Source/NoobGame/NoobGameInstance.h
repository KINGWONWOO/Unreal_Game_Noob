#pragma once



#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "NoobGameInstance.generated.h"



UCLASS()
class NOOBGAME_API UNoobGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UNoobGameInstance(const FObjectInitializer& ObjectInitializer);

	// GameInstance 초기화 시 호출됩니다.

	virtual void Init() override;



	// 기존 함수들

	UFUNCTION(BlueprintCallable)

	void LoadMenu();



	UFUNCTION(BlueprintCallable)

	void InGameLoadMenu();



	UFUNCTION(Exec)

	void Host() override;



	UFUNCTION(Exec)

	void Join(const FString& Address) override;



	virtual void LoadMainMenu() override;



	UPROPERTY()

	UUserWidget* LoadingWidget;


protected:

	// 맵 로딩 시작 전 호출될 함수

	UFUNCTION()

	void OnPreLoadMap(const FString& MapName);



	// 맵 로딩 완료 후 호출될 함수

	UFUNCTION()

	void OnPostLoadMap(UWorld* LoadedWorld);



private:

	// 메인 메뉴 위젯 클래스

	TSubclassOf<class UUserWidget> MenuClass;



	TSubclassOf<class UUserWidget> InGameMenuClass;



	// 로딩 화면 위젯 클래스 (비동기 로드를 위해 TSoftClassPtr 사용)

	UPROPERTY()
	TSoftClassPtr<UUserWidget> LoadingScreenClass;



	class UMainMenu* MMenu;

};