#pragma once

#include "CoreMinimal.h"
#include "../NoobPlayerController.h" 
#include "OXQuizPlayerController.generated.h"

UCLASS()
class NOOBGAME_API AOXQuizPlayerController : public ANoobPlayerController
{
    GENERATED_BODY()

public:
    // --- 1. 로컬 UI/블루프린트 인터페이스 ---
    // 위젯의 준비 버튼 등에서 호출되는 로컬 함수
    UFUNCTION(BlueprintCallable, Category = "Quiz")
    void PlayerReady();

    // --- 2. 서버 RPC (클라이언트 -> 서버 요청) ---
    // 서버에 플레이어의 준비 상태를 알림
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_PlayerReady();

    // 서버에 게임 난이도 변경을 요청 (방장 전용 로직 등에서 활용)
    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Game Logic")
    void Server_RequestSetDifficulty(EQuizDifficulty NewDifficulty);

protected:
    // 향후 퀴즈 전용 UI 참조 변수 등을 위한 공간
};