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
	UPROPERTY(EditDefaultsOnly, Category = "Steering Behaviors")
	FVector Direction;

	UPROPERTY(EditDefaultsOnly, Category = "Steering Behaviors")
	FVector Heading;

	UPROPERTY(EditDefaultsOnly, Category = "Steering Behaviors")
	float Mass;

	UPROPERTY(EditDefaultsOnly, Category = "Steering Behaviors")
	float MaxSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Steering Behaviors")
	float MaxForce;

	UPROPERTY(EditDefaultsOnly, Category = "Steering Behaviors")
	TArray<class UState*> SteeringState;

	UPROPERTY(EditAnywhere, Category = "Steering Behaviors")
	TArray<AAgent*> Neighbors;

	//Placeholder
	//AIManager

	//Runs all steering behaviors for this agent
	USteeringComponent* SteeringComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//SetAIManager();

	USteeringComponent* GetSteeringComp() { return SteeringComp; }

	const TArray<AAgent*> GetNeighbors() { return Neighbors; }

	FVector GetDirection() { return Direction; }
	void SetDirection(FVector Value) { Direction = Value; }

	FVector GetHeading() { return Heading; }
	void SetHeading(FVector Value) { Heading = Value; }

	const float GetMass() { return Mass; }
	const float GetMaxSpeed() { return MaxSpeed; }
	const float GetMaxForce() { return MaxForce; }

	void SetNeighbors(TArray<AAgent*> OtherAgents);
	void RunSteeringBehaviors();

private:
	void ClearNeighbors();

};
