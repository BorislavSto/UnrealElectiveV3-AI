// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindPlayerLocation.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Find Player Location";
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get player character
	UE_LOG(LogTemp, Log, TEXT("FindPlayerLocation 1"));
	if (ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		UE_LOG(LogTemp, Log, TEXT("FindPlayerLocation 2"));

		// get player location to use as an origin
		FVector const PlayerLocation = Player->GetActorLocation();
		if (SearchRandom)
		{
			FNavLocation Loc;
			UE_LOG(LogTemp, Log, TEXT("FindPlayerLocation 3"));

			// get the navigation system and generate a random location near the player
			if (UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
					UE_LOG(LogTemp, Log, TEXT("FindPlayerLocation 4"));

				// try to get a random location near the player
				if (NavSys->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, Loc))
				{
					UE_LOG(LogTemp, Log, TEXT("FindPlayerLocation 5"));

					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("FindPlayerLocation 6"));

			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
