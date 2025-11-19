#include "QuizObstacleBase.h"
#include "Components/TextRenderComponent.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

AQuizObstacleBase::AQuizObstacleBase()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = false;

    bReplicates = true;
    bAlwaysRelevant = true;
    SetReplicateMovement(false);

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = Root;

    Trigger_1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger_1")); Trigger_1->SetupAttachment(Root);
    Trigger_2 = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger_2")); Trigger_2->SetupAttachment(Root);
    Trigger_3 = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger_3")); Trigger_3->SetupAttachment(Root);
    Trigger_4 = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger_4")); Trigger_4->SetupAttachment(Root);

    // [New] 카테고리 텍스트 생성
    CategoryText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("CategoryText"));
    CategoryText->SetupAttachment(Root);
    CategoryText->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
    CategoryText->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
    CategoryText->SetTextRenderColor(FColor::Yellow); // 카테고리는 노란색으로 강조

    // 질문 텍스트 생성
    QuestionText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("QuestionText"));
    QuestionText->SetupAttachment(Root);
    QuestionText->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
    QuestionText->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);

    MoveSpeed = 0.0f;
    bIsMoving = false;
}

void AQuizObstacleBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AQuizObstacleBase, CurrentQuizData);
    DOREPLIFETIME(AQuizObstacleBase, MoveSpeed);
    DOREPLIFETIME(AQuizObstacleBase, bIsMoving);
}

void AQuizObstacleBase::BeginPlay()
{
    Super::BeginPlay();
    if (!HasAuthority() && !CurrentQuizData.Question.IsEmpty())
    {
        OnRep_CurrentQuizData();
    }
}

void AQuizObstacleBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bIsMoving && MoveSpeed > 0.f)
    {
        PushOverlappingCharacters(DeltaTime);
        AddActorWorldOffset(GetActorForwardVector() * MoveSpeed * DeltaTime, true);
    }
}

void AQuizObstacleBase::InitializeObstacle(const FQuizData& NewQuizData, float NewMoveSpeed)
{
    if (HasAuthority())
    {
        CurrentQuizData = NewQuizData;
        MoveSpeed = NewMoveSpeed;
        bIsMoving = true;
        OnRep_IsMoving();
        OnRep_CurrentQuizData();
    }
}

void AQuizObstacleBase::OnRep_IsMoving()
{
    SetActorTickEnabled(bIsMoving);
}

void AQuizObstacleBase::OnRep_CurrentQuizData()
{
    // 1. [수정됨] 카테고리 텍스트 업데이트 (크기 10 고정)
    if (CategoryText)
    {
        FString CatStr = CurrentQuizData.Category.IsEmpty() ? TEXT("Quiz") : CurrentQuizData.Category;

        // 기존: 계산된 크기 사용
        // float NewSize = CalculateFontSize(CatStr.Len(), CategoryMaxSize, CategoryMinSize);

        // [변경] 요청하신 대로 10.0f로 고정
        CategoryText->SetWorldSize(15.0f);
        CategoryText->SetText(FText::FromString(CatStr));
    }

    // 2. 질문 텍스트 업데이트 (공통)
    if (QuestionText)
    {
        FString QStr = CurrentQuizData.Question.ToString();

        float NewSize = CalculateFontSize(QStr.Len(), QuestionMaxSize, QuestionMinSize);
        QuestionText->SetWorldSize(NewSize);

        // 글자가 작으면(20이하) 한 줄에 25자, 크면 15자
        int32 LineLen = (NewSize < 20.0f) ? 25 : 15;
        QuestionText->SetText(FText::FromString(AddLineBreaksToText(QStr, LineLen)));
    }

    // 3. 정답(선택지) 업데이트는 자식 클래스에게 위임
    SetupQuizVisualsAndCollision();
}

// --- Helpers ---

float AQuizObstacleBase::CalculateFontSize(int32 TextLength, float MaxSize, float MinSize)
{
    const float ShortLen = 1.0f;
    const float LongLen = 5.0f;

    // FMath::GetMappedRangeValueClamped를 사용하여 반대로 작동하는 실수 방지
    // 글자수 5 -> MaxSize, 글자수 30 -> MinSize
    return FMath::GetMappedRangeValueClamped(
        FVector2D(ShortLen, LongLen),
        FVector2D(MaxSize, MinSize),
        (float)TextLength
    );
}

FString AQuizObstacleBase::AddLineBreaksToText(FString InText, int32 MaxLineLength)
{
    if (InText.IsEmpty() || MaxLineLength <= 0) return InText;

    FString Result = InText;
    int32 CurrentLength = 0;
    int32 LastSpaceIndex = -1;

    for (int32 i = 0; i < Result.Len(); i++)
    {
        CurrentLength++;
        if (Result[i] == ' ') LastSpaceIndex = i;
        else if (Result[i] == '\n') { CurrentLength = 0; LastSpaceIndex = -1; }

        if (CurrentLength >= MaxLineLength)
        {
            if (LastSpaceIndex != -1)
            {
                Result[LastSpaceIndex] = '\n';
                CurrentLength = i - LastSpaceIndex;
                LastSpaceIndex = -1;
            }
        }
    }
    return Result;
}

void AQuizObstacleBase::PushOverlappingCharacters(float DeltaTime)
{
    TSet<AActor*> UniqueActors;
    TArray<UBoxComponent*> AllTriggers = { Trigger_1, Trigger_2, Trigger_3, Trigger_4 };
    for (UBoxComponent* Trigger : AllTriggers) {
        if (Trigger) {
            TArray<AActor*> TempActors;
            Trigger->GetOverlappingActors(TempActors, ACharacter::StaticClass());
            for (AActor* Actor : TempActors) UniqueActors.Add(Actor);
        }
    }
    if (UniqueActors.Num() == 0) return;

    const FVector WallVelocity = GetActorForwardVector() * MoveSpeed;
    FVector PushDelta = WallVelocity * DeltaTime;
    PushDelta.Z = 0.0f;

    for (AActor* Actor : UniqueActors) {
        ACharacter* Char = Cast<ACharacter>(Actor);
        if (Char) {
            bool bIsServer = HasAuthority();
            bool bIsLocalPlayer = Char->IsLocallyControlled();
            if (bIsServer || bIsLocalPlayer) {
                UCharacterMovementComponent* CMC = Char->GetCharacterMovement();
                if (!CMC) continue;
                Char->AddActorWorldOffset(PushDelta, true);
                FVector NewVelocity = WallVelocity;
                NewVelocity.Z = CMC->Velocity.Z;
                CMC->Velocity = NewVelocity;
                if (CMC->MovementMode == MOVE_Walking) {
                    CMC->FindFloor(Char->GetActorLocation(), CMC->CurrentFloor, true, NULL);
                    CMC->SetMovementMode(MOVE_Walking);
                }
                CMC->AddInputVector(WallVelocity.GetSafeNormal());
            }
        }
    }
}