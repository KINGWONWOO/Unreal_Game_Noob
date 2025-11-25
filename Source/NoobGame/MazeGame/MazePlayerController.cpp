#include "MazePlayerController.h"
#include "MazeGameMode.h"

void AMazePlayerController::PlayerReady()
{
	Server_PlayerReady();
}

bool AMazePlayerController::Server_PlayerReady_Validate() { return true; }
void AMazePlayerController::Server_PlayerReady_Implementation()
{
	if (AMazeGameMode* GM = GetWorld()->GetAuthGameMode<AMazeGameMode>())
	{
		GM->PlayerIsReady(this);
	}
}