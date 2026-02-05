#include "FruitGame/InteractableFruitObject.h"
#include "Net/UnrealNetwork.h"

// =============================================================
// 1. 초기 생성 및 컴포넌트 설정
// =============================================================

AInteractableFruitObject::AInteractableFruitObject()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;                    // 네트워크 복제 활성화

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    MeshComponent->SetCollisionObjectType(ECC_Visibility);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

    GuessIndex = 0;
    CurrentFruit = EFruitType::FT_Apple;
}

void AInteractableFruitObject::BeginPlay()
{
    Super::BeginPlay();
    // 시작 시 서버/클라이언트 모두 현재 과일에 맞는 머티리얼 적용
    OnRep_CurrentFruit();
}

// =============================================================
// 2. 네트워크 복제 및 상태 관리
// =============================================================

void AInteractableFruitObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // CurrentFruit 변수가 변경되면 모든 클라이언트에게 전송
    DOREPLIFETIME(AInteractableFruitObject, CurrentFruit);
}

// =============================================================
// 3. 핵심 게임 로직 (Fruit Cycling)
// =============================================================

EFruitType AInteractableFruitObject::CycleFruit()
{
    // 서버에서만 로직 실행 가능
    if (!HasAuthority())
    {
        return CurrentFruit;
    }

    // 과일 타입을 숫자로 변환하여 순환 로직 계산
    uint8 CurrentValue = static_cast<uint8>(CurrentFruit);

    // 0(None)이거나 4(Orange) 이상인 경우 1(Apple)로 초기화
    if (CurrentValue == 0 || CurrentValue >= 4)
    {
        CurrentValue = 1;
    }
    else
    {
        // 그 외엔 다음 단계 과일로 증가 (Apple -> Banana -> Cherry -> Orange)
        CurrentValue++;
    }

    CurrentFruit = static_cast<EFruitType>(CurrentValue);

    // 서버는 RepNotify가 자동으로 호출되지 않으므로 수동으로 머티리얼 갱신
    OnRep_CurrentFruit();

    return CurrentFruit;
}

// =============================================================
// 4. 비주얼 업데이트 (Visual Update)
// =============================================================

void AInteractableFruitObject::OnRep_CurrentFruit()
{
    // 데이터 맵에서 현재 과일 타입에 맞는 머티리얼을 찾아 메쉬에 적용
    if (MeshComponent)
    {
        UMaterialInterface** FoundMaterial = FruitMaterials.Find(CurrentFruit);
        if (FoundMaterial)
        {
            MeshComponent->SetMaterial(0, *FoundMaterial);
        }
    }
}