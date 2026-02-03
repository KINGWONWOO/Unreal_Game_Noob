// FruitGameTypes.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameTypes.generated.h"

// 0. 공용
UENUM(BlueprintType)
enum class ECharacterType : uint8
{
    ECT_None,
    ECT_Cat		UMETA(DisplayName = "Cat"),
    ECT_Dog		UMETA(DisplayName = "Dog")
};

// 1. Fruit Game
UENUM(BlueprintType)
enum class EFruitType : uint8
{
	FT_None		UMETA(DisplayName = "None"),
	FT_Apple	UMETA(DisplayName = "Apple"),
	FT_Banana	UMETA(DisplayName = "Banana"),
	FT_Cherry	UMETA(DisplayName = "Cherry"),
	FT_Orange	UMETA(DisplayName = "Orange")
};

UENUM(BlueprintType)
enum class EFruitGamePhase : uint8
{
	GP_WaitingToStart	UMETA(DisplayName = "WaitingToStart"), // 2명 대기
	GP_Instructions		UMETA(DisplayName = "Instructions"),  // 게임 설명
	GP_Setup			UMETA(DisplayName = "Setup"), // 각자 과일 선택
	GP_SpinnerTurn		UMETA(DisplayName = "SpinnerTurn"), // <-- 신규 단계 추가
	GP_PlayerTurn		UMETA(DisplayName = "PlayerTurn"), // 턴 진행
	GP_GameOver			UMETA(DisplayName = "GameOver") // 게임 종료
};


// 2. OX Quiz Game

UENUM(BlueprintType)
enum class EQuizDifficulty : uint8
{
    Easy    UMETA(DisplayName = "Easy (하)"),
    Medium  UMETA(DisplayName = "Medium (중)"),
    Hard    UMETA(DisplayName = "Hard (상)")
};

UENUM(BlueprintType)
enum class EQuizGamePhase : uint8
{
    GP_WaitingToStart   UMETA(DisplayName = "WaitingToStart"),
    GP_Instructions     UMETA(DisplayName = "Instructions"),
    GP_Playing          UMETA(DisplayName = "Playing"),
    GP_GameOver         UMETA(DisplayName = "GameOver")
};

UENUM(BlueprintType)
enum class EQuizCategory : uint8
{
    None            UMETA(DisplayName = "없음"),

    // JSON 데이터에 존재하는 카테고리들
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

// 데이터 테이블 구조체
USTRUCT(BlueprintType)
struct FQuizData : public FTableRowBase
{
    GENERATED_BODY()

    /** 퀴즈 문제 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quiz")
    FText Question;

    /** 선택지 배열 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quiz")
    TArray<FText> Answers;

    /** 정답 인덱스 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quiz")
    int32 CorrectAnswerIndex;

    /** 난이도 (JSON의 "Easy", "Hard" 등 영어 텍스트는 Enum과 자동 매칭됨) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quiz")
    EQuizDifficulty Difficulty;


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quiz")
    FString Category;

    /** 문제 유형 (OX, 3Choice 등) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quiz")
    FString Type;
};

//3. Maze Game
UENUM(BlueprintType)
enum class EMazeGamePhase : uint8
{
    GP_WaitingToStart   UMETA(DisplayName = "WaitingToStart"),
    GP_Instructions     UMETA(DisplayName = "Instructions"),
    GP_MapSelection     UMETA(DisplayName = "MapSelection"),
    GP_Playing          UMETA(DisplayName = "Playing"),
    GP_GameOver         UMETA(DisplayName = "GameOver")
};

USTRUCT(BlueprintType)
struct FMazePropData
{
    GENERATED_BODY()
    UPROPERTY() int32 MeshIndex;
    UPROPERTY() FVector RelativePos;
    UPROPERTY() FRotator Rotation;
};

USTRUCT(BlueprintType)
struct FMazeLightData
{
    GENERATED_BODY()
    UPROPERTY() int32 LightIndex;     // RandomLightClasses 배열의 인덱스
    UPROPERTY() FVector RelativePos;  // 미로 기준 상대 위치
    UPROPERTY() FRotator Rotation;    // 회전값
};

UENUM(BlueprintType)
enum class EMazeMapSize : uint8
{
    Small   UMETA(DisplayName = "Small (9x9)"),
    Medium  UMETA(DisplayName = "Medium (15x15)"),
    Big     UMETA(DisplayName = "Big (23x23)")
};