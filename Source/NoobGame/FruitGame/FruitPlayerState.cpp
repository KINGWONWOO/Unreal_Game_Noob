// FruitPlayerState.cpp

#include "FruitGame/FruitPlayerState.h"
#include "Net/UnrealNetwork.h" // DOREPLIFETIME을 사용하기 위해 필수

AFruitPlayerState::AFruitPlayerState()
{
	// 변수 기본값 초기화
	bHasSubmittedFruits = false;
	bIsReady_Instructions = false;
}

void AFruitPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 복제할 변수들을 등록합니다.
	// SecretFruitAnswers는 보안을 위해 절대 등록하지 않습니다.
	DOREPLIFETIME(AFruitPlayerState, bHasSubmittedFruits);
	DOREPLIFETIME(AFruitPlayerState, bIsReady_Instructions); // 신규 변수 복제 등록
}

// --- 서버 전용 함수 ---

void AFruitPlayerState::SetSecretAnswers_Server(const TArray<EFruitType>& Answers)
{
	// 오직 서버(Authority)에서만 이 변수를 수정할 수 있도록 강제합니다.
	if (HasAuthority())
	{
		SecretFruitAnswers = Answers;
		bHasSubmittedFruits = true; // 정답을 설정했으므로 Setup 준비 완료 상태로 변경

		// bHasSubmittedFruits가 true가 되었으므로, 모든 클라이언트에 복제될 것입니다.
		// 서버에서도 OnRep 함수를 수동으로 호출해주는 것이 좋습니다. (서버 로컬 UI 갱신 등)
		OnRep_HasSubmittedFruits();
	}
}

const TArray<EFruitType>& AFruitPlayerState::GetSecretAnswers_Server() const
{
	// Get은 서버가 아니어도 호출은 가능하지만,
	// 클라이언트에서는 SecretFruitAnswers 배열이 비어있으므로(복제되지 않았으므로) 의미가 없습니다.
	// 사용 시 HasAuthority() 체크를 권장합니다.
	return SecretFruitAnswers;
}

// (신규) Instructions 준비 완료 함수 구현
void AFruitPlayerState::SetInstructionReady_Server()
{
	// 서버에서만 실행
	if (HasAuthority())
	{
		// 아직 준비가 안 된 상태일 때만 실행
		if (!bIsReady_Instructions)
		{
			bIsReady_Instructions = true;

			// bIsReady_Instructions가 복제될 것이므로
			// 서버측 UI/로직을 위해 OnRep 함수를 수동으로 호출합니다.
			// (이 함수는 private/protected이므로 클래스 내부에서 호출 가능)
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