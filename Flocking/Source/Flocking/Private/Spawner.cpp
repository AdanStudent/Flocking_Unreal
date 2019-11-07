// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Public\Spawner.h"
#include "Engine/World.h"
#include "../Public/Tree/OctTree.h"


// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tree = CreateDefaultSubobject<UOctTree>(TEXT("Tree"));

	auto t = Tree->Boundary;
}

void ASpawner::SpawnActors()
{
	if (ActorToBeSpawned)
	{
		FRandomStream Rand;
		Rand.GenerateNewSeed();

		for (int32 i = 0; i < NumToSpawn; i++)
		{
			//Spawning Agent
			FActorSpawnParameters sParameters;
			sParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			float range = Rand.FRandRange(0.f, 2000.f);

			FVector Loc = Rand.GetUnitVector() * range;

			AActor* Agent = GetWorld()->SpawnActor<AActor>(ActorToBeSpawned, Loc, FRotator::ZeroRotator, sParameters);

			//Add Agent to the Agents list
			SpawnedUnits.Add(Agent);

			//Adding the point of the agent to list of agents
			FPoint Point;

			Point.Location = Agent->GetActorLocation();
			Point.Data = Agent;

			Tree->Insert(Point);
		}
	}
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
	//Tree->Setup();
	
	SpawnActors();
	Tree->Display();
	
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

