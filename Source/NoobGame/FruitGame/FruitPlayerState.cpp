// FruitPlayerState.cpp

#include "FruitGame/FruitPlayerState.h"
#include "Net/UnrealNetwork.h" // DOREPLIFETIME

AFruitPlayerState::AFruitPlayerState()
{
	// 변수 기본값 초기화
	bHasSubmittedFruits = false;
	bIsReady_Instructions = false;
	PunchHitCount = 0;
	bIsKnockedDown = false;
	bIsNextPunchLeft = true; // (신규!) 기본값은 왼쪽 펀치부터
}

void AFruitPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 복제할 변수들을 등록합니다.
	DOREPLIFETIME(AFruitPlayerState, bHasSubmittedFruits);
	DOREPLIFETIME(AFruitPlayerState, bIsReady_Instructions);
	DOREPLIFETIME(AFruitPlayerState, PunchHitCount);
	DOREPLIFETIME(AFruitPlayerState, bIsKnockedDown);
	DOREPLIFETIME(AFruitPlayerState, bIsNextPunchLeft); // (신규!) 복제 등록
}

// --- 서버 전용 함수 ---

void AFruitPlayerState::SetSecretAnswers_Server(const TArray<EFruitType>& Answers)
{
	// 오직 서버(Authority)에서만 이 변수를 수정할 수 있도록 강제합니다.
	if (HasAuthority())
	{
		SecretFruitAnswers = Answers;
		bHasSubmittedFruits = true;

		// 서버에서도 OnRep 함수를 수동으로 호출해주는 것이 좋습니다.
		OnRep_HasSubmittedFruits();
	}
}

const TArray<EFruitType>& AFruitPlayerState::GetSecretAnswers_Server() const
{
	return SecretFruitAnswers;
}

void AFruitPlayerState::SetInstructionReady_Server()
{
	// 서버에서만 실행
	if (HasAuthority())
	{
		// 아직 준비가 안 된 상태일 때만 실행
		if (!bIsReady_Instructions)
		{
			bIsReady_Instructions = true;
			OnRep_IsReady_Instructions();
		}
	}
}

// --- OnRep 함수 (클라이언트에서 실행됨) ---

void AFruitPlayerState::OnRep_HasSubmittedFruits()
{
	// Setup 준비 상태가 변경되었음을 UI(블루프린트)에 알립니다.
	OnReadyStateChanged.Broadcast();
}

void AFruitPlayerState::OnRep_IsReady_Instructions()
{
	// Instructions 준비 상태가 변경되었음을 UI(블루프린트)에 알립니다.
	OnInstructionReadyChanged.Broadcast();
}

/** (신규!) 쓰러짐 상태가 복제될 때 델리게이트 호출 */
void AFruitPlayerState::OnRep_KnockedDown()
{
	// 쓰러짐 상태가 변경되었음을 캐릭터 BP에 알립니다.
	OnKnockdownStateChanged.Broadcast(bIsKnockedDown);
}