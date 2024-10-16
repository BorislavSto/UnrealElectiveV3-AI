// Fill out your copyright notice in the Description page of Project Settings.

#include "GameManager.h"
#include "Tribe.h"

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
	
	InitializeTribes();
	InitializeGame();
}


// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameManager::InitializeGame()
{

}
	
void AGameManager::InitializeTribes()
{
	for (const FTribePreset& Preset : TribePresets)
	{
		FActorSpawnParameters SpawnParams;
		ATribe* NewTribe = GetWorld()->SpawnActor<ATribe>(Preset.TribeClass, Preset.SpawnLocation, FRotator::ZeroRotator, SpawnParams);
        
		if (NewTribe)
		{
			NewTribe->InitializeFromPreset(Preset);
			Tribes.Add(NewTribe);
		}
	}
}

void AGameManager::GenerateLandscape()
{
	// Implement landscape generation logic
}

void AGameManager::SetupTribalRelations()
{
	for (int i = 0; i < Tribes.Num(); ++i)
	{
		for (int j = i + 1; j < Tribes.Num(); ++j)
		{
			ERelationType InitialRelation = ERelationType::Neutral; // You can randomize this or set based on some logic
			Tribes[i]->SetRelationWithTribe(Tribes[j], InitialRelation);
			Tribes[j]->SetRelationWithTribe(Tribes[i], InitialRelation);
		}
	}
}