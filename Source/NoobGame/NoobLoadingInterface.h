#pragma once

#include "UObject/Interface.h"
#include "NoobLoadingInterface.generated.h"

UINTERFACE(BlueprintType)
class UNoobLoadingInterface : public UInterface
{
	GENERATED_BODY()
};

class INoobLoadingInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Loading")
	void ShowLoading();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Loading")
	void HideLoading();
};
