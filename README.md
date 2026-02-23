# 🎮 NoobGame (Multiplayer Party Game Project)

![Project Banner](https://via.placeholder.com/1200x400?text=NoobGame+Project+Banner)

> **"다양한 미니게임을 친구들과 함께 즐기는 멀티플레이어 파티 게임"**
>
> *언리얼 엔진 5를 활용한 네트워크 게임 개발 포트폴리오입니다.*

---

## 📋 1. 프로젝트 개요 (Overview)

*   **프로젝트명:** NoobGame
*   **장르:** 멀티플레이어 파티 / 캐주얼 / 서바이벌
*   **개발 인원:** 1인 개발 (원우)
*   **역할:** 디자인, 사운드, 애니메이션 리깅, 서버 및 시스템 구현 등 **프로젝트 전 과정 단독 수행**
*   **개발 기간:** 2025.07 ~ 2026.02
*   **플랫폼:** PC (Steam)
*   **핵심 목표:**
    *   **프로젝트 전 과정 습득:** 기획부터 아트, 프로그래밍, 배포까지 1인 개발 사이클 완수.
    *   언리얼 엔진의 **Replication(네트워크 동기화)** 시스템 심층 이해 및 구현.
    *   확장 가능한 모듈형 게임 아키텍처 설계 (다양한 미니게임 모드 통합).

---

## 📸 2. 플레이 사진 (Screenshots)

| 메인 메뉴 & 옵션 (Main Menu) | 게임 선택 (Game Selection) |
| :---: | :---: |
| ![MainMenu](https://via.placeholder.com/400x225?text=Main+Menu+Screenshot) | ![GameSelect](https://via.placeholder.com/400x225?text=Game+Select+Screenshot) |
| **미로 게임 (Maze)** | **퀴즈 게임 (Quiz)** |
| ![Maze](https://via.placeholder.com/400x225?text=Maze+Screenshot) | ![Quiz](https://via.placeholder.com/400x225?text=Quiz+Screenshot) |

---

## 🎥 3. 플레이 영상 (Gameplay Video)

> *아래 이미지를 클릭하면 플레이 영상을 시청할 수 있습니다. (YouTube)*

[![Gameplay Video](https://img.youtube.com/vi/YOUR_VIDEO_ID/0.jpg)](https://www.youtube.com/watch?v=YOUR_VIDEO_ID)

*또는 아래에 플레이 영상을 드래그하여 업로드해주세요.*
*(GitHub Issue나 Editor에 영상을 드래그하면 생성되는 링크를 이곳에 붙여넣으세요)*

---

## 🛠️ 4. 사용 기술 (Tech Stack)

### Engine & Language
*   **Unreal Engine 5.x**: Core Engine
*   **C++ & Blueprints**: 하이브리드 구조 (C++로 핵심 시스템 구현, BP로 로직/UI 연결)
*   **Visual Studio 2022**: IDE

### Generative AI Tools
*   **Tripo 3D**: 3D 에셋 생성 및 프로토타이핑
*   **Nano Banana**: 텍스처 및 이미지 리소스 생성
*   **Rodin AI**: 3D 모델 최적화 및 변환

### Version Control
*   **Git / GitHub**: 소스 코드 관리
*   **Git LFS**: 대용량 에셋(.blend, .uasset) 관리

---

## 📚 5. 기술 문서 (Technical Docs)

본 프로젝트는 `GameMode`, `GameState`, `PlayerController` 등 언리얼 프레임워크를 정석적으로 활용하여 설계되었습니다.

### 5.1 네트워크 아키텍처 (Network Architecture)
*   **Dedicated Server** 모델 기반.
*   `ANoobGameStateBase`를 상속받은 각 게임 모드별 State(`MazeGameState`, `FruitGameState`)에서 게임의 진행 상태(남은 시간, 점수, 단계)를 관리하고 리플리케이션합니다.

<details>
<summary>💻 GameStateBase.h 예시 (접기/펼치기)</summary>

```cpp
// Source/NoobGame/NoobGameStateBase.h
UCLASS()
class NOOBGAME_API ANoobGameStateBase : public AGameStateBase
{
    GENERATED_BODY()

public:
    // 전체 남은 시간 (Replicated)
    UPROPERTY(ReplicatedUsing = OnRep_RemainingTime, BlueprintReadOnly)
    int32 RemainingTime;

    UFUNCTION()
    void OnRep_RemainingTime();
};
```
</details>

### 5.2 캐릭터 시스템
*   `ANoobGameCharacter`를 베이스로 하여 다양한 파생 클래스(`CombatCharacter`, `PlatformingCharacter` 등)로 확장.
*   공통적인 움직임, 상호작용 인터페이스(`ICombatInteractable`) 등을 모듈화하여 관리.

---

## 🎮 6. 구현 기능 및 게임 모드 (Game Modes & Features)

### 6.1 과일 게임 (Fruit Game)
*   **핵심 로직**: 서버에서 정답 과일을 설정하고, 클라이언트가 제출한 답과 비교.
*   **구현**: `AFruitGameState`에서 정답 과일 목록을 관리하며, `Replicated` 변수를 통해 클라이언트와 동기화.

<details>
<summary>💻 정답 비교 로직 (Pseudo Code)</summary>

```cpp
// AFruitGameState.cpp
void AFruitGameState::SubmitAnswer(AFruitPlayerController* PC, FName SubmittedFruitID)
{
    if (HasAuthority())
    {
        bool bIsCorrect = TargetFruits.Contains(SubmittedFruitID);
        if (bIsCorrect)
        {
            PC->AddScore(100);
            // 정답 처리 후 새로운 과일 설정 로직 호출
        }
    }
}
```
</details>

### 6.2 미로 게임 (Maze Game)
*   **핵심 로직**: 절차적 미로 생성 알고리즘.
*   **구현**: `AMazeGenerate` 액터가 DFS(깊이 우선 탐색) 또는 Prim 알고리즘을 사용하여 런타임에 벽(`MazeStoneWall`)을 배치.

<details>
<summary>💻 미로 생성 알고리즘 (Pseudo Code)</summary>

```cpp
// Source/NoobGame/MazeGame/MazeGenerate.cpp
void AMazeGenerate::GenerateMaze()
{
    // 그리드 초기화
    InitializeGrid();

    // 스택을 이용한 백트래킹 알고리즘
    while (Stack.Num() > 0)
    {
        FCell Current = Stack.Top();
        FCell Next = GetUnvisitedNeighbor(Current);

        if (Next.IsValid())
        {
            RemoveWallBetween(Current, Next);
            MarkVisited(Next);
            Stack.Push(Next);
        }
        else
        {
            Stack.Pop();
        }
    }
}
```
</details>

### 6.3 퀴즈 게임 (Quiz Game)
*   **핵심 로직**: 데이터 테이블 기반 문제 출제 및 액터 스폰.
*   **구현**: `QuizList.json` 또는 `UDataTable`에서 문제 데이터를 로드하여, 런타임에 O/X 발판(`OXQuizObstacle`)과 문제를 스폰.

<details>
<summary>💻 문제 스폰 로직 (Pseudo Code)</summary>

```cpp
// Source/NoobGame/QuizGame/OXQuizGameMode.cpp
void AOXQuizGameMode::SpawnNextQuestion()
{
    FQuizData* Row = QuizDataTable->FindRow<FQuizData>(CurrentRowName, Context);
    if (Row)
    {
        // 문제 텍스트 업데이트 (GameState를 통해 리플리케이션)
        GameState->CurrentQuestion = Row->QuestionText;
        
        // 정답에 따른 발판 설정 (O/X 중 정답만 안전지대로 설정)
        SetupPlatforms(Row->bIsTrue);
    }
}
```
</details>

---

## 📂 7. 프로젝트 구조 (Project Structure)

```
C:/Users/qudtn/UnrealEngine/NoobGame
├── Config/               # 엔진 설정 파일
├── Content/              # 블루프린트, 에셋, 맵
│   ├── Blueprints/       # 로비, UI 등 BP
│   ├── Characters/       # 캐릭터 모델링 및 애니메이션
│   ├── Levels/           # Game, Lobby 맵 파일
│   └── UI/               # 위젯 및 이미지 리소스
├── Source/               # C++ 소스 코드
│   └── NoobGame/
│       ├── FruitGame/    # 과일 게임 로직
│       ├── MazeGame/     # 미로 게임 로직
│       ├── QuizGame/     # 퀴즈 게임 로직
│       ├── MenuSystem/   # 메뉴 및 UI 시스템
│       ├── NPC/          # NPC AI
│       └── Variant_*/    # 전투/플랫포머 등 프로토타입
├── 기획/                 # 기획 문서
└── 모델링/               # Blender 원본 파일 (Cat, Dog, Raccoon 등)
```

---

## 🐛 8. 트러블 슈팅 (Troubleshooting)

### 이슈 1: Git LFS 및 대용량 파일 관리
*   **문제**: `.blend` 파일 등 100MB를 초과하는 에셋을 Push 할 때 GitHub 용량 제한으로 인해 업로드 실패.
*   **해결**:
    1.  `Git LFS`를 도입하여 대용량 파일을 포인터로 관리하도록 설정.
    2.  불필요한 백업 파일(`.blend1`)을 `.gitignore`에 추가하고 히스토리에서 제거하여 저장소 용량 최적화.
    3.  `git filter-branch` 또는 `git reset`을 활용해 잘못 올라간 대용량 파일의 커밋 기록 정리.

### 이슈 2: 동적 액터 스폰과 네트워크 오너십
*   **문제**: 미로 게임에서 런타임에 생성된 벽(Wall)이 클라이언트에서 보이지 않거나 충돌 처리가 안 되는 현상.
*   **해결**: `SpawnActor` 호출 시 `bNetLoadOnClient` 설정을 확인하고, `Replicates` 속성을 True로 설정. 또한 `SwitchHasAuthority`를 통해 서버에서만 스폰 로직이 실행되도록 엄격히 제어.

---

*Contact: Wonwoo*
