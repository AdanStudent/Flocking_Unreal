// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Agent.generated.h"

class USteeringComponent;

UCLASS()
class FLOCKING_API AAgent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAgent();


private:
	//Used for what direction the Agent is moving towards
	UPROPERTY(EditDefaultsOnly, Category = "Steering Behaviors")
	FVector Direction;

	//a normalized Vector of the Direction
	UPROPERTY(EditDefaultsOnly, Category = "Steering Behaviors")
	FVector Heading;

	//Determines its Mass
	UPROPERTY(EditDefaultsOnly, Category = "Steering Behaviors")
	float Mass;

	//Determines what the fastest it can go.
	UPROPERTY(EditDefaultsOnly, Category = "Steering Behaviors")
	float MaxSpeed;

	//Used to keep reference of all the States it would be handling
	UPROPERTY(EditDefaultsOnly, Category = "Steering Behaviors")
	TArray<class UState*> SteeringState;

	//A reference of all its neighbors to check against for the flocking behaviors
	UPROPERTY(EditAnywhere, Category = "Steering Behaviors")
	TArray<UObject*> Neighbors;

	//Runs all steering behaviors for this agent
	USteeringComponent* SteeringComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	USteeringComponent* GetSteeringComp() { return SteeringComp; }

	const TArray<UObject*> GetNeighbors() { return Neighbors; }

	FVector GetDirection() { return Direction; }
	void SetDirection(FVector Value) { Direction = Value; }

	FVector GetHeading() { return Heading; }
	void SetHeading(FVector Value) { Heading = Value; }

	const float GetMass() { return Mass; }
	const float GetMaxSpeed() { return MaxSpeed; }

	//Setting all its Neighbors
	void SetNeighbors(TArray<UObject*> OtherAgents);
	
	//Call the SteeringBehaviors run
	void RunSteeringBehaviors();

private:

	//Clearing the Neighbors every frame after the calculations are done
	void ClearNeighbors();

};
