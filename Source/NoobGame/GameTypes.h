#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameTypes.generated.h"

// =============================================================
// 0. 공용 타입 (Common Types)
// =============================================================

// 프로젝트 전체에서 사용되는 캐릭터 종류
UENUM(BlueprintType)
enum class ECharacterType : uint8
{
    ECT_None,
    ECT_Cat     UMETA(DisplayName = "Cat"),
    ECT_Dog     UMETA(DisplayName = "Dog")
};

// =============================================================
// 1. 과일 게임 타입 (Fruit Game Types)
// =============================================================

// 게임에 등장하는 과일의 종류
UENUM(BlueprintType)
enum class EFruitType : uint8
{
    FT_None     UMETA(DisplayName = "None"),
    FT_Apple    UMETA(DisplayName = "Apple"),
    FT_Banana   UMETA(DisplayName = "Banana"),
    FT_Cherry   UMETA(DisplayName = "Cherry"),
    FT_Orange   UMETA(DisplayName = "Orange")
};

// 과일 게임의 진행 단계
UENUM(BlueprintType)
enum class EFruitGamePhase : uint8
{
    GP_WaitingToStart   UMETA(DisplayName = "WaitingToStart"), // 2명 대기 중
    GP_Instructions     UMETA(DisplayName = "Instructions"),   // 게임 설명 단계
    GP_Setup            UMETA(DisplayName = "Setup"),          // 각자 과일 설정 단계
    GP_SpinnerTurn      UMETA(DisplayName = "SpinnerTurn"),    // 선공 결정을 위한 스피너
    GP_PlayerTurn       UMETA(DisplayName = "PlayerTurn"),     // 메인 턴 진행
    GP_GameOver         UMETA(DisplayName = "GameOver")        // 게임 종료
};

// =============================================================
// 2. OX 퀴즈 게임 타입 (OX Quiz Game Types)
// =============================================================

// 퀴즈 난이도 설정
UENUM(BlueprintType)
enum class EQuizDifficulty : uint8
{
    Easy    UMETA(DisplayName = "Easy (하)"),
    Medium  UMETA(DisplayName = "Medium (중)"),
    Hard    UMETA(DisplayName = "Hard (상)")
};

// 퀴즈 게임의 진행 단계
UENUM(BlueprintType)
enum class EQuizGamePhase : uint8
{
    GP_WaitingToStart   UMETA(DisplayName = "WaitingToStart"),
    GP_Instructions     UMETA(DisplayName = "Instructions"),
    GP_Playing           UMETA(DisplayName = "Playing"),
    GP_GameOver          UMETA(DisplayName = "GameOver")
};

// 퀴즈 문제의 카테고리 정의 (데이터 테이블 및 JSON 매칭용)
UENUM(BlueprintType)
enum class EQuizCategory : uint8
{
    None            UMETA(DisplayName = "없음"),
    Science         UMETA(DisplayName = "과학"),
    History         UMETA(DisplayName = "역사"),
    IT              UMETA(DisplayName = "IT"),
    CommonSense     UMETA(DisplayName = "상식"),
    Biology         UMETA(DisplayName = "생물"),
    Geography       UMETA(DisplayName = "지리"),
    Sports          UMETA(DisplayName = "스포츠"),
    Art             UMETA(DisplayName = "예술"),
    Math            UMETA(DisplayName = "수학"),
    Culture         UMETA(DisplayName = "문화"),
    Literature      UMETA(DisplayName = "문학"),
    Economy         UMETA(DisplayName = "경제"),
    Environment     UMETA(DisplayName = "환경"),
    Technology      UMETA(DisplayName = "기술"),
    Entertainment   UMETA(DisplayName = "엔터")
};

// 퀴즈 데이터 테이블 구조체
USTRUCT(BlueprintType)
struct FQuizData : public FTableRowBase
{
    GENERATED_BODY()

    // 퀴즈 문제 텍스트
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quiz")
    FText Question;

    // 선택지 배열 (2개면 OX, 3개면 3지선다)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quiz")
    TArray<FText> Answers;

    // 정답 배열의 인덱스
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quiz")
    int32 CorrectAnswerIndex;

    // 문제 난이도
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quiz")
    EQuizDifficulty Difficulty;

    // 카테고리 명칭
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quiz")
    FString Category;

    // 문제 유형 구분 (OX, 3Choice 등)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quiz")
    FString Type;
};

// =============================================================
// 3. 미로 게임 타입 (Maze Game Types)
// =============================================================

// 미로 게임의 진행 단계
UENUM(BlueprintType)
enum class EMazeGamePhase : uint8
{
    GP_WaitingToStart   UMETA(DisplayName = "WaitingToStart"),
    GP_Instructions     UMETA(DisplayName = "Instructions"),
    GP_MapSelection     UMETA(DisplayName = "MapSelection"),
    GP_Playing           UMETA(DisplayName = "Playing"),
    GP_GameOver          UMETA(DisplayName = "GameOver")
};

// 미로 내 동적 프롭(Prop) 동기화 데이터
USTRUCT(BlueprintType)
struct FMazePropData
{
    GENERATED_BODY()
    UPROPERTY() int32 MeshIndex;
    UPROPERTY() FVector RelativePos;
    UPROPERTY() FRotator Rotation;
};

// 미로 내 동적 조명(Light) 동기화 데이터
USTRUCT(BlueprintType)
struct FMazeLightData
{
    GENERATED_BODY()
    UPROPERTY() int32 LightIndex;    // 조명 액터 클래스 인덱스
    UPROPERTY() FVector RelativePos; // 미로 기준 상대 위치
    UPROPERTY() FRotator Rotation;   // 조명 회전값
};

// 미로 맵 크기 옵션
UENUM(BlueprintType)
enum class EMazeMapSize : uint8
{
    Small   UMETA(DisplayName = "Small (9x9)"),
    Medium  UMETA(DisplayName = "Medium (15x15)"),
    Big     UMETA(DisplayName = "Big (23x23)")
};