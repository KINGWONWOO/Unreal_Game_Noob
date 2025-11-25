#include "OXQuizPlayerController.h"
#include "OXQuizGameMode.h"
#include "Kismet/GameplayStatics.h"

void AOXQuizPlayerController::PlayerReady()
{
	Server_PlayerReady();
}

bool AOXQuizPlayerController::Server_PlayerReady_Validate() { return true; }
void AOXQuizPlayerController::Server_PlayerReady_Implementation()
{
	if (AOXQuizGameMode* GM = GetWorld()->GetAuthGameMode<AOXQuizGameMode>())
	{
		GM->PlayerIsReady(this);
	}
}

void AOXQuizPlayerController::Server_RequestSetDifficulty_Implementation(EQuizDifficulty NewDifficulty)
{
	if (AOXQuizGameMode* GM = Cast<AOXQuizGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->SetGameDifficulty(NewDifficulty);
		UE_LOG(LogTemp, Warning, TEXT("[Server] Difficulty Changed to %d"), (int32)NewDifficulty);
	}
}