#include "OXQuizObstacle_3Choice.h"
#include "Components/TextRenderComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

// =============================================================
// 1. 초기화 및 컴포넌트 동적 생성 (Constructor)
// =============================================================

AOXQuizObstacle_3Choice::AOXQuizObstacle_3Choice()
{
    // 배열 크기 선행 할당
    EntranceRoots.SetNum(NumEntrances);
    EntranceMeshes.SetNum(NumEntrances);
    EntranceCollisions.SetNum(NumEntrances);
    EntranceAnswerTexts.SetNum(NumEntrances);

    // 3개의 입구를 루프를 돌며 생성 및 부착
    for (int32 i = 0; i < NumEntrances; ++i)
    {
        // 1. 기준점 생성
        EntranceRoots[i] = CreateDefaultSubobject<USceneComponent>(*FString::Printf(TEXT("EntranceRoot_%d"), i));
        EntranceRoots[i]->SetupAttachment(RootComponent);

        // 2. 메쉬(벽) 생성
        EntranceMeshes[i] = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("EntranceMesh_%d"), i));
        EntranceMeshes[i]->SetupAttachment(EntranceRoots[i]);

        // 3. 콜리전(충돌체) 생성 및 기본 프로필 설정
        EntranceCollisions[i] = CreateDefaultSubobject<UBoxComponent>(*FString::Printf(TEXT("EntranceCollision_%d"), i));
        EntranceCollisions[i]->SetupAttachment(EntranceRoots[i]);
        EntranceCollisions[i]->SetCollisionProfileName(TEXT("BlockAllDynamic"));

        // 4. 선택지 텍스트 렌더러 생성
        EntranceAnswerTexts[i] = CreateDefaultSubobject<UTextRenderComponent>(*FString::Printf(TEXT("EntranceAnswerText_%d"), i));
        EntranceAnswerTexts[i]->SetupAttachment(EntranceRoots[i]);
    }
}

// =============================================================
// 2. 비주얼 업데이트 및 정답 로직 적용 (Visual & Collision)
// =============================================================

void AOXQuizObstacle_3Choice::SetupQuizVisualsAndCollision()
{
    // [1] 공통 질문 텍스트 설정
    if (QuestionText)
    {
        FString QStr = CurrentQuizData.Question.ToString();

        // 부모의 Helper 함수를 활용해 길이에 따른 폰트 크기 동적 계산
        float NewSize = CalculateFontSize(QStr.Len(), QuestionMaxSize, QuestionMinSize);
        QuestionText->SetWorldSize(NewSize);

        // 텍스트 줄바꿈 처리 (폰트 크기가 작을수록 한 줄에 더 많은 글자 허용)
        int32 LineLen = (NewSize < (QuestionMaxSize + QuestionMinSize) * 0.5f) ? 25 : 15;
        QuestionText->SetText(FText::FromString(AddLineBreaksToText(QStr, LineLen)));
    }

    // [2] 3개 선택지 텍스트 및 충돌 판정 설정
    int32 NumAnswers = CurrentQuizData.Answers.Num();
    if (NumAnswers > NumEntrances) NumAnswers = NumEntrances;

    for (int32 i = 0; i < NumEntrances; ++i)
    {
        // 유효한 선택지 데이터가 있는 경우
        if (i < NumAnswers)
        {
            FString AnsStr = CurrentQuizData.Answers[i].ToString();

            // 선택지 폰트 크기 및 줄바꿈 설정
            float NewSize = CalculateFontSize(AnsStr.Len(), AnswerMaxSize, AnswerMinSize);
            EntranceAnswerTexts[i]->SetWorldSize(NewSize);

            int32 LineLen = (NewSize < 20.0f) ? 15 : 10;
            EntranceAnswerTexts[i]->SetText(FText::FromString(AddLineBreaksToText(AnsStr, LineLen)));
            EntranceAnswerTexts[i]->SetVisibility(true);

            // 정답 여부에 따른 물리 충돌 제어
            if (i == CurrentQuizData.CorrectAnswerIndex)
            {
                // 정답: 벽과 충돌체를 모두 비활성화하여 플레이어 통과 허용
                EntranceMeshes[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
                EntranceCollisions[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            }
            else
            {
                // 오답: 물리 충돌을 활성화하여 플레이어의 진입을 차단
                EntranceMeshes[i]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
                EntranceCollisions[i]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
                EntranceCollisions[i]->SetCollisionResponseToAllChannels(ECR_Block);
            }
        }
        else
        {
            // 데이터가 없는 입구는 텍스트를 숨기고 충돌체는 기본적으로 막아둠
            EntranceAnswerTexts[i]->SetVisibility(false);
            EntranceCollisions[i]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        }
    }
}