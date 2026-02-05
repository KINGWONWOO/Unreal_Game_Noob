#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameTypes.h"
#include "NoobGameModeBase.generated.h"

class ACharacter;
class UAnimMontage;
class ANoobPlayerState;
class ANoobPlayerController;

UCLASS()
class NOOBGAME_API ANoobGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    ANoobGameModeBase();

    // 1. 프레임워크 오버라이드
    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual void Logout(AController* Exiting) override;

    // 2. 게임 흐름 제어
    // 승자 발표 단계 시작
    virtual void StartWinnerAnnouncement(APlayerState* Winner);

    // 게임 종료 처리 및 엔딩 시퀀스 전환
    UFUNCTION(BlueprintCallable, Category = "Game")
    virtual void EndGame(APlayerState* Winner);

    // 방장의 맵 전환 요청 처리
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void Server_TransitionToSelectedMap(FString MapName);

    // 3. 전투 및 충돌 로직
    // 펀치 적중 처리 및 넉다운 판정
    UFUNCTION(BlueprintCallable, Category = "Game")
    virtual void ProcessPunch(APlayerController* PuncherController, ACharacter* HitCharacter);

    // 모든 클라이언트에게 펀치 애니메이션 재생 요청
    UFUNCTION(BlueprintCallable, Category = "Game")
    void ProcessPunchAnimation(ACharacter* PunchingCharacter, UAnimMontage* MontageToPlay);

protected:
    // 내부 헬퍼 로직
    virtual bool IsGameInProgress() const { return true; }
    virtual void AnnounceWinnerToClients(APlayerState* Winner) {}
    virtual void CleanupLevelActors() {}

    // 플레이어 접속 끊김 시 패배 처리 로직
    void HandlePlayerDisconnect(AController* ExitingPlayer);

    // 넉다운 상태 복구 (타이머 호출용)
    UFUNCTION()
    void RecoverCharacter(ACharacter* CharacterToRecover);

    // 설정값
    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    float PunchPushForce = 500.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    float KnockdownDuration = 8.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
    float WinnerAnnouncementDuration = 3.0f;

    // 엔딩 시퀀스 설정 (Tags)
    UPROPERTY(EditDefaultsOnly, Category = "GameOver")
    FName WinnerSpawnTag = TEXT("Result_Spawn_Winner");

    UPROPERTY(EditDefaultsOnly, Category = "GameOver")
    FName LoserSpawnTag = TEXT("Result_Spawn_Defeat");

    UPROPERTY(EditDefaultsOnly, Category = "GameOver")
    FName EndingCameraTag = TEXT("EndingCamera");

    // 실행 변수 (Runtime)
    bool bHasAssignedRoomOwner = false;
    FTimerHandle EndGameDelayTimerHandle;

    // 캐릭터별 넉다운 관리
    UPROPERTY()
    TMap<TWeakObjectPtr<ACharacter>, FTimerHandle> KnockdownTimers;
};