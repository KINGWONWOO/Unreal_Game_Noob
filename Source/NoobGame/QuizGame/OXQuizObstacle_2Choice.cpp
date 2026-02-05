#include "OXQuizObstacle_2Choice.h"
#include "Components/TextRenderComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

// =============================================================
// 1. 초기화 및 컴포넌트 생성 (Constructor)
// =============================================================

AOXQuizObstacle_2Choice::AOXQuizObstacle_2Choice()
{
    // 배열 크기 예약
    EntranceRoots.SetNum(NumEntrances);
    EntranceMeshes.SetNum(NumEntrances);
    EntranceCollisions.SetNum(NumEntrances);
    EntranceAnswerTexts.SetNum(NumEntrances);

    // 루프를 돌며 각 입구에 필요한 컴포넌트들을 동적으로 생성 및 부착
    for (int32 i = 0; i < NumEntrances; ++i)
    {
        FString IdxStr = FString::FromInt(i);

        EntranceRoots[i] = CreateDefaultSubobject<USceneComponent>(*FString::Printf(TEXT("EntranceRoot_%d"), i));
        EntranceRoots[i]->SetupAttachment(RootComponent);

        EntranceMeshes[i] = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("EntranceMesh_%d"), i));
        EntranceMeshes[i]->SetupAttachment(EntranceRoots[i]);

        EntranceCollisions[i] = CreateDefaultSubobject<UBoxComponent>(*FString::Printf(TEXT("EntranceCollision_%d"), i));
        EntranceCollisions[i]->SetupAttachment(EntranceRoots[i]);
        EntranceCollisions[i]->SetCollisionProfileName(TEXT("BlockAllDynamic"));

        EntranceAnswerTexts[i] = CreateDefaultSubobject<UTextRenderComponent>(*FString::Printf(TEXT("EntranceAnswerText_%d"), i));
        EntranceAnswerTexts[i]->SetupAttachment(EntranceRoots[i]);
    }
}

// =============================================================
// 2. 퀴즈 데이터 시각화 및 물리 설정 (Visuals & Collision)
// =============================================================

void AOXQuizObstacle_2Choice::SetupQuizVisualsAndCollision()
{
    // [1] 질문 텍스트 업데이트
    if (QuestionText)
    {
        FString QStr = CurrentQuizData.Question.ToString();

        // 부모의 헬퍼 함수를 사용하여 길이에 따른 폰트 크기 및 줄바꿈 계산
        float NewSize = CalculateFontSize(QStr.Len(), QuestionMaxSize, QuestionMinSize);
        QuestionText->SetWorldSize(NewSize);

        int32 LineLen = (NewSize < (QuestionMaxSize + QuestionMinSize) * 0.5f) ? 25 : 15;
        QuestionText->SetText(FText::FromString(AddLineBreaksToText(QStr, LineLen)));
    }

    // [2] 선택지 텍스트 및 정답 구역 충돌 설정
    int32 NumAnswers = CurrentQuizData.Answers.Num();
    if (NumAnswers > NumEntrances) NumAnswers = NumEntrances;

    for (int32 i = 0; i < NumEntrances; ++i)
    {
        if (i < NumAnswers)
        {
            FString AnsStr = CurrentQuizData.Answers[i].ToString();

            // 선택지 텍스트 설정
            float NewSize = CalculateFontSize(AnsStr.Len(), AnswerMaxSize, AnswerMinSize);
            EntranceAnswerTexts[i]->SetWorldSize(NewSize);

            int32 LineLen = (NewSize < 20.0f) ? 15 : 10;
            EntranceAnswerTexts[i]->SetText(FText::FromString(AddLineBreaksToText(AnsStr, LineLen)));
            EntranceAnswerTexts[i]->SetVisibility(true);

            // 정답 여부에 따른 충돌 처리 로직
            if (i == CurrentQuizData.CorrectAnswerIndex)
            {
                // 정답인 경우: 플레이어가 통과할 수 있도록 콜리전 제거
                EntranceCollisions[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
                EntranceMeshes[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            }
            else
            {
                // 오답인 경우: 플레이어를 막기 위해 물리 충돌 활성화
                EntranceMeshes[i]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
                EntranceCollisions[i]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
                EntranceCollisions[i]->SetCollisionResponseToAllChannels(ECR_Block);
            }
        }
        else
        {
            // 데이터가 없는 입구는 텍스트를 숨기고 충돌체 활성화
            EntranceAnswerTexts[i]->SetVisibility(false);
            EntranceCollisions[i]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        }
    }
}