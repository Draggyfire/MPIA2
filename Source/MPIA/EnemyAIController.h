// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PathNode.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class MPIA_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Pathfinding")
	TArray<APathNode*> FindPath(APathNode* Start, APathNode* Goal);

	UFUNCTION(BlueprintCallable, Category = "Pathfinding")
	static APathNode* GetClosestPathNode(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Pathfinding")
	bool IsPathNodeTargeted(
		APathNode* Node,
		const TArray<AActor*>& ActorsToCheck,
		float MaxDistance,
		float MaxAngleDegrees
	);
	UFUNCTION(BlueprintCallable, Category = "Pathfinding")
	TArray<APathNode*> FindPathWithBlockedNodes(
		APathNode* Start,
		APathNode* Goal,
		const TSet<APathNode*>& BlockedNodes
	);


};
