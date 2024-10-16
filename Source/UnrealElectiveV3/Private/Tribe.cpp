// Fill out your copyright notice in the Description page of Project Settings.


#include "Tribe.h"

#include "AdvancedNPC_AIController.h"

// Sets default values
ATribe::ATribe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATribe::BeginPlay()
{
	Super::BeginPlay();
}

void ATribe::InitializeFromPreset(const FTribePreset& Preset)
{
	TribeKind = Preset.Kind;
	MemberCount = Preset.MemberCount;
	HutMesh = Preset.HutMesh;
	NPCBlueprint = Preset.NPCBlueprint;

	for (int32 i = 0; i < MemberCount; ++i)
	{
		FActorSpawnParameters SpawnParams;
		AAdvancedNPC* npc = GetWorld()->SpawnActor<AAdvancedNPC>(NPCBlueprint, Preset.SpawnLocation, FRotator::ZeroRotator, SpawnParams);

		if (AAdvancedNPC_AIController* controller = GetWorld()->SpawnActor<AAdvancedNPC_AIController>(AAdvancedNPC_AIController::StaticClass(), Preset.SpawnLocation, FRotator::ZeroRotator, SpawnParams))
			controller->Possess(npc);
	}
}

void ATribe::SetRelationWithTribe(ATribe* OtherTribe, ERelationType Relation)
{
	if (OtherTribe && OtherTribe != this)
	{
		Relations.Add(OtherTribe, Relation);
	}
}

ERelationType ATribe::GetRelationWithTribe(ATribe* OtherTribe) const
{
	if (OtherTribe && Relations.Contains(OtherTribe))
	{
		return Relations[OtherTribe];
	}
	return ERelationType::Neutral;
}
