// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Public\Spawner.h"
#include "Engine/World.h"
#include "../Public/Tree/OctTree.h"
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
	for (int32 i = 0; i < 350; i++)
	{
		FName name = FName("Child", i);
		UOctTree* Temp = CreateDefaultSubobject<UOctTree>(name);
		TempTrees.Add(Temp);
	}
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
			//Spawning Agent
			FActorSpawnParameters sParameters;
			sParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			float range = Rand.FRandRange(1.f, 2000.f);

			FVector Loc = Rand.GetUnitVector() * range;

			AActor* Agent = GetWorld()->SpawnActor<AActor>(ActorToBeSpawned, Loc, FRotator::ZeroRotator, sParameters);

			//Add Agent to the Agents list
			SpawnedUnits.Add(Agent);

			//Adding the point of the agent to list of agents
			FPoint Point;

			Point.Location = Agent->GetActorLocation();
			Point.Data = Agent;

			if (Tree->Insert(Point))
			{
				counter++;
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("Number of inserts: %d"), counter)
	}
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
	Tree->NewMapWorld = GetWorld();
	SpawnActors();
	Tree->Display();
	int count = Tree->HowManyChildren();

	UE_LOG(LogTemp, Warning, TEXT("Number of children: %d"), count);
	
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

