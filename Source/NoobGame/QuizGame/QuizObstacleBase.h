#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameTypes.h" // FQuizData 포함
#include "QuizObstacleBase.generated.h"

class UTextRenderComponent;
class UBoxComponent;

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

    // 트리거 박스들
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UBoxComponent> Trigger_1;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UBoxComponent> Trigger_2;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UBoxComponent> Trigger_3;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UBoxComponent> Trigger_4;

    // [Common] 카테고리 텍스트 (New)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UTextRenderComponent> CategoryText;

    // [Common] 질문 텍스트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UTextRenderComponent> QuestionText;

    // --- Replicated Data ---
    UPROPERTY(ReplicatedUsing = OnRep_CurrentQuizData)
    FQuizData CurrentQuizData;

    UPROPERTY(Replicated)
    float MoveSpeed;

    UPROPERTY(ReplicatedUsing = OnRep_IsMoving)
    bool bIsMoving;

    UFUNCTION()
    void OnRep_IsMoving();

    UFUNCTION()
    virtual void OnRep_CurrentQuizData();

    /** 자식 클래스는 이 함수에서 '정답(Answers)'에 대한 비주얼만 처리하면 됩니다. */
    virtual void SetupQuizVisualsAndCollision() PURE_VIRTUAL(AQuizObstacleBase::SetupQuizVisualsAndCollision, );

    // --- [Settings] 글자 크기 설정 ---

    // 카테고리 (New)
    UPROPERTY(EditDefaultsOnly, Category = "Quiz Visuals|Category")
    float CategoryMaxSize = 35.0f; // 카테고리는 보통 짧으니 좀 크게

    UPROPERTY(EditDefaultsOnly, Category = "Quiz Visuals|Category")
    float CategoryMinSize = 20.0f;

    // 질문
    UPROPERTY(EditDefaultsOnly, Category = "Quiz Visuals|Question")
    float QuestionMaxSize = 25.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Quiz Visuals|Question")
    float QuestionMinSize = 15.0f;

    // 정답
    UPROPERTY(EditDefaultsOnly, Category = "Quiz Visuals|Answer")
    float AnswerMaxSize = 26.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Quiz Visuals|Answer")
    float AnswerMinSize = 15.0f;

    // --- Helpers ---
    float CalculateFontSize(int32 TextLength, float MaxSize, float MinSize);

    UFUNCTION(BlueprintCallable, Category = "Quiz Helper")
    FString AddLineBreaksToText(FString InText, int32 MaxLineLength);

private:
    void PushOverlappingCharacters(float DeltaTime);
};