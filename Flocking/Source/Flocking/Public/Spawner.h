// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

class UOctTree;
class AAgent;

UCLASS()
class FLOCKING_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

	//Reference of OctTree to be built
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	UOctTree* Tree;

	//What AAgent based class should be spawned
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<AAgent> ActorToBeSpawned;

	//Collection of all Spawned Units
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TArray<AAgent*> SpawnedUnits;

	//how many to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 NumToSpawn;

private:

	//go through all the SpawnedUnits and call FillTree for each
	void RebuildTree();

	//Query Tree for each of the Spawned Units
	void CheckForNearestNeighbors();
	
	//Used to Call insert for the specified Tree, with the specific location and UObject*
	void FillTree(UOctTree* Tree, FVector Location, UObject* Data);

	//Spawning the Actors
	void SpawnActors();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//used to allocate new memory for the tree this frame, with specified size 

	void SetupTree();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
