#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "MyNPC.generated.h"

class USoundBase;
class USoundAttenuation;

UCLASS()
class NOOBGAME_API AMyNPC : public ACharacter
{
    GENERATED_BODY()

public:
    AMyNPC();

protected:
    // 초기 설정 및 매 프레임 업데이트
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
    // 상호작용 시스템
    // 블루프린트에서 오버라이드 가능한 인터랙션 핵심 함수
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "NPC|Interaction")
    void Interact(APlayerController* InteractedPlayerController);
    virtual void Interact_Implementation(APlayerController* InteractedPlayerController);

    // 대화 시작 및 종료 제어
    UFUNCTION(BlueprintCallable, Category = "NPC|Interaction")
    void StartDialogue(APlayerController* PC);

    UFUNCTION(BlueprintCallable, Category = "NPC|Interaction")
    void ExitDialogue();

    // 대화 내용 텍스트 추출 로직
    UFUNCTION(BlueprintCallable, Category = "NPC|Dialogue")
    FString GetNextDialogue();

    // 전투 및 피격 시스템
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

    // 모든 클라이언트에게 피격 애니메이션과 사운드를 전달
    UFUNCTION(NetMulticast, Unreliable)
    void Multicast_PlayHitEffects(UAnimMontage* TargetMontage);

    // NPC 상태 변수 (네트워크 동기화 포함)
    UPROPERTY(Replicated, BlueprintReadWrite, Category = "NPC|State")
    bool bIsOccupied = false; // 현재 다른 플레이어와 대화 중인지 여부

    UPROPERTY(BlueprintReadWrite, Category = "NPC|State")
    bool bIsDialogueMode = false; // 대화 모드 활성화 여부

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "NPC|State")
    bool bIsAngryState = false; // 공격받아 화난 상태인지 여부

protected:
    // UI 및 오디오 설정 에셋
    UPROPERTY(EditAnywhere, Category = "NPC|UI")
    TSubclassOf<UUserWidget> NpcTalkWidgetClass;

    UPROPERTY(BlueprintReadOnly, Category = "NPC|UI")
    UUserWidget* CurrentTalkWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Audio")
    USoundBase* HitSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Audio")
    USoundAttenuation* HitAttenuation;

    // 대화 텍스트 리스트
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Dialogue")
    TArray<FString> NormalDialogues;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Dialogue")
    TArray<FString> HitDialogues;

    // 피격 방향별 몽타주
    UPROPERTY(EditAnywhere, Category = "NPC|Animation")
    class UAnimMontage* HitMontage_Front;

    UPROPERTY(EditAnywhere, Category = "NPC|Animation")
    class UAnimMontage* HitMontage_Back;

    UPROPERTY(EditAnywhere, Category = "NPC|Animation")
    class UAnimMontage* HitMontage_Left;

    UPROPERTY(EditAnywhere, Category = "NPC|Animation")
    class UAnimMontage* HitMontage_Right;

    // 대화 중 회전 및 시선 처리 관련
    UPROPERTY(EditAnywhere, Category = "NPC|Interaction")
    float RotationSpeed = 6.0f;

    UPROPERTY(BlueprintReadOnly, Category = "NPC|AI")
    FVector LookAtTargetLocation;

    UPROPERTY(BlueprintReadWrite, Category = "NPC|Interaction")
    FRotator TargetRotation;

    UPROPERTY(BlueprintReadWrite, Category = "NPC|Interaction")
    class APlayerController* CurrentInteractingPC;

private:
    // 내부 헬퍼 함수: 헤드 트래킹 및 가까운 플레이어 탐색
    void UpdateSmoothHeadTracking(float DeltaTime);
    APawn* GetClosestPlayer();
};