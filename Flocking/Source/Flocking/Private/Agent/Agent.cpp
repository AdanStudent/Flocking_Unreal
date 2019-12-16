// Fill out your copyright notice in the Description page of Project Settings.


#include "Public\Agent\Agent.h"
#include "Public\Agent\SteeringComponent.h"

// Sets default values
AAgent::AAgent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Allocating memory for the SteeringComponent
	SteeringComp = CreateDefaultSubobject<USteeringComponent>("Steering Comp");

	//Setup mass and MaxSpeed
	Mass = 10.f;
	MaxSpeed = 1000.f;
}


// Called when the game starts or when spawned
void AAgent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Update all the steering behaviors
	SteeringComp->UpdateForces(DeltaTime);

	//Then clear all the Neighbors for next update
	ClearNeighbors();
}

void AAgent::ClearNeighbors()
{
	Neighbors.Empty();
}

void AAgent::SetNeighbors(TArray<UObject*> OtherAgents)
{
	Neighbors = OtherAgents;
}

