// Fill out your copyright notice in the Description page of Project Settings.

#include "EngineUtils.h"
#include "EnemyAIController.h"

TArray<APathNode*> AEnemyAIController::FindPath(APathNode* Start, APathNode* Goal)
{
    if (!Start || !Goal) return {};

    TMap<APathNode*, float> Distances;
    TMap<APathNode*, APathNode*> Previous;
    TSet<APathNode*> Visited;
    TArray<APathNode*> Unvisited;

    // Init : ajouter tous les nodes avec distance infinie
    for (TObjectIterator<APathNode> It; It; ++It)
    {
        APathNode* Node = *It;
        if (Node)
        {
            Distances.Add(Node, FLT_MAX);
            Unvisited.Add(Node);
        }
    }

    // Mettre la distance du start à 0
    Distances[Start] = 0.f;

    while (Unvisited.Num() > 0)
    {
        // Trouver le noeud avec la plus petite distance
        APathNode* Current = nullptr;
        float MinDist = FLT_MAX;

        for (APathNode* Node : Unvisited)
        {
            float* Dist = Distances.Find(Node);
            if (Dist && *Dist < MinDist)
            {
                MinDist = *Dist;
                Current = Node;
            }
        }

        if (!Current)
            break;

        Unvisited.Remove(Current);
        Visited.Add(Current);

        if (Current == Goal)
            break;

        for (APathNode* Neighbor : Current->ConnectedNodes)
        {
            if (!Neighbor || Visited.Contains(Neighbor)) continue;

            // Ajout pour éviter crash : forcer ajout si jamais le voisin n'est pas dans Distances
            if (!Distances.Contains(Neighbor))
            {
                Distances.Add(Neighbor, FLT_MAX);
                Unvisited.Add(Neighbor);
            }

            float CurrentDist = Distances[Current];
            float NewDist = CurrentDist + FVector::Dist(Current->GetActorLocation(), Neighbor->GetActorLocation());

            if (NewDist < Distances[Neighbor])
            {
                Distances[Neighbor] = NewDist;
                Previous.Add(Neighbor, Current);
            }
        }

    }

    if (!Previous.Contains(Goal))
    {
        // Aucun chemin trouvé
        return {};
    }

    // Reconstruction du chemin
    TArray<APathNode*> Path;
    APathNode* Current = Goal;

    while (Current)
    {
        Path.Insert(Current, 0);

        APathNode** Prev = Previous.Find(Current);
        if (!Prev) break;

        Current = *Prev;
    }

    return Path;
}

APathNode* AEnemyAIController::GetClosestPathNode(AActor* FromActor)
{
    if (!FromActor) return nullptr;

    UWorld* World = FromActor->GetWorld();
    if (!World) return nullptr;

    APathNode* ClosestNode = nullptr;
    float ClosestDistanceSq = FLT_MAX;

    for (TActorIterator<APathNode> It(World); It; ++It)
    {
        APathNode* Node = *It;
        if (!Node) continue;

        float DistSq = FVector::DistSquared(FromActor->GetActorLocation(), Node->GetActorLocation());
        UE_LOG(LogTemp, Warning, TEXT("Dist entre %s et %s: %.2f"),*FromActor->GetActorLabel(), *Node->GetActorLabel(), DistSq);

        if (DistSq < ClosestDistanceSq)
        {
            ClosestDistanceSq = DistSq;
            ClosestNode = Node;
        }
    }

    return ClosestNode;
}


bool AEnemyAIController::IsPathNodeTargeted(
    APathNode* Node,
    const TArray<AActor*>& ActorsToCheck,
    float MaxDistance,
    float MaxAngleDegrees
)
{
    if (!Node) return false;

    FVector NodeLocation = Node->GetActorLocation();

    for (AActor* Actor : ActorsToCheck)
    {
        if (!Actor) continue;

        FVector ToNode = (NodeLocation - Actor->GetActorLocation());
        float Distance = ToNode.Size();

        if (Distance > MaxDistance) continue;

        FVector ActorForward = Actor->GetActorForwardVector();
        float Angle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(ActorForward.GetSafeNormal(), ToNode.GetSafeNormal())));

        UE_LOG(LogTemp, Warning, TEXT("Actor: %s | Dist: %.1f | Angle: %.1f"), *Actor->GetName(), Distance, Angle);


        if (Angle <= MaxAngleDegrees)
        {
            return true;
        }
    }

    return false;
}

TArray<APathNode*> AEnemyAIController::FindPathWithBlockedNodes(
    APathNode* Start,
    APathNode* Goal,
    const TSet<APathNode*>& BlockedNodes
)
{
    if (!Start || !Goal) return {};

    TMap<APathNode*, float> Distances;
    TMap<APathNode*, APathNode*> Previous;
    TSet<APathNode*> Visited;
    TArray<APathNode*> Unvisited;

    for (TObjectIterator<APathNode> It; It; ++It)
    {
        APathNode* Node = *It;
        if (Node && !BlockedNodes.Contains(Node))
        {
            Distances.Add(Node, FLT_MAX);
            Unvisited.Add(Node);
        }
    }

    if (!Distances.Contains(Start) || !Distances.Contains(Goal))
        return {};

    Distances[Start] = 0.f;

    while (Unvisited.Num() > 0)
    {
        APathNode* Current = nullptr;
        float MinDist = FLT_MAX;

        for (APathNode* Node : Unvisited)
        {
            float* Dist = Distances.Find(Node);
            if (Dist && *Dist < MinDist)
            {
                MinDist = *Dist;
                Current = Node;
            }
        }

        if (!Current) break;

        Unvisited.Remove(Current);
        Visited.Add(Current);

        if (Current == Goal) break;

        for (APathNode* Neighbor : Current->ConnectedNodes)
        {
            if (!Neighbor || Visited.Contains(Neighbor) || BlockedNodes.Contains(Neighbor)) continue;

            float CurrentDist = Distances[Current];
            float NewDist = CurrentDist + FVector::Dist(Current->GetActorLocation(), Neighbor->GetActorLocation());

            if (NewDist < Distances[Neighbor])
            {
                Distances[Neighbor] = NewDist;
                Previous.Add(Neighbor, Current);
            }
        }
    }

    if (!Previous.Contains(Goal)) return {};

    // Reconstruire chemin
    TArray<APathNode*> Path;
    APathNode* Current = Goal;

    while (Current)
    {
        Path.Insert(Current, 0);
        APathNode** Prev = Previous.Find(Current);
        if (!Prev) break;
        Current = *Prev;
    }

    return Path;
}




