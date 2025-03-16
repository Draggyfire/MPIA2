// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PathNode.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MPIA_API AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
    virtual void BeginPlay() override;

    void SpawnGoal();
    APathNode* GetRandomFreePathNode();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    TSubclassOf<AActor> GoalClass;

    UPROPERTY(EditAnywhere)
    int32 GoalCount = 2;

    UFUNCTION(BlueprintCallable)
    void OnGoalDestroyed(AActor* DestroyedActor);
};
