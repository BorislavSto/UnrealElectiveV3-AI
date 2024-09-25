// Fill out your copyright notice in the Description page of Project Settings.

#include "AdvancedNPC_AIController.h"
#include "AdvancedNPC.h"

AAdvancedNPC_AIController::AAdvancedNPC_AIController(FObjectInitializer const& ObjectInitializer)
{
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
		}
	}
}
