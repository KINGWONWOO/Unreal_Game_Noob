#pragma once

#include "CoreMinimal.h"
#include "NoobPlayerState.h"
#include "OXQuizPlayerState.generated.h"

UCLASS()
class NOOBGAME_API AOXQuizPlayerState : public ANoobPlayerState
{
    GENERATED_BODY()

public:
    // --- 초기화 및 프레임워크 ---
    AOXQuizPlayerState();

    // 네트워크 복제 설정 (상태 동기화의 핵심)
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // --- 데이터 설정 및 서버 API ---
    // 서버 전용: 플레이어의 준비 상태를 완료로 변경
    void SetInstructionReady_Server();

    // --- 유틸리티 및 헬퍼 ---
    // 블루프린트 UI 등에서 현재 로컬 플레이어의 상태를 즉시 가져오기 위한 정적 함수
    UFUNCTION(BlueprintPure, Category = "Game Helper", meta = (WorldContext = "WorldContextObject"))
    static AOXQuizPlayerState* GetOXPlayerState(const UObject* WorldContextObject);
};