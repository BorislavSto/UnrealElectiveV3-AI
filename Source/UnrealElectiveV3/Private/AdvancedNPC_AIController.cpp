// Fill out your copyright notice in the Description page of Project Settings.

#include "AdvancedNPC_AIController.h"
#include "AdvancedNPC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "UnrealElectiveV3/UnrealElectiveV3Character.h"

AAdvancedNPC_AIController::AAdvancedNPC_AIController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
}

void AAdvancedNPC_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AAdvancedNPC* const npc = Cast<AAdvancedNPC>(InPawn)) 
	{
		if(UBehaviorTree* const tree = npc->GetBehaviorTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(tree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(tree);
			UE_LOG(LogTemp, Warning, TEXT("BehaviorTree or BlackboardComponent is not missing!"));
		}
	}
}

void AAdvancedNPC_AIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	UE_LOG(LogTemp, Log, TEXT("AAdvancedNPC_AIController before SightConfig"));
	if (SightConfig)
	{
		UE_LOG(LogTemp, Log, TEXT("AAdvancedNPC_AIController after SightConfig"));

		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(
			TEXT("PerceptionComponent")));
		SightConfig->SightRadius = 500.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		SightConfig->PeripheralVisionAngleDegrees = 90.f;
		SightConfig->SetMaxAge(5.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAdvancedNPC_AIController::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
}

void AAdvancedNPC_AIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (AUnrealElectiveV3Character* const ch = Cast<AUnrealElectiveV3Character>(Actor))
	{
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
	}
}
