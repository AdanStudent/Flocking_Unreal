// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Public\Spawner.h"
#include "Engine/World.h"
#include "../Public/Tree/OctTree.h"
#include "Public/Agent/Agent.h"
#include "Public/Agent/SteeringComponent.h"
#include "DrawDebugHelpers.h"


// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ASpawner::RebuildTree()
{
	for (int i = 0; i < SpawnedUnits.Num(); i++)
	{
		FillTree(Tree, SpawnedUnits[i]->GetActorLocation(), SpawnedUnits[i]);
	}
}

void ASpawner::CheckForNearestNeighbors()
{
	for (int i = 0; i < SpawnedUnits.Num(); i++)
	{
		FRect Range = FRect(SpawnedUnits[i]->GetActorLocation(), FVector(750));

		TArray<UObject*> FoundPoints;
		Tree->Query(Range, FoundPoints);
		SpawnedUnits[i]->SetNeighbors(FoundPoints);
	}
}

void ASpawner::FillTree(UOctTree* Tree, FVector Location, UObject* Data)
{
	Tree->Insert(FPoint(Location, Data));
}

void ASpawner::SpawnActors()
{
	if (ActorToBeSpawned)
	{
		FRandomStream Rand;
		Rand.GenerateNewSeed();

		//int counter = 0;

		for (int32 i = 0; i < NumToSpawn; i++)
		{
			AAgent* SpawnedAgent;

			//Spawning Agent
			{
				//Setting up Parameters for Spawning
				FActorSpawnParameters sParameters;
				sParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				
				float range = 2000;
				//Determining its location
				FVector Loc = FVector(Rand.FRandRange(-range, range), Rand.FRandRange(-range, range), Rand.FRandRange(-range, range));

				//Spawning the AAgent
				SpawnedAgent = GetWorld()->SpawnActor<AAgent>(ActorToBeSpawned, Loc, FRotator::ZeroRotator, sParameters);

				//Setting the Steering Component it has with reference to itself
				SpawnedAgent->GetSteeringComp()->SetAgent(SpawnedAgent);
			}


			//Add Agent to the Agents list
			SpawnedUnits.Add(SpawnedAgent);

			//Adding the point of the agent to list of agents
			{
				if (Tree->Insert(FPoint(SpawnedAgent->GetActorLocation(), SpawnedAgent)))
				{
					//logging to make sure the correct amount of insertions is happening
					//counter++;
				}
			}
		}

		//UE_LOG(LogTemp, Warning, TEXT("Number of inserts: %d"), counter)
	}
}


// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
	SetupTree();
	
	//Spawning all the Actors
	SpawnActors();
}

void ASpawner::SetupTree()
{
	//some text here
	Tree = NewObject<UOctTree>();

	Tree->Boundary.Size = FVector(2000);
	//Setting the Tree's reference for the World's Level
	Tree->NewMapWorld = GetWorld();
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetupTree();
	RebuildTree();
	CheckForNearestNeighbors();


}

