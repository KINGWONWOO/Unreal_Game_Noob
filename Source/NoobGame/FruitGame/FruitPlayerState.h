#pragma once

#include "CoreMinimal.h"
#include "NoobPlayerState.h" 
#include "GameTypes.h"       
#include "FruitPlayerState.generated.h"

UCLASS()
class NOOBGAME_API AFruitPlayerState : public ANoobPlayerState
{
    GENERATED_BODY()

public:
    AFruitPlayerState();

    //네트워크 복제 설정
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    //데이터 접근 API (Getters/Setters)
    void SetSecretAnswers_Server(const TArray<EFruitType>& SecretFruits);
    const TArray<EFruitType>& GetSecretAnswers_Server() const;

    //동기화 변수 (Replicated Properties)

    //Setup 단계 과일 제출 완료 여부
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Fruit Game")
    bool bHasSubmittedFruits;

    //이 플레이어의 비밀 정답 (서버 저장 및 복제)
    UPROPERTY(Replicated)
    TArray<EFruitType> SecretAnswers;
};