// Fill out your copyright notice in the Description page of Project Settings.


#include "Public\Agent\SteeringComponent.h"
#include "Public\Agent\Agent.h"
#include "Kismet\KismetMathLibrary.h"
#include "DrawDebugHelpers.h"


// Sets default values for this component's properties
USteeringComponent::USteeringComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	CircleRadius = 50;
	WanderDist = 80;
	WanderJitterPerSecond = 0.3f;

	DesiredSeparation = 150.f;
	DesiredAlignment = 500.f;
	DesiredCohesion = 750.f;

	RandStream.GenerateNewSeed();

	// ...
}


void USteeringComponent::SetAgent(AAgent* OwningAgent)
{
	if (OwningAgent)
	{
		Agent = OwningAgent;
	}
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

void USteeringComponent::UpdateForces(float DeltaTime)
{
	if (Agent)
	{

		SteeringForce += (Seperation() * 10); 
		SteeringForce += (Alignment()); 
		SteeringForce += (Cohesion()); 
		SteeringForce += (Wander()); 


		if (SteeringForce.Size() > 0.0001f)
		{
			Acceleration = SteeringForce / Agent->GetMass();
			Agent->SetDirection(Agent->GetDirection() + Acceleration * DeltaTime);

			Agent->SetHeading(Agent->GetDirection().GetSafeNormal());

			FVector NewLocation = Agent->GetActorLocation();

			NewLocation += Agent->GetDirection() * DeltaTime;
			Agent->SetActorLocation(NewLocation);

			FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Agent->GetActorRotation().Vector(), Agent->GetDirection());
			Agent->SetActorRotation(Rotation);

			SteeringForce = FVector::ZeroVector;

		}

		WrapAroundWorld();

	}
}

FVector USteeringComponent::Seek(const FVector Target)
{
	FVector DesiredVelocity = FVector(Target - Agent->GetActorLocation());

	DesiredVelocity = DesiredVelocity.GetSafeNormal();
	DesiredVelocity *= Agent->GetMaxSpeed();


	return (DesiredVelocity - Agent->GetDirection());
}

FVector USteeringComponent::Wander()
{
	WanderTheta += RandStream.FRandRange(-WanderJitterPerSecond, WanderJitterPerSecond);
	FVector Heading = Agent->GetHeading();

	FVector CirclePos = Heading;
	CirclePos *= WanderDist;
	CirclePos += Agent->GetActorLocation();


	float h = FMath::Acos(FVector::DotProduct(FVector::ZeroVector, Heading));
	h = FMath::RadiansToDegrees(h);

	float XOffset = CircleRadius * FMath::Cos(WanderTheta + h);
	float YOffset = CircleRadius * FMath::Sin(WanderTheta + h);
	float ZOffset = CircleRadius * FMath::Tan(WanderTheta + h);

	FVector Target = CirclePos + FVector(XOffset, YOffset, ZOffset);

	FVector WanderForce = Seek(Target);

	return WanderForce;
}

FVector USteeringComponent::Seperation()
{
	FVector Sum;
	int32 Count = 0;

	for (int32 i = 0; i < Agent->GetNeighbors().Num(); i++)
	{
		FVector Loc = Agent->GetActorLocation();

		if (auto OtherAgent = Cast<AAgent>(Agent->GetNeighbors()[i]))
		{
			FVector OtherLoc = OtherAgent->GetActorLocation();

			float Distance = FVector::Dist(Loc, OtherLoc);

			if ((Distance > 0) && (Distance < DesiredSeparation))
			{
				FVector Difference = Loc - OtherLoc;
				Difference = Difference.GetUnsafeNormal();
				Difference /= Distance;
				Sum += Difference;
				Count++;
			}
		}
	}

	FVector SeperationForce = FVector::ZeroVector;

	if (Count > 0)
	{
		Sum /= Count;
		Sum = Sum.GetUnsafeNormal();
		Sum *= Agent->GetMaxSpeed();
		SeperationForce = Sum - Agent->GetDirection();
	}

	return SeperationForce;
}

FVector USteeringComponent::Alignment()
{
	int32 Count = 0;
	FVector Sum;

	for (int32 i = 0; i < Agent->GetNeighbors().Num(); i++)
	{
		FVector Loc = Agent->GetActorLocation();

		if (auto OtherAgent = Cast<AAgent>(Agent->GetNeighbors()[i]))
		{
			FVector OtherLoc = OtherAgent->GetActorLocation();

			float Distance = FVector::Dist(Loc, OtherLoc);

			if ((Distance > 0) && (Distance < DesiredAlignment) && Count < 30)
			{
				Sum += OtherAgent->GetDirection();
				Count++;
			}
		}
	}

	FVector AlignmentForce = FVector::ZeroVector;

	if (Count > 0)
	{
		Sum /= Count;
		Sum = Sum.GetSafeNormal();
		Sum *= Agent->GetMaxSpeed();
		AlignmentForce = Sum - Agent->GetDirection();
	}

	return AlignmentForce;
}

FVector USteeringComponent::Cohesion()
{
	int32 Count = 0;
	FVector Sum;

	for (int32 i = 0; i < Agent->GetNeighbors().Num(); i++)
	{
		FVector Loc = Agent->GetActorLocation();

		if (auto OtherAgent = Cast<AAgent>(Agent->GetNeighbors()[i]))
		{
			FVector OtherLoc = OtherAgent->GetActorLocation();

			float Distance = FVector::Dist(Loc, OtherLoc);

			if ((Distance > 0) && (Distance < DesiredAlignment))
			{
				Sum += OtherLoc;
				Count++;
			}
		}
	}

	FVector CohesionForce = FVector::ZeroVector;

	if (Count > 0)
	{
		Sum /= Count;
		CohesionForce = Seek(Sum);

		return CohesionForce;
	}
	else
	{
		return CohesionForce;
	}
}

void USteeringComponent::WrapAroundWorld()
{
	FVector Loc = Agent->GetActorLocation();
	float value = 5000;

	if (Loc.X < -value)
	{
		Agent->SetActorLocation(FVector(value, Loc.Y, Loc.Z));
	}
	else if (Loc.X > value)
	{
		Agent->SetActorLocation(FVector(-value, Loc.Y, Loc.Z));
	}

	if (Loc.Y < -value)
	{
		Agent->SetActorLocation(FVector(Loc.X, value, Loc.Z));
	}
	else if (Loc.Y > value)
	{
		Agent->SetActorLocation(FVector(Loc.X, -value, Loc.Z));
	}

	if (Loc.Z < -value)
	{
		Agent->SetActorLocation(FVector(Loc.X, Loc.Y, value));
	}
	else if (Loc.Z > value)
	{
		Agent->SetActorLocation(FVector(Loc.X, Loc.Y, -value));
	}

}

