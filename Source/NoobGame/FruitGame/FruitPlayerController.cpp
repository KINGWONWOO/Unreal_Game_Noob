// FruitPlayerController.cpp

#include "FruitGame/FruitPlayerController.h"
#include "FruitGame/FruitGameMode.h"
#include "FruitGame/FruitPlayerState.h"
#include "FruitGame/FruitGameState.h"
#include "FruitGame/InteractableFruitObject.h"
#include "FruitGame/SubmitGuessButton.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h" 

// --- (신규) 로컬 정답 Get 함수 ---
const TArray<EFruitType>& AFruitPlayerController::GetMyLocalSecretAnswers() const
{
	return MyLocalSecretAnswers;
}


// --- 캐릭터(Pawn) -> 컨트롤러 ---
void AFruitPlayerController::RequestInteract(AActor* HitActor)
{
	// 캐릭터(클라이언트)가 호출 -> 서버 RPC 실행
	Server_RequestInteract(HitActor);
}


// --- 1. Instructions 단계 (UI) ---
void AFruitPlayerController::PlayerReady()
{
	Server_PlayerReady();
}
bool AFruitPlayerController::Server_PlayerReady_Validate() { return true; }
void AFruitPlayerController::Server_PlayerReady_Implementation()
{
	AFruitGameMode* GM = GetWorld()->GetAuthGameMode<AFruitGameMode>();
	if (GM)
	{
		GM->PlayerIsReady(this);
	}
}

// --- 2. Setup 단계 (UI) (수정됨) ---
void AFruitPlayerController::SubmitSecretFruits(const TArray<EFruitType>& SecretFruits)
{
	/** (신규)
	 * 서버 RPC를 호출하기 전, '내'가 '내' 컨트롤러의 로컬 변수에 정답을 저장합니다.
	 * IsLocalController() 확인은 이 PC가 실제로 이 컨트롤러의 주인인지 확인합니다.
	 */
	if (IsLocalController())
	{
		MyLocalSecretAnswers = SecretFruits;
	}

	// (기존) 서버에 정답을 제출 (RPC 호출)
	Server_SubmitSecretFruits(SecretFruits);
}

bool AFruitPlayerController::Server_SubmitSecretFruits_Validate(const TArray<EFruitType>& SecretFruits) { return SecretFruits.Num() == 5; }
void AFruitPlayerController::Server_SubmitSecretFruits_Implementation(const TArray<EFruitType>& SecretFruits)
{
	AFruitGameMode* GM = GetWorld()->GetAuthGameMode<AFruitGameMode>();
	if (GM)
	{
		// GameMode의 원래 함수 호출 (정답을 PlayerState에 저장)
		GM->PlayerSubmittedFruits(this, SecretFruits);
	}
}

// --- 3. PlayerTurn 단계 (월드 상호작용) ---
bool AFruitPlayerController::Server_RequestInteract_Validate(AActor* HitActor) { return HitActor != nullptr; }
void AFruitPlayerController::Server_RequestInteract_Implementation(AActor* HitActor)
{
	AFruitGameMode* GM = GetWorld()->GetAuthGameMode<AFruitGameMode>();
	AFruitGameState* GS = GetWorld()->GetGameState<AFruitGameState>();

	if (GM && GS && HitActor)
	{
		// GameMode에 현재 게임 단계를 같이 넘겨줍니다.
		// GameMode는 GP_PlayerTurn일 때만 처리할 것입니다.
		GM->PlayerInteracted(this, HitActor, GS->CurrentGamePhase);
	}
}

// --- 4. 서버 -> 클라이언트 RPC ---
void AFruitPlayerController::Client_StartTurn_Implementation() { OnTurnStarted.Broadcast(); }
void AFruitPlayerController::Client_ReceiveGuessResult_Implementation(const TArray<EFruitType>& Guess, int32 MatchCount) { OnGuessResultReceived.Broadcast(Guess, MatchCount); }
void AFruitPlayerController::Client_OpponentGuessed_Implementation(const TArray<EFruitType>& Guess, int32 MatchCount) { OnOpponentGuessReceived.Broadcast(Guess, MatchCount); }
void AFruitPlayerController::Client_GameOver_Implementation(bool bYouWon) { OnGameOver.Broadcast(bYouWon); }