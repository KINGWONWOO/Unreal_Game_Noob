// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

bool UInGameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(CancelButton != nullptr)) return false;
	CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::CancelPressed);

	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::QuitPressed);

	if (!ensure(SettingButton != nullptr)) return false;
	SettingButton->OnClicked.AddDynamic(this, &UInGameMenu::SettingPressed);

	return true;
}

void UInGameMenu::CancelPressed()
{
	Teardown();
}


void UInGameMenu::QuitPressed()
{
	if (MMenuInterface != nullptr) {
		Teardown();
		MMenuInterface->LoadMainMenu();
	}
}

void UInGameMenu::SettingPressed()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(SettingMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(SettingMenu);
}