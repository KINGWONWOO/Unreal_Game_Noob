#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameTypes.h"
#include "NoobGameStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNoobWinnerAnnouncement, FString, WinnerName);

UCLASS()
class NOOBGAME_API ANoobGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	ANoobGameStateBase();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
	TObjectPtr<APlayerState> Winner;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
	ECharacterType WinningCharacterType;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AnnounceWinner(const FString& WinnerName);

	UPROPERTY(BlueprintAssignable, Category = "Game State")
	FOnNoobWinnerAnnouncement OnWinnerAnnouncement;
};