// Fill out your copyright notice in the Description page of Project Settings.

#include "GameManager.h"
#include "WorldGen.h"

// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	
    InitializeGame();
}


// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameManager::InitializeGame()
{
   // CreateWorldGenerator();
    //if (WorldGenerator)
    //{
    //    //UWorldGen* Generator = NewObject<UWorldGen>(this);

    //    // Choose one of these:
    //    //WorldGenerator->GenerateWorldPerlinNoise(100, 100, 0.01f);
    //    //Generator->GenerateWorldDiamondSquare(7, 1.0f);
    //    //Generator->GenerateWorldCellularAutomata(100, 100, 0.45f);
    //}
}
void AGameManager::CreateWorldGenerator()
{
    if (!WorldGenerator)
    {
        WorldGenerator = NewObject<UWorldGen>(this);
    }
}

UWorldGen* AGameManager::GetWorldGenerator()
{
    return WorldGenerator;
}
