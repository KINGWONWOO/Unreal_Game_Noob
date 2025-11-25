#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NoobPlayerState.generated.h"

UCLASS()
class NOOBGAME_API ANoobPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ANoobPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetInstructionReady_Server();

	UPROPERTY(Replicated)
	TSubclassOf<APawn> SelectedPawnClass;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Noob|Flow")
	bool bIsReady_Instructions;

	UPROPERTY(ReplicatedUsing = OnRep_IsRoomOwner, BlueprintReadOnly, Category = "Noob|Flow")
	bool bIsRoomOwner = false;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Noob|Combat")
	int32 PunchHitCount;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Noob|Combat")
	bool bIsKnockedDown;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Noob|Combat")
	bool bIsNextPunchLeft;

	UFUNCTION()
	void OnRep_IsRoomOwner();
};