// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WalkToPointsAI.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "AdvancedNPC.generated.h"

UCLASS()
class UNREALELECTIVEV3_API AAdvancedNPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAdvancedNPC();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UBehaviorTree* GetBehaviorTree() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	UBehaviorTree* Tree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	AWalkToPointsAI* WalkToPoints;

};
