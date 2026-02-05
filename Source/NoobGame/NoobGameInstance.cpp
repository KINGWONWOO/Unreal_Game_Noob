#include "NoobGameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MoviePlayer.h"
#include "MenuSystem/MainMenu.h"

// =============================================================
// 1. 초기화 및 에셋 로드 (Initialization)
// =============================================================

UNoobGameInstance::UNoobGameInstance(const FObjectInitializer& ObjectInitializer)
{
    // 메인 메뉴 위젯 클래스 찾기
    ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/UI/MainMenu/WBP_MainMenu"));
    if (MenuBPClass.Class)
    {
        MenuClass = MenuBPClass.Class;
    }

    // 인게임 메뉴 클래스 확인
    ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/UI/Option/WBP_InOption"));
    if (!ensure(InGameMenuBPClass.Class != nullptr)) return;

    // 로딩 위젯 클래스 경로를 소프트 클래스 포인터에 할당 (메모리 최적화)
    LoadingScreenClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(TEXT("/Game/MenuSystem/WBP_TransitonLoading")));
}

void UNoobGameInstance::Init()
{
    Super::Init();

    // 맵 로딩 시작/종료 이벤트를 엔진 델리게이트에 바인딩
    FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UNoobGameInstance::OnPreLoadMap);
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UNoobGameInstance::OnPostLoadMap);
}

// =============================================================
// 2. 네트워크 및 이동 로직 (Networking & Travel)
// =============================================================

void UNoobGameInstance::Host()
{
    if (MMenu != nullptr)
    {
        MMenu->Teardown();
    }

    UWorld* World = GetWorld();
    if (!ensure(World != nullptr)) return;

    // 리슨 서버로 로비 맵 이동
    World->ServerTravel("/Game/Levels/Lobby?listen");
}

void UNoobGameInstance::Join(const FString& Address)
{
    if (MMenu != nullptr)
    {
        MMenu->Teardown();
    }

    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if (!ensure(PlayerController != nullptr)) return;

    // 특정 주소로 클라이언트 접속 시도
    PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UNoobGameInstance::LoadMainMenu()
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if (!ensure(PlayerController != nullptr)) return;

    // 메인 메뉴 맵으로 돌아가기
    PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}

// =============================================================
// 3. 메뉴 위젯 제어 (Menu Management)
// =============================================================

void UNoobGameInstance::LoadMenu()
{
    // 로직 보존 (기존 코드 비어있음)
}

void UNoobGameInstance::InGameLoadMenu()
{
    if (!ensure(InGameMenuClass != nullptr)) return;

    UMenuWidget* Menu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
    if (!ensure(Menu != nullptr)) return;

    Menu->Setup();
    Menu->SetMenuInterface(this);
}

// =============================================================
// 4. 로딩 화면 시스템 (Loading Screen Logic)
// =============================================================

void UNoobGameInstance::OnPreLoadMap(const FString& MapName)
{
    if (IsRunningDedicatedServer()) return; // 서버는 시각적 요소 불필요

    if (LoadingScreenClass.IsNull()) return;

    // 로딩 화면 속성 구성
    FLoadingScreenAttributes LoadingScreen;
    LoadingScreen.bAutoCompleteWhenLoadingCompletes = false; // 수동 제어 모드
    LoadingScreen.MinimumLoadingScreenDisplayTime = 2.0f;    // 최소 노출 시간 보장

    // 소프트 포인터를 사용하여 위젯을 동기적으로 로드 후 슬레이트(Slate)로 변환
    UUserWidget* Widget = CreateWidget<UUserWidget>(this, LoadingScreenClass.LoadSynchronous());
    if (Widget)
    {
        LoadingScreen.WidgetLoadingScreen = Widget->TakeWidget();
    }

    // 엔진의 MoviePlayer에 로딩 화면 등록 및 재생
    GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
}

void UNoobGameInstance::OnPostLoadMap(UWorld* LoadedWorld)
{
    // 맵 로딩 완료 시점에 필요한 추가 작업 수행 (현재 수동 종료중)
}