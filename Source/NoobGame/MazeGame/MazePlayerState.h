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

    // 네트워크 복제를 위한 설정
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    /** 블루프린트용 헬퍼 함수: 로컬 플레이어의 MazePlayerState를 반환 */
    UFUNCTION(BlueprintPure, Category = "Maze | Helper", meta = (WorldContext = "WorldContextObject"))
    static AMazePlayerState* GetMazePlayerState(const UObject* WorldContextObject);
};