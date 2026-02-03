#include "MazeGenerate.h"
#include "MazeGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

void AMazeGenerate::UpdateMazeWithSeed(int32 NewSeed)
{
    this->Seed = NewSeed;

    // 1. 기존 액터 및 컴포넌트 정리
    if (SpawnedGoalActor) { SpawnedGoalActor->Destroy(); SpawnedGoalActor = nullptr; }
    for (UStaticMeshComponent* Comp : SpawnedMeshComponents) { if (Comp) Comp->DestroyComponent(); }
    SpawnedMeshComponents.Empty();

    for (AActor* Light : SpawnedLights) { if (Light) Light->Destroy(); }
    SpawnedLights.Empty();

    // 2. 미로 생성
    this->ClearMaze();
    this->UpdateMaze();

    // 3. 서버(Authority)인 경우 데이터 생성 및 전송
    if (HasAuthority())
    {
        if (AMazeGameState* GS = Cast<AMazeGameState>(UGameplayStatics::GetGameState(GetWorld())))
        {
            TArray<FMazePropData> PD; GenerateRandomPropData(PD); GS->SetMazePropData(PD);
            TArray<FMazeLightData> LD; GenerateRandomLightData(LD); GS->SetMazeLightData(LD);
        }
    }

    SpawnGoalTrigger(GetActorLocation(), GetActorScale3D());
}

FVector2D AMazeGenerate::GetMaxCellSize() const
{
    return Super::GetMaxCellSize() - FVector2D(25.0f, 25.0f);
}

void AMazeGenerate::GenerateRandomPropData(TArray<FMazePropData>& OutData)
{
    if (RandomPropMeshes.Num() == 0 || SpawnProbability <= 0.0f) return;
    for (int32 y = 0; y < MazeSize.Y; y++) {
        for (int32 x = 0; x < MazeSize.X; x++) {
            if (MazeGrid.IsValidIndex(y) && MazeGrid[y].IsValidIndex(x) && MazeGrid[y][x] == 1) {
                if ((x == 0 && y == 0) || (x == PathEnd.X && y == PathEnd.Y)) continue;
                if (FMath::FRand() < SpawnProbability) {
                    FMazePropData D;
                    D.MeshIndex = FMath::RandRange(0, RandomPropMeshes.Num() - 1);
                    float OffX = MazeCellSize.X * 0.3f; float OffY = MazeCellSize.Y * 0.3f;
                    D.RelativePos = FVector(x * MazeCellSize.X + FMath::RandRange(-OffX, OffX), y * MazeCellSize.Y + FMath::RandRange(-OffY, OffY), 50.0f);
                    D.Rotation = FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f);
                    OutData.Add(D);
                }
            }
        }
    }
}

void AMazeGenerate::SyncRandomProps(const TArray<FMazePropData>& PropData)
{
    for (UStaticMeshComponent* Comp : SpawnedMeshComponents) { if (Comp) Comp->DestroyComponent(); }
    SpawnedMeshComponents.Empty();
    FVector ML = GetActorLocation(); FVector MS = GetActorScale3D();
    for (const auto& D : PropData) {
        if (RandomPropMeshes.IsValidIndex(D.MeshIndex)) {
            UStaticMeshComponent* NewComp = NewObject<UStaticMeshComponent>(this);
            NewComp->SetStaticMesh(RandomPropMeshes[D.MeshIndex]);
            NewComp->RegisterComponent();
            NewComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
            NewComp->SetWorldLocationAndRotation(ML + (D.RelativePos * MS), D.Rotation);
            NewComp->SetWorldScale3D(MS);
            SpawnedMeshComponents.Add(NewComp);
        }
    }
}

void AMazeGenerate::GenerateRandomLightData(TArray<FMazeLightData>& OutData)
{
    if (RandomLightClasses.Num() == 0 || LightSpawnProbability <= 0.0f) return;
    for (int32 y = 0; y < MazeSize.Y; y++) {
        for (int32 x = 0; x < MazeSize.X; x++) {
            if (MazeGrid[y][x] == 1 && FMath::FRand() < LightSpawnProbability) {
                if ((x == 0 && y == 0) || (x == PathEnd.X && y == PathEnd.Y)) continue;
                FMazeLightData D;
                D.LightIndex = FMath::RandRange(0, RandomLightClasses.Num() - 1);
                D.RelativePos = FVector(x * MazeCellSize.X, y * MazeCellSize.Y, LightHeightOffset);
                D.Rotation = FRotator::ZeroRotator;
                OutData.Add(D);
            }
        }
    }
}

void AMazeGenerate::SyncRandomLights(const TArray<FMazeLightData>& LightData)
{
    for (AActor* L : SpawnedLights) { if (L) L->Destroy(); }
    SpawnedLights.Empty();
    FVector ML = GetActorLocation(); FVector MS = GetActorScale3D();
    for (const auto& Data : LightData)
    {
        if (RandomLightClasses.IsValidIndex(Data.LightIndex))
        {
            FVector FinalPos = ML + (Data.RelativePos * MS);
            AActor* NewLight = GetWorld()->SpawnActor<AActor>(RandomLightClasses[Data.LightIndex], FinalPos, Data.Rotation);

            if (NewLight)
            {
        
                NewLight->SetActorScale3D(LightScale);

                SpawnedLights.Add(NewLight);
            }
        }
    }
}

void AMazeGenerate::SpawnGoalTrigger(const FVector& ML, const FVector& MS)
{
    if (!GoalActorClass) return;
    FVector FL = ML + (FVector(PathEnd.X * MazeCellSize.X, PathEnd.Y * MazeCellSize.Y, 150.0f) * MS);
    SpawnedGoalActor = GetWorld()->SpawnActor<AActor>(GoalActorClass, FL, FRotator::ZeroRotator);
}