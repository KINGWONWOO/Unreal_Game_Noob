#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameTypes.h"
#include "NiagaraFunctionLibrary.h"
#include "QuizObstacleBase.generated.h"

class UTextRenderComponent;
class UBoxComponent;
class UNiagaraSystem;
class USoundBase;

UENUM(BlueprintType)
enum class EObstacleState : uint8
{
    Falling,
    Landing,
    Moving
};

UCLASS(Abstract)
class NOOBGAME_API AQuizObstacleBase : public AActor
{
    GENERATED_BODY()

public:
    AQuizObstacleBase();

    virtual void Tick(float DeltaTime) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void InitializeObstacle(const FQuizData& NewQuizData, float NewMoveSpeed);

protected:
    virtual void BeginPlay() override;

    // --- Components ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USceneComponent> Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UBoxComponent> Trigger_1;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UBoxComponent> Trigger_2;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UBoxComponent> Trigger_3;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UBoxComponent> Trigger_4;

    UPROPERTY(EditDefaultsOnly, Category = "Settings|Movement")
    float GravityScale = 4.0f; // 중력 배수 (높을수록 빠르게 가속)

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    TSubclassOf<UCameraShakeBase> LandingCameraShake;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    float ShakeInnerRadius = 500.f;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    float ShakeOuterRadius = 2000.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UTextRenderComponent> CategoryText;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UTextRenderComponent> QuestionText;

    // --- Effects ---
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    TObjectPtr<UNiagaraSystem> LandingParticle;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    TObjectPtr<USoundBase> LandingSound;

    // --- Replicated Data ---
    UPROPERTY(ReplicatedUsing = OnRep_CurrentQuizData)
    FQuizData CurrentQuizData;

    UPROPERTY(Replicated)
    float MoveSpeed;

    UPROPERTY(ReplicatedUsing = OnRep_ObstacleState)
    EObstacleState ObstacleState;

    UFUNCTION()
    void OnRep_ObstacleState();

    UFUNCTION()
    virtual void OnRep_CurrentQuizData();

    virtual void SetupQuizVisualsAndCollision() PURE_VIRTUAL(AQuizObstacleBase::SetupQuizVisualsAndCollision, );

    // --- Settings (복구됨) ---
    UPROPERTY(EditDefaultsOnly, Category = "Quiz Visuals|Question")
    float QuestionMaxSize = 25.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Quiz Visuals|Question")
    float QuestionMinSize = 15.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Quiz Visuals|Answer")
    float AnswerMaxSize = 26.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Quiz Visuals|Answer")
    float AnswerMinSize = 10.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Settings|Movement")
    float FallSpeed = 2500.f;

    UPROPERTY(EditDefaultsOnly, Category = "Settings|Movement")
    float LandingDelay = 0.6f;

    UPROPERTY(EditDefaultsOnly, Category = "Settings|Movement")
    float SpawnHeightOffset = 1800.f;

    // --- Logic Helpers ---
    void HandleFalling(float DeltaTime);
    void HandleMoving(float DeltaTime);
    void ExecuteLandingSequence();

    float TargetZ;
    FTimerHandle TimerHandle_Landing;

    float CalculateFontSize(int32 TextLength, float MaxSize, float MinSize);
    FString AddLineBreaksToText(FString InText, int32 MaxLineLength);
    void PushOverlappingCharacters(float DeltaTime);

private:
        float CurrentVerticalVelocity = 0.0f;
};