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

// 장애물의 현재 동작 상태를 정의하는 열거형
UENUM(BlueprintType)
enum class EObstacleState : uint8
{
    Falling, // 하늘에서 떨어지는 상태
    Landing, // 바닥에 닿아 충격을 주는 상태
    Moving   // 플레이어를 밀며 전진하는 상태
};

UCLASS(Abstract)
class NOOBGAME_API AQuizObstacleBase : public AActor
{
    GENERATED_BODY()

public:
    AQuizObstacleBase();

    // 프레임 워크 오버라이드
    virtual void Tick(float DeltaTime) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // 외부(GameMode)에서 장애물을 생성하고 초기화할 때 호출
    void InitializeObstacle(const FQuizData& NewQuizData, float NewMoveSpeed);

protected:
    virtual void BeginPlay() override;

    // 기본 컴포넌트 구성
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USceneComponent> Root;

    // 캐릭터를 밀어내기 위한 4방향 트리거
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UBoxComponent> Trigger_1;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UBoxComponent> Trigger_2;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UBoxComponent> Trigger_3;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UBoxComponent> Trigger_4;

    // 퀴즈 텍스트 렌더러
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UTextRenderComponent> CategoryText;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UTextRenderComponent> QuestionText;

    // 이동 및 물리 설정
    UPROPERTY(EditDefaultsOnly, Category = "Settings|Movement")
    float GravityScale = 4.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Settings|Movement")
    float FallSpeed = 2500.f;
    UPROPERTY(EditDefaultsOnly, Category = "Settings|Movement")
    float LandingDelay = 0.6f;
    UPROPERTY(EditDefaultsOnly, Category = "Settings|Movement")
    float SpawnHeightOffset = 1800.f;

    // 시각 및 청각 효과 (착지 연출)
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    TSubclassOf<UCameraShakeBase> LandingCameraShake;
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    float ShakeInnerRadius = 500.f;
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    float ShakeOuterRadius = 2000.f;
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    TObjectPtr<UNiagaraSystem> LandingParticle;
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    TObjectPtr<USoundBase> LandingSound;

    // 텍스트 비주얼 설정
    UPROPERTY(EditDefaultsOnly, Category = "Quiz Visuals|Question")
    float QuestionMaxSize = 25.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Quiz Visuals|Question")
    float QuestionMinSize = 15.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Quiz Visuals|Answer")
    float AnswerMaxSize = 26.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Quiz Visuals|Answer")
    float AnswerMinSize = 10.0f;

    // 네트워크 동기화 데이터
    UPROPERTY(ReplicatedUsing = OnRep_CurrentQuizData)
    FQuizData CurrentQuizData;

    UPROPERTY(Replicated)
    float MoveSpeed;

    UPROPERTY(ReplicatedUsing = OnRep_ObstacleState)
    EObstacleState ObstacleState;

    // 동기화 응답 함수 및 순수 가상 함수
    UFUNCTION() void OnRep_ObstacleState();
    UFUNCTION() virtual void OnRep_CurrentQuizData();
    virtual void SetupQuizVisualsAndCollision() PURE_VIRTUAL(AQuizObstacleBase::SetupQuizVisualsAndCollision, );

    // 내부 로직 헬퍼
    void HandleFalling(float DeltaTime);
    void HandleMoving(float DeltaTime);
    void ExecuteLandingSequence();
    void PushOverlappingCharacters(float DeltaTime);

    // 수학 및 문자열 가공 헬퍼
    float CalculateFontSize(int32 TextLength, float MaxSize, float MinSize);
    FString AddLineBreaksToText(FString InText, int32 MaxLineLength);

    // 물리 및 타이머 관련
    float TargetZ;
    FTimerHandle TimerHandle_Landing;

private:
    float CurrentVerticalVelocity = 0.0f;
};