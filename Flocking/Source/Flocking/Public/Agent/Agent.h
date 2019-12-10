// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Agent.generated.h"

UCLASS()
class FLOCKING_API AAgent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAgent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Steering Behaviors")
	FVector Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Steering Behaviors")
	FVector Heading;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Steering Behaviors")
	float Mass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Steering Behaviors")
	float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Steering Behaviors")
	float MaxForce;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Steering Behaviors")
	TArray<class UState> SteeringState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Steering Behaviors")
	TArray<AAgent*> Neighbors;

	//Placeholder
	//AIManager
	//SetAIManager();

	//SteeringComp
	//GetSteeringComp();





protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void RunSteeringBehaviors();

	void ClearNeighbors();

	void SetNeighbors(TArray<AAgent*> OtherAgents);

};
