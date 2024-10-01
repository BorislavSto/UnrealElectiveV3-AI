// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/KismetMathLibrary.h"
#include "Landscape.h"
#include "LandscapeInfo.h"
#include "LandscapeEditorUtils.h"
#include "LandscapeStreamingProxy.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInterface.h"
#include "LandscapeLayerInfoObject.h"
#include "WorldGenerator.h"

AWorldGenerator* AWorldGenerator::Instance = nullptr;

// Sets default values
AWorldGenerator::AWorldGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
    RootComponent = MeshComponent;
}

// Called when the game starts or when spawned
void AWorldGenerator::BeginPlay()
{
	Super::BeginPlay();

    if (Instance == nullptr)
    {
        Instance = this;
        UE_LOG(LogTemp, Warning, TEXT("No other WorldGenerator instances detected."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Multiple WorldGenerator instances detected. Destroying duplicate."));
        Destroy();
    }

    //GenerateWorldPerlinNoise(100, 100, 0.01f);
    //GenerateWorldDiamondSquare(6, 0.5f);
    //GenerateWorldCellularAutomata(100, 100, 0.4f, 500.0f, 0.1f);
    GenerateLandscape();
}

// Called every frame
void AWorldGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AWorldGenerator* AWorldGenerator::GetInstance(UWorld* World)
{
    if (Instance == nullptr)
        Instance = World->SpawnActor<AWorldGenerator>(AWorldGenerator::StaticClass());
    
    return Instance;
}

void AWorldGenerator::GenerateWorldPerlinNoise(int32 Width, int32 Height, float Scale)
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;

    for (int32 Y = 0; Y < Height; ++Y)
    {
        for (int32 X = 0; X < Width; ++X)
        {
            float PerlinValue = FMath::PerlinNoise2D(FVector2D(X * Scale, Y * Scale)) * 0.5f + 0.5f;
            Vertices.Add(FVector(X * 100, Y * 100, PerlinValue * 1000));

            if (X < Width - 1 && Y < Height - 1)
            {
                int32 V = X + Y * Width;
                Triangles.Add(V);
                Triangles.Add(V + Width);
                Triangles.Add(V + 1);
                Triangles.Add(V + 1);
                Triangles.Add(V + Width);
                Triangles.Add(V + Width + 1);
            }
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("No other WorldGenerator instances detected."));
    CreateMesh(Vertices, Triangles);
}

void AWorldGenerator::GenerateWorldDiamondSquare(int32 Size, float Roughness)
{
    if (Size > 10) // Limit to prevent overflow
        Size = 10;

    int32 GridSize = (1 << Size) + 1; // Calculate grid size as 2^Size + 1, with safe shifting

    TArray<TArray<float>> HeightMap;
    HeightMap.SetNum(GridSize);
    for (auto& Row : HeightMap)
    {
        Row.SetNum(GridSize);
    }

    // Initialize corners
    HeightMap[0][0] = HeightMap[0][GridSize - 1] = HeightMap[GridSize - 1][0] = HeightMap[GridSize - 1][GridSize - 1] = 0;

    // Diamond-Square algorithm
    for (int32 Step = GridSize - 1; Step > 1; Step /= 2)
    {
        int32 HalfStep = Step / 2;

        // Diamond step
        for (int32 Y = HalfStep; Y < GridSize; Y += Step)
        {
            for (int32 X = HalfStep; X < GridSize; X += Step)
            {
                float Average = (HeightMap[Y - HalfStep][X - HalfStep] + HeightMap[Y - HalfStep][X + HalfStep] +
                    HeightMap[Y + HalfStep][X - HalfStep] + HeightMap[Y + HalfStep][X + HalfStep]) / 4.0f;
                HeightMap[Y][X] = Average + FMath::RandRange(-Roughness, Roughness);
            }
        }

        // Square step
        for (int32 Y = 0; Y < GridSize; Y += HalfStep)
        {
            for (int32 X = (Y + HalfStep) % Step; X < GridSize; X += Step)
            {
                float Average = 0;
                int32 Count = 0;

                if (X >= HalfStep) { Average += HeightMap[Y][X - HalfStep]; Count++; }
                if (X + HalfStep < GridSize) { Average += HeightMap[Y][X + HalfStep]; Count++; }
                if (Y >= HalfStep) { Average += HeightMap[Y - HalfStep][X]; Count++; }
                if (Y + HalfStep < GridSize) { Average += HeightMap[Y + HalfStep][X]; Count++; }

                Average /= Count;
                HeightMap[Y][X] = Average + FMath::RandRange(-Roughness, Roughness);
            }
        }

        Roughness /= 2.0f;
    }

    // Create mesh from heightmap
    TArray<FVector> Vertices;
    TArray<int32> Triangles;

    for (int32 Y = 0; Y < GridSize; ++Y)
    {
        for (int32 X = 0; X < GridSize; ++X)
        {
            Vertices.Add(FVector(X * 100, Y * 100, HeightMap[Y][X] * 1000));

            if (X < GridSize - 1 && Y < GridSize - 1)
            {
                int32 V = X + Y * GridSize;
                Triangles.Add(V);
                Triangles.Add(V + GridSize);
                Triangles.Add(V + 1);
                Triangles.Add(V + 1);
                Triangles.Add(V + GridSize);
                Triangles.Add(V + GridSize + 1);
            }
        }
    }

    CreateMesh(Vertices, Triangles);
}

void AWorldGenerator::GenerateWorldCellularAutomata(int32 Width, int32 Height, float FillProbability, float MaxHeight, float NoiseScale)
{
    TArray<TArray<bool>> Grid;
    Grid.SetNum(Height);
    for (auto& Row : Grid)
    {
        Row.SetNum(Width);
        for (auto& Cell : Row)
        {
            Cell = FMath::RandRange(0.0f, 1.0f) < FillProbability;
        }
    }

    // Apply cellular automata rules
    for (int32 Iteration = 0; Iteration < 5; ++Iteration)
    {
        TArray<TArray<bool>> NewGrid = Grid;

        for (int32 Y = 0; Y < Height; ++Y)
        {
            for (int32 X = 0; X < Width; ++X)
            {
                int32 AliveNeighbors = 0;
                for (int32 NY = -1; NY <= 1; ++NY)
                {
                    for (int32 NX = -1; NX <= 1; ++NX)
                    {
                        if (NX == 0 && NY == 0) continue;
                        int32 CheckX = X + NX;
                        int32 CheckY = Y + NY;
                        if (CheckX >= 0 && CheckX < Width && CheckY >= 0 && CheckY < Height)
                        {
                            AliveNeighbors += Grid[CheckY][CheckX] ? 1 : 0;
                        }
                    }
                }

                if (Grid[Y][X])
                {
                    NewGrid[Y][X] = AliveNeighbors >= 4;
                }
                else
                {
                    NewGrid[Y][X] = AliveNeighbors >= 5;
                }
            }
        }

        Grid = NewGrid;
    }

    // Create mesh from grid with Perlin noise for height variation
    TArray<FVector> Vertices;
    TArray<int32> Triangles;

    for (int32 Y = 0; Y < Height; ++Y)
    {
        for (int32 X = 0; X < Width; ++X)
        {
            // Using Perlin noise to modify Z based on terrain randomness
            float Z = FMath::PerlinNoise2D(FVector2D(X * NoiseScale, Y * NoiseScale)) * MaxHeight;

            // Apply some smoothing based on number of alive neighbors
            int32 AliveNeighbors = 0;
            for (int32 NY = -1; NY <= 1; ++NY)
            {
                for (int32 NX = -1; NX <= 1; ++NX)
                {
                    if (NX == 0 && NY == 0) continue;
                    int32 CheckX = X + NX;
                    int32 CheckY = Y + NY;
                    if (CheckX >= 0 && CheckX < Width && CheckY >= 0 && CheckY < Height)
                    {
                        AliveNeighbors += Grid[CheckY][CheckX] ? 1 : 0;
                    }
                }
            }

            // Modify height further based on number of alive neighbors to smooth terrain
            Z += FMath::Lerp(0.0f, MaxHeight * 0.5f, static_cast<float>(AliveNeighbors) / 8.0f);
            Vertices.Add(FVector(X * 100, Y * 100, Z));

            // Create triangles for mesh
            if (X < Width - 1 && Y < Height - 1)
            {
                int32 V = X + Y * Width;
                Triangles.Add(V);
                Triangles.Add(V + Width);
                Triangles.Add(V + 1);
                Triangles.Add(V + 1);
                Triangles.Add(V + Width);
                Triangles.Add(V + Width + 1);
            }
        }
    }

    CreateMesh(Vertices, Triangles);
}

void AWorldGenerator::CreateMesh(const TArray<FVector>& Vertices, const TArray<int32>& Triangles)
{
    TArray<FVector> Normals;
    TArray<FVector2D> UV0;
    TArray<FProcMeshTangent> Tangents;
    TArray<FLinearColor> VertexColors;

    MeshComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);
}


void AWorldGenerator::GenerateLandscape()
{
    UWorld* World = GetWorld();
    if (!World) return;

    // Define landscape size
    int32 QuadsPerSection = 63;
    int32 SectionsPerComponent = 1;
    int32 ComponentCountX = 8;
    int32 ComponentCountY = 8;
    int32 QuadsPerComponent = QuadsPerSection * SectionsPerComponent;
    int32 SizeX = ComponentCountX * QuadsPerComponent + 1;
    int32 SizeY = ComponentCountY * QuadsPerComponent + 1;

    // Create heightmap data
    TArray<uint16> HeightData;
    HeightData.SetNumUninitialized(SizeX * SizeY);

    // Generate heightmap using Perlin noise
    for (int32 Y = 0; Y < SizeY; Y++)
    {
        for (int32 X = 0; X < SizeX; X++)
        {
            float Height = 0.0f;
            float Amplitude = 1.0f;
            float Frequency = 0.01f;
            for (int32 Octave = 0; Octave < 4; Octave++)
            {
                Height += Amplitude * FMath::PerlinNoise2D(FVector2D(X * Frequency, Y * Frequency));
                Amplitude *= 0.5f;
                Frequency *= 2.0f;
            }
            Height = FMath::Clamp(Height, -1.0f, 1.0f);
            HeightData[Y * SizeX + X] = FMath::Clamp<uint16>((Height + 1.0f) * 32767.5f, 0, 65535);
        }
    }

    // Spawn the landscape actor
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    ALandscape* Landscape = World->SpawnActor<ALandscape>(ALandscape::StaticClass(), FTransform::Identity, SpawnParams);

    if (Landscape)
    {
        // Generate a new GUID for the landscape
        FGuid LandscapeGuid = FGuid::NewGuid();

        // Create height data map (GUID is the landscape's unique identifier)
        TMap<FGuid, TArray<uint16>> ImportHeightData;
        ImportHeightData.Add(LandscapeGuid, HeightData);

        // Ensure height data was correctly added
        if (!ImportHeightData.Contains(LandscapeGuid))
        {
            UE_LOG(LogTemp, Error, TEXT("Height data for landscape not found!"));
            return; // Prevent further execution
        }

        // Prepare an empty material layer info map
        TMap<FGuid, TArray<FLandscapeImportLayerInfo>> ImportMaterialLayerInfos;
        ImportMaterialLayerInfos.Add(LandscapeGuid, TArray<FLandscapeImportLayerInfo>());

        // Ensure material layer info was correctly added
        if (!ImportMaterialLayerInfos.Contains(LandscapeGuid))
        {
            UE_LOG(LogTemp, Error, TEXT("Material layer info for landscape not found!"));
            return; // Prevent further execution
        }

        // Import the landscape
        bool bSuccess = Landscape->Import(
            LandscapeGuid,
            0, 0, SizeX - 1, SizeY - 1,
            SectionsPerComponent,
            QuadsPerSection,
            ImportHeightData,
            nullptr, // No heightmap filename
            ImportMaterialLayerInfos,
            ELandscapeImportAlphamapType::Additive,
            nullptr // No import layers
        );

        if (!bSuccess)
        {
            UE_LOG(LogTemp, Error, TEXT("Landscape import failed."));
            return;
        }

        // Assign a material if one is set
        if (LandscapeMaterial)
        {
            Landscape->LandscapeMaterial = LandscapeMaterial;
        }

        // Set landscape properties
        Landscape->SetActorScale3D(FVector(100.0f, 100.0f, 100.0f)); // Scale the landscape
        Landscape->MaxLODLevel = 8; // Set maximum LOD level
        Landscape->bCastStaticShadow = true;
        Landscape->bCastDynamicShadow = true;

        // Foliage spawning (simplified for now, could be more detailed)
        if (Landscape->GetLandscapeInfo())
        {
            for (const FMyFoliageTypeInfo& FoliageInfo : FoliageTypes)
            {
                UObject* FoliageTypeObj = StaticLoadObject(UObject::StaticClass(), nullptr, *FoliageInfo.FoliageTypePath);
                if (FoliageTypeObj)
                {
                    // Implement foliage spawning logic here
                    UE_LOG(LogTemp, Warning, TEXT("Loaded foliage type: %s"), *FoliageInfo.FoliageTypePath);
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to load foliage type: %s"), *FoliageInfo.FoliageTypePath);
                }
            }
        }

        // Apply landscape layers (if any are set)
        if (Landscape->GetLandscapeInfo() && !LandscapeLayers.IsEmpty())
        {
            for (auto& Layer : LandscapeLayers)
            {
                Landscape->GetLandscapeInfo()->GetLayerInfoByName(Layer->LayerName);
                // Paint the layers (logic for applying layers needs to be implemented)
            }
        }
    }
}

// void AWorldGenerator::GenerateWorldWithLandscape(int32 Width, int32 Height, float FillProbability, float MaxHeight, float NoiseScale)
// {
//     // Set up landscape configuration
//     int32 QuadsPerSection = 63;
//     int32 SectionsPerComponent = 1;
//     int32 ComponentsX = FMath::CeilToInt(static_cast<float>(Width) / (QuadsPerSection * SectionsPerComponent));
//     int32 ComponentsY = FMath::CeilToInt(static_cast<float>(Height) / (QuadsPerSection * SectionsPerComponent));
//
//     // Create heightmap data
//     TArray<uint16> HeightData;
//     HeightData.SetNumZeroed(Width * Height);
//
//     // Generate cellular automata grid
//     TArray<TArray<bool>> Grid;
//     GenerateCellularAutomataGrid(Grid, Width, Height, FillProbability);
//
//     // Apply heightmap data
//     for (int32 Y = 0; Y < Height; ++Y)
//     {
//         for (int32 X = 0; X < Width; ++X)
//         {
//             float NoiseValue = FMath::PerlinNoise2D(FVector2D(X * NoiseScale, Y * NoiseScale));
//             float CellularValue = Grid[Y][X] ? 1.0f : 0.0f;
//             float HeightValue = FMath::Lerp(0.0f, MaxHeight, (NoiseValue + CellularValue) * 0.5f);
//
//             // Convert height to uint16 (Unreal's heightmap format)
//             uint16 CompressedHeight = FMath::Clamp<uint16>(FMath::RoundToInt(HeightValue / MaxHeight * 65535.0f), 0, 65535);
//             HeightData[Y * Width + X] = CompressedHeight;
//         }
//     }
//
//     // Create the landscape
//     UWorld* World = GetWorld();
//     if (World)
//     {
//         ALandscape* Landscape = World->SpawnActor<ALandscape>(ALandscape::StaticClass(), FTransform::Identity);
//         if (Landscape)
//         {
//             FGuid LandscapeGuid = FGuid::NewGuid();
//
//             // Create a heightmap info object
//             TMap<FGuid, TArray<uint16>> HeightmapDataPerLayers;
//             HeightmapDataPerLayers.Add(LandscapeGuid, HeightData);
//
//             // Create layer info
//             TMap<FGuid, TArray<FLandscapeImportLayerInfo>> MaterialLayerDataPerLayer;
//             TArray<FLandscapeImportLayerInfo> LayerInfos;
//             FLandscapeImportLayerInfo LayerInfo;
//             LayerInfo.LayerName = TEXT("DefaultLayer");
//             LayerInfos.Add(LayerInfo);
//             MaterialLayerDataPerLayer.Add(LandscapeGuid, LayerInfos);
//
//             // Create the landscape
//             Landscape->CreateLandscapeInfo();
//             Landscape->Import(LandscapeGuid,
//                 0, 0, ComponentsX - 1, ComponentsY - 1,
//                 SectionsPerComponent, QuadsPerSection,
//                 HeightmapDataPerLayers, MaterialLayerDataPerLayer,
//                 ELandscapeImportAlphamapType::Additive);
//
//             // Optional: Add landscape material
//             // Landscape->LandscapeMaterial = Your landscape material asset;
//         }
//     }
// }
//
// void AWorldGenerator::GenerateCellularAutomataGrid(TArray<TArray<bool>>& Grid, int32 Width, int32 Height, float FillProbability)
// {
//     // Initialize grid
//     Grid.SetNum(Height);
//     for (auto& Row : Grid)
//     {
//         Row.SetNum(Width);
//         for (auto& Cell : Row)
//         {
//             Cell = FMath::RandRange(0.0f, 1.0f) < FillProbability;
//         }
//     }
//
//     // Apply cellular automata rules (same as before)
//     for (int32 Iteration = 0; Iteration < 5; ++Iteration)
//     {
//         TArray<TArray<bool>> NewGrid = Grid;
//         for (int32 Y = 0; Y < Height; ++Y)
//         {
//             for (int32 X = 0; X < Width; ++X)
//             {
//                 int32 AliveNeighbors = 0;
//                 for (int32 NY = -1; NY <= 1; ++NY)
//                 {
//                     for (int32 NX = -1; NX <= 1; ++NX)
//                     {
//                         if (NX == 0 && NY == 0) continue;
//                         int32 CheckX = X + NX;
//                         int32 CheckY = Y + NY;
//                         if (CheckX >= 0 && CheckX < Width && CheckY >= 0 && CheckY < Height)
//                         {
//                             AliveNeighbors += Grid[CheckY][CheckX] ? 1 : 0;
//                         }
//                     }
//                 }
//                 if (Grid[Y][X])
//                 {
//                     NewGrid[Y][X] = AliveNeighbors >= 4;
//                 }
//                 else
//                 {
//                     NewGrid[Y][X] = AliveNeighbors >= 5;
//                 }
//             }
//         }
//         Grid = NewGrid;
//     }
// }
