#pragma once

#include "CoreMinimal.h"
#include "QuizObstacleBase.h"
#include "OXQuizObstacle_3Choice.generated.h"

// 컴포넌트 전방 선언
class UBoxComponent;
class UStaticMeshComponent;
class UTextRenderComponent;
class USceneComponent;

UCLASS()
class NOOBGAME_API AOXQuizObstacle_3Choice : public AQuizObstacleBase
{
    GENERATED_BODY()

public:
    AOXQuizObstacle_3Choice();

protected:
    // --- 선택지 구성을 위한 컴포넌트 배열 ---

    // 각 입구의 트랜스폼 기준점
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TArray<TObjectPtr<USceneComponent>> EntranceRoots;

    // 선택지 벽 메쉬
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TArray<TObjectPtr<UStaticMeshComponent>> EntranceMeshes;

    // 물리적 통과를 제어하는 콜리전
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TArray<TObjectPtr<UBoxComponent>> EntranceCollisions;

    // 선택지 내용을 표시하는 텍스트 렌더러
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TArray<TObjectPtr<UTextRenderComponent>> EntranceAnswerTexts;

    // 부모 클래스의 순수 가상 함수 구현
    virtual void SetupQuizVisualsAndCollision() override;

private:
    // 3지선다 설정을 위한 상수
    const int32 NumEntrances = 3;
};