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
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(EditAnywhere, Category = "NPC|UI")
    TSubclassOf<UUserWidget> NpcTalkWidgetClass;

    UPROPERTY(BlueprintReadOnly, Category = "NPC|UI")
    UUserWidget* CurrentTalkWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Audio")
    USoundBase* HitSound;

    // [추가] 피격 사운드 감쇄 설정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Audio")
    USoundAttenuation* HitAttenuation;

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "NPC|Interaction")
    void Interact(APlayerController* InteractedPlayerController);
    virtual void Interact_Implementation(APlayerController* InteractedPlayerController);

    UFUNCTION(BlueprintCallable, Category = "NPC|Interaction")
    void StartDialogue(APlayerController* PC);

    /** 대화 종료 및 모든 설정 복구 */
    UFUNCTION(BlueprintCallable, Category = "NPC|Interaction")
    void ExitDialogue();

    UFUNCTION(BlueprintCallable, Category = "NPC|Dialogue")
    FString GetNextDialogue();

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

    UPROPERTY(Replicated, BlueprintReadWrite, Category = "NPC|State")
    bool bIsOccupied = false;

    UPROPERTY(BlueprintReadWrite, Category = "NPC|State")
    bool bIsDialogueMode = false;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "NPC|State")
    bool bIsAngryState = false;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Dialogue")
    TArray<FString> NormalDialogues;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Dialogue")
    TArray<FString> HitDialogues;

    UPROPERTY(EditAnywhere, Category = "NPC|Animation")
    class UAnimMontage* HitMontage_Front;

    UPROPERTY(EditAnywhere, Category = "NPC|Animation")
    class UAnimMontage* HitMontage_Back;

    UPROPERTY(EditAnywhere, Category = "NPC|Animation")
    class UAnimMontage* HitMontage_Left;

    UPROPERTY(EditAnywhere, Category = "NPC|Animation")
    class UAnimMontage* HitMontage_Right;


    UFUNCTION(NetMulticast, Unreliable)
    void Multicast_PlayHitEffects(UAnimMontage* TargetMontage);

protected:
    UPROPERTY(EditAnywhere, Category = "NPC|Interaction")
    float RotationSpeed = 6.0f;

    UPROPERTY(BlueprintReadOnly, Category = "NPC|AI")
    FVector LookAtTargetLocation;

    UPROPERTY(BlueprintReadWrite, Category = "NPC|Interaction")
    FRotator TargetRotation;

    UPROPERTY(BlueprintReadWrite, Category = "NPC|Interaction")
    class APlayerController* CurrentInteractingPC;

private:
    void UpdateSmoothHeadTracking(float DeltaTime);
    APawn* GetClosestPlayer();
};