#pragma once

#include "CoreMinimal.h"
#include "QuizObstacleBase.h"
#include "OXQuizObstacle_2Choice.generated.h"

// 컴포넌트 전방 선언
class UBoxComponent;
class UStaticMeshComponent;
class UTextRenderComponent;
class USceneComponent;

UCLASS()
class NOOBGAME_API AOXQuizObstacle_2Choice : public AQuizObstacleBase
{
    GENERATED_BODY()

public:
    AOXQuizObstacle_2Choice();

protected:
    // 입구 구성을 위한 컴포넌트 배열
    // 각 입구의 기준점이 되는 루트 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TArray<TObjectPtr<USceneComponent>> EntranceRoots;

    // 선택지 벽 메쉬
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TArray<TObjectPtr<UStaticMeshComponent>> EntranceMeshes;

    // 통과를 막는 물리 충돌체
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TArray<TObjectPtr<UBoxComponent>> EntranceCollisions;

    // 선택지(O, X 등) 텍스트 렌더러
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TArray<TObjectPtr<UTextRenderComponent>> EntranceAnswerTexts;

    // 부모 클래스의 추상 함수 구현
    // 데이터에 맞춰 텍스트를 변경하고 충돌 설정을 갱신합니다.
    virtual void SetupQuizVisualsAndCollision() override;

private:
    // 2지선다 고정 값
    const int32 NumEntrances = 2;
};