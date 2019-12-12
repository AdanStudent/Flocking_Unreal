// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SteeringComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FLOCKING_API USteeringComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USteeringComponent();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Steering Behaviors")
		FVector SteeringForce;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Steering Behaviors")
		float CircleRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Steering Behaviors")
		float WanderDist;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Steering Behaviors")
		float WanderJitterPerSecond;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Steering Behaviors")
		float DesiredSeparation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Steering Behaviors")
		float DesiredAlignment;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Steering Behaviors")
		float DesiredAlignment;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Steering Behaviors")
		class AAgent* Agent;

	FRandomStream RandStream;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetAgent(AAgent* OwningAgent);

	AAgent* GetAgent() const { return Agent; };

	//used to update the calculations of this component
	void UpdateForces();
	

private:

	//Steering Behaviors needed for Flocking simulation
	
	//Finds and Returns FVector needed to reach the Target
	FVector Seek(const FVector Target);
	
	//Finds a random point infront of the Agent this Component is attached to and Seek() that Target point
	FVector Wander();

	//With all its neighbors in its seperation range it will steer away from them to not collide
	FVector Seperation();

	//With all neighbors in a range finds the direction the general sum of them are going and adjusts its steering
	FVector Alignment();

	//All Neighbors in range finds their weighted center and trys to stay within the group of Neighbors
	FVector Cohesion();

	//used to have all the Agents as they are moving around the level to wrap around the designated space
	void WrapAroundWorld();



};
