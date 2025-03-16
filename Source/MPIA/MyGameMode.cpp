// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "MyGameMode.h"

void AMyGameMode::BeginPlay()
{
    Super::BeginPlay();

    for (int32 i = 0; i < GoalCount; i++)
    {
        SpawnGoal();
    }
}

void AMyGameMode::SpawnGoal()
{
    APathNode* Node = GetRandomFreePathNode();
    if (!Node || !GoalClass) return;

    FActorSpawnParameters SpawnParams;
    AActor* SpawnedGoal = GetWorld()->SpawnActor<AActor>(GoalClass, Node->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);

    if (SpawnedGoal)
    {
        SpawnedGoal->OnDestroyed.AddDynamic(this, &AMyGameMode::OnGoalDestroyed);
    }
}

void AMyGameMode::OnGoalDestroyed(AActor* DestroyedActor)
{
    // Délai de respawn
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, this, &AMyGameMode::SpawnGoal, 3.0f, false); // 3 secondes d'attente
}

APathNode* AMyGameMode::GetRandomFreePathNode()
{
    TArray<APathNode*> FreeNodes;

    for (TActorIterator<APathNode> It(GetWorld()); It; ++It)
    {
        APathNode* Node = *It;
        if (!Node || !Node->bCanSpawnGoal) continue; // <-- On skip les nodes interdits

        FVector NodeLocation = Node->GetActorLocation();
        bool bOccupied = false;

        TArray<AActor*> AllActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);

        for (AActor* Actor : AllActors)
        {
            if (!Actor || Actor == Node) continue;

            float Dist = FVector::Dist(NodeLocation, Actor->GetActorLocation());

            if (Dist < 100.f) // Ajuste ce rayon selon tes besoins
            {
                bOccupied = true;
                break;
            }
        }

        if (!bOccupied)
        {
            FreeNodes.Add(Node);
        }
    }

    if (FreeNodes.Num() > 0)
    {
        int32 Index = FMath::RandRange(0, FreeNodes.Num() - 1);
        return FreeNodes[Index];
    }

    return nullptr;
}


