// Fill out your copyright notice in the Description page of Project Settings.


#include "Public\Agent\SteeringComponent.h"
#include "Public\Agent\Agent.h"

// Sets default values for this component's properties
USteeringComponent::USteeringComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void USteeringComponent::SetAgent(AAgent* OwningAgent)
{
}

// Called when the game starts
void USteeringComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USteeringComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USteeringComponent::UpdateForces()
{
}

FVector USteeringComponent::Seek(const FVector Target)
{
	return FVector();
}

FVector USteeringComponent::Wander()
{
	return FVector();
}

FVector USteeringComponent::Seperation()
{
	return FVector();
}

FVector USteeringComponent::Alignment()
{
	return FVector();
}

FVector USteeringComponent::Cohesion()
{
	return FVector();
}

void USteeringComponent::WrapAroundWorld()
{
}

