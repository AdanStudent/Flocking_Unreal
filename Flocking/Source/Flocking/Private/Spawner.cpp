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

	//some text here
	Tree = CreateDefaultSubobject<UOctTree>(TEXT("Tree"));
	Tree->SpawnerRef = this;
	Tree->Boundary.Size = FVector(2000);

	//creating a list of empty OctTree's to be able to call from when a divide is called
	for (int32 i = 0; i < 4096; i++)
	{
		FName name = FName("Child", i);
		UOctTree* Temp = CreateDefaultSubobject<UOctTree>(name);
		TempTrees.Add(Temp);
	}
}

void ASpawner::FillTree(FVector Location, UObject* Data)
{
	Tree->Insert(FPoint(Location, Data));
}

void ASpawner::SpawnActors()
{
	if (ActorToBeSpawned)
	{
		FRandomStream Rand;
		Rand.GenerateNewSeed();

		int counter = 0;

		for (int32 i = 0; i < NumToSpawn; i++)
		{
			AAgent* SpawnedAgent;

			//Spawning Agent
			{
				//Setting up Parameters for Spawning
				FActorSpawnParameters sParameters;
				sParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				
				//Determining its location
				float range = Rand.FRandRange(1.f, 2000.f);
				FVector Loc = Rand.GetUnitVector() * range;

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
					counter++;
				}
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("Number of inserts: %d"), counter)
	}
}


// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
	//Setting the Tree's reference for the World's Level
	Tree->NewMapWorld = GetWorld();
	
	//Spawning all the Actors
	SpawnActors();


}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

