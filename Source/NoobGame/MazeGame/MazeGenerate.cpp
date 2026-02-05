#include "MazeGenerate.h"
#include "MazeGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

// =============================================================
// 1. 미로 업데이트 및 초기화 (Core Update Flow)
// =============================================================

void AMazeGenerate::UpdateMazeWithSeed(int32 NewSeed)
{
    this->Seed = NewSeed;

    // 1. 기존에 생성된 모든 리소스(골, 프롭, 조명)를 파괴 및 정리
    if (SpawnedGoalActor) { SpawnedGoalActor->Destroy(); SpawnedGoalActor = nullptr; }
    for (UStaticMeshComponent* Comp : SpawnedMeshComponents) { if (Comp) Comp->DestroyComponent(); }
    SpawnedMeshComponents.Empty();

    for (AActor* Light : SpawnedLights) { if (Light) Light->Destroy(); }
    SpawnedLights.Empty();

    // 2. 새로운 시드로 미로 데이터 재생성
    this->ClearMaze();
    this->UpdateMaze();

    // 3. 서버 권한(Authority)이 있는 경우 랜덤 데이터를 생성하여 GameState로 전송
    if (HasAuthority())
    {
        if (AMazeGameState* GS = Cast<AMazeGameState>(UGameplayStatics::GetGameState(GetWorld())))
        {
            TArray<FMazePropData> PD;
            GenerateRandomPropData(PD);
            GS->SetMazePropData(PD);

            TArray<FMazeLightData> LD;
            GenerateRandomLightData(LD);
            GS->SetMazeLightData(LD);
        }
    }

    // 4. 골인 지점 트리거 생성
    SpawnGoalTrigger(GetActorLocation(), GetActorScale3D());
}

FVector2D AMazeGenerate::GetMaxCellSize() const
{
    // 부모의 셀 사이즈에서 여백(25.0f)을 뺀 값을 반환
    return Super::GetMaxCellSize() - FVector2D(25.0f, 25.0f);
}

// =============================================================
// 2. 랜덤 데이터 생성 로직 (Data Generation)
// =============================================================

void AMazeGenerate::GenerateRandomPropData(TArray<FMazePropData>& OutData)
{
    if (RandomPropMeshes.Num() == 0 || SpawnProbability <= 0.0f) return;

    for (int32 y = 0; y < MazeSize.Y; y++) {
        for (int32 x = 0; x < MazeSize.X; x++) {
            // 길(Path, 1)이면서 시작점이나 끝점이 아닌 곳에 확률적으로 스폰
            if (MazeGrid.IsValidIndex(y) && MazeGrid[y].IsValidIndex(x) && MazeGrid[y][x] == 1) {
                if ((x == 0 && y == 0) || (x == PathEnd.X && y == PathEnd.Y)) continue;

                if (FMath::FRand() < SpawnProbability) {
                    FMazePropData D;
                    D.MeshIndex = FMath::RandRange(0, RandomPropMeshes.Num() - 1);
                    float OffX = MazeCellSize.X * 0.3f;
                    float OffY = MazeCellSize.Y * 0.3f;

                    D.RelativePos = FVector(x * MazeCellSize.X + FMath::RandRange(-OffX, OffX),
                        y * MazeCellSize.Y + FMath::RandRange(-OffY, OffY), 50.0f);
                    D.Rotation = FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f);
                    OutData.Add(D);
                }
            }
        }
    }
}

void AMazeGenerate::GenerateRandomLightData(TArray<FMazeLightData>& OutData)
{
    if (RandomLightClasses.Num() == 0 || LightSpawnProbability <= 0.0f) return;

    for (int32 y = 0; y < MazeSize.Y; y++) {
        for (int32 x = 0; x < MazeSize.X; x++) {
            // 길 위에 설정된 확률(LightSpawnProbability)에 따라 조명 배치 결정
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

// =============================================================
// 3. 동기화 및 오브젝트 스폰 (Sync & Spawning)
// =============================================================

void AMazeGenerate::SyncRandomProps(const TArray<FMazePropData>& PropData)
{
    // 기존 컴포넌트 제거 및 정리
    for (UStaticMeshComponent* Comp : SpawnedMeshComponents) { if (Comp) Comp->DestroyComponent(); }
    SpawnedMeshComponents.Empty();

    FVector ML = GetActorLocation();
    FVector MS = GetActorScale3D();

    for (const auto& D : PropData) {
        if (RandomPropMeshes.IsValidIndex(D.MeshIndex)) {
            // 새로운 스태틱 메시 컴포넌트를 동적으로 생성하여 부착
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

void AMazeGenerate::SyncRandomLights(const TArray<FMazeLightData>& LightData)
{
    for (AActor* L : SpawnedLights) { if (L) L->Destroy(); }
    SpawnedLights.Empty();

    FVector ML = GetActorLocation();
    FVector MS = GetActorScale3D();

    for (const auto& Data : LightData)
    {
        if (RandomLightClasses.IsValidIndex(Data.LightIndex))
        {
            // 월드 좌표를 계산하여 조명 액터를 스폰
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
    // 경로의 마지막 위치(PathEnd)에 골인 지점 스폰
    FVector FL = ML + (FVector(PathEnd.X * MazeCellSize.X, PathEnd.Y * MazeCellSize.Y, 150.0f) * MS);
    SpawnedGoalActor = GetWorld()->SpawnActor<AActor>(GoalActorClass, FL, FRotator::ZeroRotator);
}