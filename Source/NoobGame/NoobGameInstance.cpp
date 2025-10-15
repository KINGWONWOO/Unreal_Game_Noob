// NoobGameInstance.cpp

#include "NoobGameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MoviePlayer.h" // MoviePlayer 헤더 추가

#include "MenuSystem/MainMenu.h"

UNoobGameInstance::UNoobGameInstance(const FObjectInitializer& ObjectInitializer)
{
	// 메인 메뉴 위젯 클래스 찾기
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (MenuBPClass.Class)
	{
		MenuClass = MenuBPClass.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr)) return;


	// ★ 로딩 위젯 클래스 경로를 TSoftClassPtr에 할당
	// WBP_TransitonLoading 경로를 사용합니다.
	LoadingScreenClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(TEXT("/Game/MenuSystem/WBP_TransitonLoading")));
}

void UNoobGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());
	// ★ 엔진 델리게이트에 로딩 시작/종료 함수를 바인딩합니다.
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UNoobGameInstance::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UNoobGameInstance::OnPostLoadMap);
}

void UNoobGameInstance::LoadMenu()
{
	
}

void UNoobGameInstance::Host()
{

	if (MMenu != nullptr)
	{
		MMenu->Teardown();
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	// ★ 이제 로딩 화면 걱정 없이 바로 Travel을 호출하면 됩니다.
	World->ServerTravel("/Game/Levels/Lobby?listen");
}

void UNoobGameInstance::Join(const FString& Address)
{
	if (MMenu != nullptr)
	{
		MMenu->Teardown();
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	// ★ 여기서도 바로 Travel을 호출하면 됩니다.
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UNoobGameInstance::OnPreLoadMap(const FString& MapName)
{
	// 맵 로딩이 시작되기 직전에 자동으로 호출되는 부분
	if (IsRunningDedicatedServer())
	{
		return; // 데디케이티드 서버에서는 로딩 화면이 필요 없습니다.
	}

	if (LoadingScreenClass.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadingScreenClass is not set in GameInstance."));
		return;
	}

	// 로딩 화면 설정 구성
	FLoadingScreenAttributes LoadingScreen;
	LoadingScreen.bAutoCompleteWhenLoadingCompletes = false; // PostLoadMap에서 직접 닫을 것이므로 false로 설정
	LoadingScreen.MinimumLoadingScreenDisplayTime = 2.0f; // 최소 2초간 표시

	// 위젯을 동기적으로 로드하고 SWidget으로 변환하여 설정에 추가
	LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
	UUserWidget* Widget = CreateWidget<UUserWidget>(this, LoadingScreenClass.LoadSynchronous());
	if (Widget)
	{
		LoadingScreen.WidgetLoadingScreen = Widget->TakeWidget();
	}

	// MoviePlayer를 통해 로딩 화면을 설정하고 재생
	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
}

void UNoobGameInstance::OnPostLoadMap(UWorld* LoadedWorld)
{
	// 맵 로딩이 완료된 직후에 자동으로 호출되는 부분
	// 특별히 할 작업이 없다면 이 함수는 비워두어도 됩니다.
	// bAutoCompleteWhenLoadingCompletes가 true라면 자동으로 닫히지만, 
	// false로 두었다면 여기서 StopMovie()를 호출하여 수동으로 닫을 수 있습니다.
}
void UNoobGameInstance::InGameLoadMenu()
{
	if (!ensure(InGameMenuClass != nullptr)) return;

	UMenuWidget* Menu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	if (!ensure(Menu != nullptr)) return;

	Menu->Setup();

	Menu->SetMenuInterface(this);
}



void UNoobGameInstance::LoadMainMenu()
{
	// (기존 코드와 동일)
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}