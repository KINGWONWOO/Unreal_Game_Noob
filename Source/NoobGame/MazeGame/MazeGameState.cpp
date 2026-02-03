#include "MazeGameState.h"
#include "Net/UnrealNetwork.h"
#include "Maze.h"
#include "MazeGenerate.h"
#include "Kismet/GameplayStatics.h"

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
    DOREPLIFETIME(AMazeGameState, ReplicatedLights); // 조명 복제 등록
}

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
        OnRep_MazeSeed();
    }
}

void AMazeGameState::OnRep_GamePhase() { OnGamePhaseChanged.Broadcast(CurrentGamePhase); }
void AMazeGameState::OnRep_PlayingCountdown() { OnPlayingCountdownChanged.Broadcast(PlayingCountdown); }

void AMazeGameState::OnRep_MazeSeed()
{
    if (MazeSeed == 0) return;

    // [로그 추가] 현재 복제된 MapSize 확인
    FString MapSizeString = UEnum::GetValueAsString(MapSize);
    UE_LOG(LogTemp, Warning, TEXT("[Client] MazeSeed Replicated: %d | Current MapSize: %s"), MazeSeed, *MapSizeString);

    TArray<AActor*> FoundMazes;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMazeGenerate::StaticClass(), FoundMazes);

    if (FoundMazes.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("[Client] MazeGenerate actor NOT found, retrying in 0.5s..."));
        FTimerHandle RetryHandle;
        GetWorld()->GetTimerManager().SetTimer(RetryHandle, [this]() { this->OnRep_MazeSeed(); }, 0.5f, false);
        return;
    }

    for (AActor* Actor : FoundMazes)
    {
        if (AMazeGenerate* MazeActor = Cast<AMazeGenerate>(Actor))
        {
            int32 SizeVal = 15; // Medium 기본값
            if (MapSize == EMazeMapSize::Small) SizeVal = 9;
            else if (MapSize == EMazeMapSize::Big) SizeVal = 25;

            // [로그 추가] 설정될 실제 미로 크기 확인
            UE_LOG(LogTemp, Log, TEXT("[Client] Applying MazeSize: %d x %d (MapSize: %s)"), SizeVal, SizeVal, *MapSizeString);

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

void AMazeGameState::SetMazePropData(const TArray<FMazePropData>& NewData)
{
    if (HasAuthority()) { ReplicatedProps = NewData; OnRep_PropData(); }
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

void AMazeGameState::SetMazeLightData(const TArray<FMazeLightData>& NewData)
{
    if (HasAuthority()) { ReplicatedLights = NewData; OnRep_LightData(); }
}