#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameTypes.h"
#include "InteractableFruitObject.generated.h"

UCLASS()
class NOOBGAME_API AInteractableFruitObject : public AActor
{
    GENERATED_BODY()

public:
    AInteractableFruitObject();

    // 네트워크 복제 설정
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // 게임 로직 관련 변수
    // 에디터의 레벨 배치 뷰에서 직접 설정하는 정답 배열 인덱스 (0~4)
    UPROPERTY(EditInstanceOnly, Category = "Fruit Game")
    int32 GuessIndex;

    // 현재 오브젝트가 상태 (서버에서 관리하며 값이 변하면 OnRep 함수 실행)
    UPROPERTY(ReplicatedUsing = OnRep_CurrentFruit)
    EFruitType CurrentFruit;

    // 외형 및 컴포넌트 설정
    // 과일 타입별 머티리얼을 매칭하기 위한 데이터 맵
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fruit Game")
    TMap<EFruitType, UMaterialInterface*> FruitMaterials;

    // 실제 화면에 보여질 메쉬 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    // 주요 기능 함수
    // 서버 전용: 호출 시 다음 과일 종류로 순환시킴
    UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "Fruit Game")
    EFruitType CycleFruit();

protected:
    // 게임 시작 시 초기화
    virtual void BeginPlay() override;

    // 클라이언트 응답 함수
    // 서버로부터 과일 타입 변수를 받았을 때 머티리얼을 실제 변경함
    UFUNCTION()
    void OnRep_CurrentFruit();
};