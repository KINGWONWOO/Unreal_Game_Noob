// OXQuizObstacle_3Choice.cpp

#include "OXQuizObstacle_3Choice.h" // 자신의 헤더 (반드시 처음)

// .cpp에서는 전체 헤더 포함
#include "Components/TextRenderComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

AOXQuizObstacle_3Choice::AOXQuizObstacle_3Choice()
{
    EntranceRoots.SetNum(NumEntrances);
    EntranceMeshes.SetNum(NumEntrances);
    EntranceCollisions.SetNum(NumEntrances);
    EntranceAnswerTexts.SetNum(NumEntrances);

    for (int32 i = 0; i < NumEntrances; ++i)
    {
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

void AOXQuizObstacle_3Choice::SetupQuizVisualsAndCollision()
{
    // 1. 공통 질문 텍스트 설정 (부모의 Helper 함수 사용!)
    if (QuestionText)
    {
        FString QStr = CurrentQuizData.Question.ToString();

        // 크기 계산
        float NewSize = CalculateFontSize(QStr.Len(), QuestionMaxSize, QuestionMinSize);
        QuestionText->SetWorldSize(NewSize);

        // 줄바꿈 (크기가 작으면 한 줄 허용량을 늘림)
        int32 LineLen = (NewSize < (QuestionMaxSize + QuestionMinSize) * 0.5f) ? 25 : 15;
        QuestionText->SetText(FText::FromString(AddLineBreaksToText(QStr, LineLen)));
    }

    // 2. 선택지 설정
    int32 NumAnswers = CurrentQuizData.Answers.Num();
    if (NumAnswers > NumEntrances) NumAnswers = NumEntrances;

    for (int32 i = 0; i < NumEntrances; ++i)
    {
        if (i < NumAnswers)
        {
            FString AnsStr = CurrentQuizData.Answers[i].ToString();

            // 크기 계산
            float NewSize = CalculateFontSize(AnsStr.Len(), AnswerMaxSize, AnswerMinSize);
            EntranceAnswerTexts[i]->SetWorldSize(NewSize);

            // 줄바꿈
            int32 LineLen = (NewSize < 20.0f) ? 15 : 10;
            EntranceAnswerTexts[i]->SetText(FText::FromString(AddLineBreaksToText(AnsStr, LineLen)));
            EntranceAnswerTexts[i]->SetVisibility(true);

            // 충돌 처리 (정답: 통과, 오답: 막힘)
            if (i == CurrentQuizData.CorrectAnswerIndex)
            {
                EntranceCollisions[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            }
            else
            {
                EntranceCollisions[i]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
                EntranceCollisions[i]->SetCollisionResponseToAllChannels(ECR_Block);
            }
        }
        else
        {
            EntranceAnswerTexts[i]->SetVisibility(false);
            EntranceCollisions[i]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 데이터 없으면 막음
        }
    }
}