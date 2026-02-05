#include "QuizObstacleBase.h"
#include "Components/TextRenderComponent.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

// =============================================================
// 1. 초기화 및 복제 설정 (Constructor & Replication)
// =============================================================

AQuizObstacleBase::AQuizObstacleBase()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = false; // 초기에는 틱을 꺼서 최적화

    bReplicates = true;
    bAlwaysRelevant = true; // 멀티플레이어 환경에서 항상 중요 액터로 취급
    SetReplicateMovement(true);

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = Root;

    // 4방향 충돌 트리거 생성
    Trigger_1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger_1")); Trigger_1->SetupAttachment(Root);
    Trigger_2 = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger_2")); Trigger_2->SetupAttachment(Root);
    Trigger_3 = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger_3")); Trigger_3->SetupAttachment(Root);
    Trigger_4 = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger_4")); Trigger_4->SetupAttachment(Root);

    // 퀴즈 텍스트 구성
    CategoryText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("CategoryText"));
    CategoryText->SetupAttachment(Root);
    CategoryText->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
    CategoryText->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
    CategoryText->SetTextRenderColor(FColor::Yellow);

    QuestionText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("QuestionText"));
    QuestionText->SetupAttachment(Root);
    QuestionText->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
    QuestionText->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);

    MoveSpeed = 0.0f;
    ObstacleState = EObstacleState::Falling;
}

void AQuizObstacleBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AQuizObstacleBase, CurrentQuizData);
    DOREPLIFETIME(AQuizObstacleBase, MoveSpeed);
    DOREPLIFETIME(AQuizObstacleBase, ObstacleState);
}

// =============================================================
// 2. 수명 주기 (Lifecycle)
// =============================================================

void AQuizObstacleBase::BeginPlay()
{
    Super::BeginPlay();
    // 클라이언트에서 늦게 생성되었을 때를 대비한 수동 호출
    if (!HasAuthority() && !CurrentQuizData.Question.IsEmpty())
    {
        OnRep_CurrentQuizData();
    }
}

void AQuizObstacleBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 현재 상태에 따른 행동 수행 (FSM)
    switch (ObstacleState)
    {
    case EObstacleState::Falling:
        HandleFalling(DeltaTime);
        break;
    case EObstacleState::Moving:
        HandleMoving(DeltaTime);
        break;
    default:
        break;
    }
}

// =============================================================
// 3. 상태 관리 및 동작 로직 (State Logic)
// =============================================================

void AQuizObstacleBase::InitializeObstacle(const FQuizData& NewQuizData, float NewMoveSpeed)
{
    if (HasAuthority())
    {
        CurrentQuizData = NewQuizData;
        MoveSpeed = NewMoveSpeed;

        TargetZ = GetActorLocation().Z;

        // 생성 위치를 하늘 높이로 오프셋 적용
        FVector NewLoc = GetActorLocation();
        NewLoc.Z += SpawnHeightOffset;
        SetActorLocation(NewLoc);

        ForceNetUpdate(); // 즉시 클라이언트에 전파

        CurrentVerticalVelocity = 0.0f;
        ObstacleState = EObstacleState::Falling;
        SetActorTickEnabled(true);

        OnRep_CurrentQuizData();
        OnRep_ObstacleState();
    }
}

void AQuizObstacleBase::HandleFalling(float DeltaTime)
{
    // 등가속도 운동 적용
    float Gravity = GetWorld()->GetGravityZ() * GravityScale;
    CurrentVerticalVelocity += Gravity * DeltaTime;

    FVector Loc = GetActorLocation();
    Loc.Z += CurrentVerticalVelocity * DeltaTime;

    // 착지 시퀀스 진입 판정
    if (Loc.Z <= TargetZ)
    {
        Loc.Z = TargetZ;
        SetActorLocation(Loc);

        if (ObstacleState == EObstacleState::Falling)
        {
            ExecuteLandingSequence();
        }

        if (HasAuthority())
        {
            ObstacleState = EObstacleState::Landing;

            // 착지 후 딜레이 대기 후 이동 상태로 전환
            GetWorldTimerManager().SetTimer(TimerHandle_Landing, [this]() {
                ObstacleState = EObstacleState::Moving;
                OnRep_ObstacleState();
                }, LandingDelay, false);
        }
    }
    else
    {
        SetActorLocation(Loc);
    }
}

