#include "NoobPlayerState.h"
#include "Net/UnrealNetwork.h"

ANoobPlayerState::ANoobPlayerState()
{
	bIsReady_Instructions = false;
	bIsRoomOwner = false;
	PunchHitCount = 0;
	bIsKnockedDown = false;
	bIsNextPunchLeft = true;
}

void ANoobPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANoobPlayerState, SelectedPawnClass);
	DOREPLIFETIME(ANoobPlayerState, bIsReady_Instructions);
	DOREPLIFETIME(ANoobPlayerState, bIsRoomOwner);
	DOREPLIFETIME(ANoobPlayerState, PunchHitCount);
	DOREPLIFETIME(ANoobPlayerState, bIsKnockedDown);
	DOREPLIFETIME(ANoobPlayerState, bIsNextPunchLeft);
}

void ANoobPlayerState::SetInstructionReady_Server()
{
	if (HasAuthority())
	{
		bIsReady_Instructions = true;
	}
}

void ANoobPlayerState::OnRep_IsRoomOwner()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] OnRep_IsRoomOwner Triggered! Am I Owner? %s"),
		*GetName(), bIsRoomOwner ? TEXT("YES") : TEXT("NO"));
}