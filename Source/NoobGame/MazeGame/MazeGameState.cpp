#include "MazeGameState.h"
#include "Net/UnrealNetwork.h"
#include "Maze.h"
#include "MazeGenerate.h"
#include "Kismet/GameplayStatics.h"

// =============================================================
// 1. 초기화 및 복제 설정
// =============================================================

AMazeGameState::AMazeGameState()
{
    CurrentGamePhase = EMazeGamePhase::GP_WaitingToStart;
    PlayingCountdown = 0;
    MazeSeed = 0;
    MapSize = EMazeMapSize::Medium;
}

void AMazeGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AMazeGameState, CurrentGamePhase);
    DOREPLIFETIME(AMazeGameState, PlayingCountdown);
    DOREPLIFETIME(AMazeGameState, MazeSeed);
    DOREPLIFETIME(AMazeGameState, MapSize);
    DOREPLIFETIME(AMazeGameState, ReplicatedProps);
    DOREPLIFETIME(AMazeGameState, ReplicatedLights);
}

// =============================================================
// 2. 서버 측 데이터 업데이트 함수
// =============================================================

void AMazeGameState::SetPlayingCountdown(int32 TimeLeft)
{
    if (HasAuthority())
    {
        PlayingCountdown = TimeLeft;
        OnPlayingCountdownChanged.Broadcast(PlayingCountdown);
    }
}

void AMazeGameState::SetMazeSeed(int32 NewSeed)
{
    if (HasAuthority())
    {
        MazeSeed = NewSeed;
        OnRep_MazeSeed(); // 서버에서도 생성 로직 실행을 위해 직접 호출
    }
}

void AMazeGameState::SetMazePropData(const TArray<FMazePropData>& NewData)
{
    if (HasAuthority()) { ReplicatedProps = NewData; OnRep_PropData(); }
}

void AMazeGameState::SetMazeLightData(const TArray<FMazeLightData>& NewData)
{
    if (HasAuthority()) { ReplicatedLights = NewData; OnRep_LightData(); }
}

// =============================================================
// 3. 클라이언트 동기화 및 미로 생성 로직 (RepNotify)
// =============================================================

void AMazeGameState::OnRep_GamePhase() { OnGamePhaseChanged.Broadcast(CurrentGamePhase); }
void AMazeGameState::OnRep_PlayingCountdown() { OnPlayingCountdownChanged.Broadcast(PlayingCountdown); }

void AMazeGameState::OnRep_MazeSeed()
{
    if (MazeSeed == 0) return;

    TArray<AActor*> FoundMazes;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMazeGenerate::StaticClass(), FoundMazes);

    // 미로 생성 액터가 아직 스폰되지 않았다면 잠시 후 재시도
    if (FoundMazes.Num() == 0)
    {
        FTimerHandle RetryHandle;
        GetWorld()->GetTimerManager().SetTimer(RetryHandle, [this]() { this->OnRep_MazeSeed(); }, 0.5f, false);
        return;
    }

    // 복제된 MapSize를 기반으로 실제 수치 결정 및 미로 생성
    for (AActor* Actor : FoundMazes)
    {
        if (AMazeGenerate* MazeActor = Cast<AMazeGenerate>(Actor))
        {
            int32 SizeVal = 15;
            if (MapSize == EMazeMapSize::Small) SizeVal = 9;
            else if (MapSize == EMazeMapSize::Big) SizeVal = 25;

            MazeActor->MazeSize.X = MazeActor->MazeSize.Y = SizeVal;
            MazeActor->UpdateMazeWithSeed(MazeSeed);
        }
    }
    OnMazeSeedChanged.Broadcast(MazeSeed);
}

void AMazeGameState::OnRep_PropData()
{
    TArray<AActor*> FoundMazes;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMazeGenerate::StaticClass(), FoundMazes);
    for (AActor* Actor : FoundMazes)
    {
        if (AMazeGenerate* Maze = Cast<AMazeGenerate>(Actor)) Maze->SyncRandomProps(ReplicatedProps);
    }
}

void AMazeGameState::OnRep_LightData()
{
    TArray<AActor*> FoundMazes;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMazeGenerate::StaticClass(), FoundMazes);
    for (AActor* Actor : FoundMazes)
    {
        if (AMazeGenerate* Maze = Cast<AMazeGenerate>(Actor)) Maze->SyncRandomLights(ReplicatedLights);
    }
}