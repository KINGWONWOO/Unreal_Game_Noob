#include "NoobGameStateBase.h"
#include "Net/UnrealNetwork.h"

ANoobGameStateBase::ANoobGameStateBase()
{
	Winner = nullptr;
	WinningCharacterType = ECharacterType::ECT_None;
}

void ANoobGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANoobGameStateBase, Winner);
	DOREPLIFETIME(ANoobGameStateBase, WinningCharacterType);
}

void ANoobGameStateBase::Multicast_AnnounceWinner_Implementation(const FString& WinnerName)
{
	OnWinnerAnnouncement.Broadcast(WinnerName);
}