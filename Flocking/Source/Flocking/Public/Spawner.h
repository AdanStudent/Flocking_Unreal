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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	UOctTree* Tree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TArray<UOctTree*> TempTrees;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TArray<AAgent*> SpawnedUnits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<AAgent> ActorToBeSpawned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 NumToSpawn;

	void FillTree(FVector Location, UObject* Data);
	void SpawnActors();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
