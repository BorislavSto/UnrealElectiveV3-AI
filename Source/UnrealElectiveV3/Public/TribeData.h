// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TribeData.generated.h"

UENUM(BlueprintType)
enum class ETribeKind : uint8
{
	Hunter UMETA(DisplayName = "Hunter"),
	Gatherer UMETA(DisplayName = "Gatherer"),
	Farmer UMETA(DisplayName = "Farmer"),
	// Add more as needed
};

UENUM(BlueprintType)
enum class ERelationType : uint8
{
	Friend UMETA(DisplayName = "Friend"),
	Enemy UMETA(DisplayName = "Enemy"),
	Trade UMETA(DisplayName = "Trade"),
	Neutral UMETA(DisplayName = "Neutral")
};

USTRUCT(BlueprintType)
struct FTribePreset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ATribe> TribeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MemberCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETribeKind Kind;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* HutMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* MemberMesh;
};

UCLASS(Blueprintable)
class UNREALELECTIVEV3_API UTribeData : public UObject 
{
	GENERATED_BODY()
	
public:
	UTribeData();
	~UTribeData();
};
