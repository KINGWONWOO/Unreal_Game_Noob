#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameTypes.h"
#include "NoobGameStateBase.generated.h"

// 승자 발표 시 UI 및 이펙트 처리를 위한 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNoobWinnerAnnouncement, FString, WinnerName);

UCLASS()
class NOOBGAME_API ANoobGameStateBase : public AGameStateBase
{
    GENERATED_BODY()

public:
    ANoobGameStateBase();

    // 1. 프레임워크 오버라이드 및 네트워크 설정
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // 2. 네트워크 복제 변수 (모든 클라이언트가 공유)
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
    TObjectPtr<APlayerState> Winner;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
    ECharacterType WinningCharacterType;

    // 3. 게임 이벤트 알림 (RPC & Delegate)
    // 모든 클라이언트에게 승자 발표를 알리는 멀티캐스트 RPC
    UFUNCTION(NetMulticast, Reliable)
    void Multicast_AnnounceWinner(const FString& WinnerName);

    // 클라이언트 UI 바인딩용 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "Game State")
    FOnNoobWinnerAnnouncement OnWinnerAnnouncement;
};