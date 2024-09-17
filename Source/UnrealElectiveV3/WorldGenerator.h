// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent/Public/ProceduralMeshComponent.h"
#include "WorldGenerator.generated.h"

UCLASS()
class UNREALELECTIVEV3_API AWorldGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldGenerator();

	UFUNCTION(BlueprintCallable, Category = "World Generation")
	static AWorldGenerator* GetInstance(UWorld* World);

	UFUNCTION(BlueprintCallable, Category = "World Generation")
	void GenerateWorldPerlinNoise(int32 Width, int32 Height, float Scale);

	UFUNCTION(BlueprintCallable, Category = "World Generation")
	void GenerateWorldDiamondSquare(int32 Size, float Roughness);

	UFUNCTION(BlueprintCallable, Category = "World Generation")
	void GenerateWorldCellularAutomata(int32 Width, int32 Height, float FillProbability, float MaxHeight, float NoiseScale);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	static AWorldGenerator* Instance;

	UPROPERTY()
	UProceduralMeshComponent* MeshComponent;

	void CreateMesh(const TArray<FVector>& Vertices, const TArray<int32>& Triangles);

};
