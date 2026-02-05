#pragma once

#include "CoreMinimal.h"
#include "NoobPlayerState.h"
#include "MazePlayerState.generated.h"

UCLASS()
class NOOBGAME_API AMazePlayerState : public ANoobPlayerState
{
    GENERATED_BODY()

public:
    AMazePlayerState();

    // 네트워크 복제 설정 (부모 클래스의 설정을 확장)
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // 블루프린트에서 현재 로컬 플레이어의 상태 정보를 쉽게 가져오기 위한 헬퍼 함수
    UFUNCTION(BlueprintPure, Category = "Maze | Helper", meta = (WorldContext = "WorldContextObject"))
    static AMazePlayerState* GetMazePlayerState(const UObject* WorldContextObject);
};