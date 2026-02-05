#pragma once

#include "CoreMinimal.h"
#include "NoobPlayerController.h"
#include "MazePlayerController.generated.h"

UCLASS()
class NOOBGAME_API AMazePlayerController : public ANoobPlayerController
{
    GENERATED_BODY()

public:
    // --- UI 및 로컬 인터페이스 ---
    // UI(위젯) 버튼 클릭 시 호출되는 함수
    UFUNCTION(BlueprintCallable, Category = "Maze Game")
    void PlayerReady();

    // --- 네트워크 통신 (RPC) ---
    // 클라이언트에서 서버로 준비 상태를 전송하는 함수
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_PlayerReady();
};