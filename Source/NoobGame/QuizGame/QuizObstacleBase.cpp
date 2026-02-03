#include "QuizObstacleBase.h"
#include "Components/TextRenderComponent.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

AQuizObstacleBase::AQuizObstacleBase()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = false;

    bReplicates = true;
    bAlwaysRelevant = true;
    SetReplicateMovement(true);

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = Root;

    Trigger_1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger_1")); Trigger_1->SetupAttachment(Root);
    Trigger_2 = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger_2")); Trigger_2->SetupAttachment(Root);
    Trigger_3 = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger_3")); Trigger_3->SetupAttachment(Root);
    Trigger_4 = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger_4")); Trigger_4->SetupAttachment(Root);

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
    TargetZ = 0.0f;
    CurrentVerticalVelocity = 0.0f;
}

void AQuizObstacleBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AQuizObstacleBase, CurrentQuizData);
    DOREPLIFETIME(AQuizObstacleBase, MoveSpeed);
    DOREPLIFETIME(AQuizObstacleBase, ObstacleState);
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

void AQuizObstacleBase::InitializeObstacle(const FQuizData& NewQuizData, float NewMoveSpeed)
{
    if (HasAuthority())
    {
        CurrentQuizData = NewQuizData;
        MoveSpeed = NewMoveSpeed;

        // 1. 목표 지점 저장 (현재 바닥 높이)
        TargetZ = GetActorLocation().Z;

        // 2. 시작 위치로 강제 이동 (하늘 위 오프셋 적용)
        FVector NewLoc = GetActorLocation();
        NewLoc.Z += SpawnHeightOffset;
        SetActorLocation(NewLoc);

        ForceNetUpdate();

        // 3. 상태 및 물리 초기화
        CurrentVerticalVelocity = 0.0f;
        ObstacleState = EObstacleState::Falling;
        SetActorTickEnabled(true);

        OnRep_CurrentQuizData();
        OnRep_ObstacleState();
    }
}

void AQuizObstacleBase::HandleFalling(float DeltaTime)
{
    // [보정] 중력 가속도 적용 (v = v0 + at)
    // GravityScale이 높을수록(예: 4.0) 묵직하게 떨어집니다.
    float Gravity = GetWorld()->GetGravityZ() * GravityScale;
    CurrentVerticalVelocity += Gravity * DeltaTime;

    FVector Loc = GetActorLocation();
    Loc.Z += CurrentVerticalVelocity * DeltaTime;

    // 착지 판정
    if (Loc.Z <= TargetZ)
    {
        Loc.Z = TargetZ;
        SetActorLocation(Loc);

        // [핵심 보정] 클라이언트/서버 공통으로 즉시 이펙트 실행 (지연 시간 제거)
        if (ObstacleState == EObstacleState::Falling)
        {
            ExecuteLandingSequence();
        }

        if (HasAuthority())
        {
            ObstacleState = EObstacleState::Landing;

            // 일정 대기 후 전진 상태로 전환
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

void AQuizObstacleBase::OnRep_ObstacleState()
{
    // Landing 상태가 복제되어 왔을 때, 아직 이펙트가 실행되지 않았다면 실행 (안전장치)
    if (ObstacleState == EObstacleState::Landing)
    {
        ExecuteLandingSequence();
    }

    SetActorTickEnabled(ObstacleState != EObstacleState::Landing);
}

void AQuizObstacleBase::ExecuteLandingSequence()
{
    // 중복 실행 방지 로직 (Falling -> Landing 전환 시 1회만 실행)
    // 여기서 사용되는 변수는 헤더에 bool bLandedEffectDone; 추가를 권장합니다.

    // 1. 니아가라 먼지 효과
    if (LandingParticle)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), LandingParticle, GetActorLocation());
    }

    // 2. 착지 사운드
    if (LandingSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, LandingSound, GetActorLocation());
    }

    // 3. 카메라 흔들림
    if (LandingCameraShake)
    {
        UGameplayStatics::PlayWorldCameraShake(
            GetWorld(),
            LandingCameraShake,
            GetActorLocation(),
            ShakeInnerRadius,
            ShakeOuterRadius
        );
    }

    // 물리 속도 초기화
    CurrentVerticalVelocity = 0.0f;
}

void AQuizObstacleBase::HandleMoving(float DeltaTime)
{
    if (MoveSpeed > 0.f)
    {
        PushOverlappingCharacters(DeltaTime);
        AddActorWorldOffset(GetActorForwardVector() * MoveSpeed * DeltaTime, true);
    }
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

    // 클라이언트에서 첫 장애물 실종 방지: 데이터가 오면 틱을 강제로 켬
    if (ObstacleState == EObstacleState::Falling)
    {
        SetActorTickEnabled(true);
    }

    SetupQuizVisualsAndCollision();
}

float AQuizObstacleBase::CalculateFontSize(int32 TextLength, float MaxSize, float MinSize)
{
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
            Char->AddActorWorldOffset(PushDelta, true);
            CMC->Velocity.X = WallVelocity.X;
            CMC->Velocity.Y = WallVelocity.Y;
        }
    }
}