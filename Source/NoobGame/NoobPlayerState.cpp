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
	// 여기서 UI를 갱신하거나 로그를 찍어 확인합니다.
	UE_LOG(LogTemp, Warning, TEXT("[%s] OnRep_IsRoomOwner Triggered! Am I Owner? %s"),
		*GetName(), bIsRoomOwner ? TEXT("YES") : TEXT("NO"));
}