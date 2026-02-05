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
    // 초기화 및 생명주기
    UNoobGameInstance(const FObjectInitializer& ObjectInitializer);
    virtual void Init() override;

    // 네트워크 및 서버 이동 인터페이스 (IMenuInterface)
    UFUNCTION(Exec)
    void Host() override;

    UFUNCTION(Exec)
    void Join(const FString& Address) override;

    virtual void LoadMainMenu() override;

    // 메뉴 시스템 호출
    UFUNCTION(BlueprintCallable)
    void LoadMenu();

    UFUNCTION(BlueprintCallable)
    void InGameLoadMenu();

    // 로딩 화면 참조
    UPROPERTY()
    UUserWidget* LoadingWidget;

protected:
    // 맵 전환 이벤트 델리게이트 응답 함수
    UFUNCTION()
    void OnPreLoadMap(const FString& MapName);

    UFUNCTION()
    void OnPostLoadMap(UWorld* LoadedWorld);

private:
    // 위젯 클래스 에셋 참조
    TSubclassOf<class UUserWidget> MenuClass;
    TSubclassOf<class UUserWidget> InGameMenuClass;

    // 비동기 및 메모리 최적화를 위한 로딩 화면 소프트 포인터
    UPROPERTY()
    TSoftClassPtr<UUserWidget> LoadingScreenClass;

    class UMainMenu* MMenu;
};