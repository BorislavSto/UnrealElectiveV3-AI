// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ChasePlayer.h"

#include "AdvancedNPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UBTTask_ChasePlayer::UBTTask_ChasePlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Chase Player";
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get target location from the blackboard via the NPC's Controller
	if(AAdvancedNPC_AIController* const cont = Cast<AAdvancedNPC_AIController>(OwnerComp.GetAIOwner()))
	{
		FVector const PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

		// move to the player's location
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, PlayerLocation);

		// finish with success
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