void AQuizObstacleBase::ExecuteLandingSequence()
{
    // 착지 연출 (먼지, 소리, 카메라 흔들림)
    if (LandingParticle)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), LandingParticle, GetActorLocation());
    }

    if (LandingSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, LandingSound, GetActorLocation());
    }

    if (LandingCameraShake)
    {
        UGameplayStatics::PlayWorldCameraShake(
            GetWorld(), LandingCameraShake, GetActorLocation(), ShakeInnerRadius, ShakeOuterRadius
        );
    }

    CurrentVerticalVelocity = 0.0f;
}

void AQuizObstacleBase::HandleMoving(float DeltaTime)
{
    if (MoveSpeed > 0.f)
    {
        // 앞에 있는 캐릭터를 부드럽게 밀어냄
        PushOverlappingCharacters(DeltaTime);
        AddActorWorldOffset(GetActorForwardVector() * MoveSpeed * DeltaTime, true);
    }
}

// =============================================================
// 4. 네트워크 동기화 응답 (RepNotify)
// =============================================================

void AQuizObstacleBase::OnRep_ObstacleState()
{
    if (ObstacleState == EObstacleState::Landing)
    {
        ExecuteLandingSequence();
    }

    // 착지 고정 상태(Landing)일 때는 연산을 잠시 멈춤
    SetActorTickEnabled(ObstacleState != EObstacleState::Landing);
}

void AQuizObstacleBase::OnRep_CurrentQuizData()
{
    if (CategoryText)
    {
        FString CatStr = CurrentQuizData.Category.IsEmpty() ? TEXT("Quiz") : CurrentQuizData.Category;
        CategoryText->SetWorldSize(15.0f);
        CategoryText->SetText(FText::FromString(CatStr));
    }

    if (QuestionText)
    {
        FString QStr = CurrentQuizData.Question.ToString();
        float NewSize = CalculateFontSize(QStr.Len(), QuestionMaxSize, QuestionMinSize);
        QuestionText->SetWorldSize(NewSize);
        int32 LineLen = (NewSize < 20.0f) ? 25 : 15;
        QuestionText->SetText(FText::FromString(AddLineBreaksToText(QStr, LineLen)));
    }

    if (ObstacleState == EObstacleState::Falling)
    {
        SetActorTickEnabled(true);
    }

    SetupQuizVisualsAndCollision();
}

// =============================================================
// 5. 헬퍼 및 가공 로직 (Helpers)
// =============================================================

float AQuizObstacleBase::CalculateFontSize(int32 TextLength, float MaxSize, float MinSize)
{
    // 텍스트 길이에 따라 폰트 크기를 선형 보간함
    return FMath::GetMappedRangeValueClamped(FVector2D(2.f, 6.f), FVector2D(MaxSize, MinSize), (float)TextLength);
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

        if (CurrentLength >= MaxLineLength && LastSpaceIndex != -1)
        {
            Result[LastSpaceIndex] = '\n';
            CurrentLength = i - LastSpaceIndex;
            LastSpaceIndex = -1;
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
        if (Char && (HasAuthority() || Char->IsLocallyControlled())) {
            UCharacterMovementComponent* CMC = Char->GetCharacterMovement();
            if (!CMC) continue;

            // 캐릭터 오프셋 강제 이동 및 속도 동기화
            Char->AddActorWorldOffset(PushDelta, true);
            CMC->Velocity.X = WallVelocity.X;
            CMC->Velocity.Y = WallVelocity.Y;
        }
    }
}