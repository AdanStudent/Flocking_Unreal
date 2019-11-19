// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Tree\OctTree.h"
#include "DrawDebugHelpers.h"
#include "Public\Spawner.h"

bool FRect::Contains(FPoint Node)
{
	if ((Size.X - CenterLocation.X/2) >= Node.Location.X || (Size.X + CenterLocation.X / 2) <= Node.Location.X)
	{
		return true;
	}
	else if ((Size.Y - CenterLocation.Y / 2) >= Node.Location.Y || (Size.Y + CenterLocation.Y / 2) <= Node.Location.Y)
	{
		return true;
	}
	else if ((Size.Z - CenterLocation.Z / 2) >= Node.Location.Z || (Size.Z + CenterLocation.Z / 2) <= Node.Location.Z)
	{
		return true;
	}


	return false;
}

bool FRect::Intersects(FRect Range)
{
	return false;
}

UOctTree::UOctTree()
{
	Capacity = 4;
	bIsFilled = false;
	
	Boundary.CenterLocation = FVector::ZeroVector;

	//for (int32 i = 0; i < 8; i++)
	//{
	//	UOctTree* Child = CreateDefaultSubobject<UOctTree>("Child");
	//	Child->Capacity = Capacity;
	//	Child->Boundary.Size = FVector(Boundary.Size/ 2);
	//	//Child->Boundary.CenterLocation = FVector()
	//	Children.Add(Child);
	//}

	//NewMapWorld = GetWorld();
}

void UOctTree::Display()
{
	if (NewMapWorld)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *GetName())

		DrawDebugBox(NewMapWorld, Boundary.CenterLocation, Boundary.Size, FColor::Red, true, -1.f, 0, 5);

		for (int32 i = 0; i < Nodes.Num(); i++)
		{
			DrawDebugPoint(NewMapWorld, Nodes[i].Location, 5, FColor::Green, true, -1);
		}

		if (bIsFilled)
		{
			TNE->Display();
			TSE->Display();
			TSW->Display();
			TNW->Display();
			BNE->Display();
			BSE->Display();
			BSW->Display();
			BNW->Display();
		}


	}
}

bool UOctTree::Insert(FPoint ToBeAdded)
{
	//checking if the current parent has ToBeAdded
	if (!Boundary.Contains(ToBeAdded))
	{
		return false;
	}

	//if the capacity is currently not at max then add it
	if (Nodes.Num() < Capacity)
	{
		Nodes.Add(ToBeAdded);
		return true;
	}
	else //otherwise find a box that it can go into
	{
		//if this is not filled then divide
		if (!bIsFilled)
		{
			Divide();
		}

		//check each subdivision to see if it can be inserted
		//UE_LOG(LogTemp, Warning, TEXT(""))
		if (TNE->Insert(ToBeAdded) || TSE->Insert(ToBeAdded) || TSW->Insert(ToBeAdded) || TNW->Insert(ToBeAdded) || 
			BNE->Insert(ToBeAdded) || BSE->Insert(ToBeAdded) || BSW->Insert(ToBeAdded) || BNW->Insert(ToBeAdded))
		{
			//once one is found return true;
			return true;
		}
		
	}

	return false;

}

void UOctTree::Query(FRect Range, TArray<FPoint> &Found)
{
}

int UOctTree::HowManyChildren()
{

	if (!bIsFilled)
	{
		return 1;
	}

	int count = 1;

	count += TNE->HowManyChildren();
	count += TSE->HowManyChildren();
	count += TSW->HowManyChildren();
	count += TNW->HowManyChildren();
	count += BNE->HowManyChildren();
	count += BSE->HowManyChildren();
	count += BSW->HowManyChildren();
	count += BNW->HowManyChildren();

	return count;
}

void UOctTree::Divide()
{

	if (!SpawnerRef || SpawnerRef->TempTrees.Num() < 7)
	{
		return;
	}

	FVector NewSize = Boundary.Size / 2;

	//Top North-East
	TNE = CreateChild(NewSize);
	TNE->Boundary.CenterLocation = NewSize;
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *TNE->Boundary.CenterLocation.ToString())

	//Children.Add(TNE);

	//Top South-East
	TSE = CreateChild(NewSize);
	TSE->Boundary.CenterLocation = FVector(-NewSize.X, NewSize.Y, NewSize.Z);
	//Children.Add(TSE);

	//Top South-West
	TSW = CreateChild(NewSize);
	TSW->Boundary.CenterLocation = FVector(-NewSize.X, -NewSize.Y, NewSize.Z);
	//Children.Add(TSW);

	//Top North-West
	TNW = CreateChild(NewSize);
	TNW->Boundary.CenterLocation = FVector(NewSize.X, -NewSize.Y, NewSize.Z);
	//Children.Add(TNW);

	//Bottom North-East
	BNE = CreateChild(NewSize);
	BNE->Boundary.CenterLocation = FVector(NewSize.X, NewSize.Y, -NewSize.Z);
	//Children.Add(BNE);

	//Bottom South-East
	BSE = CreateChild(NewSize);
	BSE->Boundary.CenterLocation = FVector(-NewSize.X, NewSize.Y, -NewSize.Z);
	//Children.Add(BSE);

	//Bottom South-West
	BSW = CreateChild(NewSize);
	BSW->Boundary.CenterLocation = FVector(-NewSize);
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *BSW->Boundary.CenterLocation.ToString())
	//Children.Add(BSW);

	//Bottom North-West
	BNW = CreateChild(NewSize);
	BNW->Boundary.CenterLocation = FVector(NewSize.X, -NewSize.Y, -NewSize.Z);

	//Children.Add(BNW);
	
	bIsFilled = true;
}

UOctTree* UOctTree::CreateChild(const FVector &NewSize)
{
	auto* Child = SpawnerRef->TempTrees.Pop();
	Child->SpawnerRef = SpawnerRef;
	Child->Capacity = Capacity;
	Child->Boundary.Size = NewSize;
	Child->NewMapWorld = NewMapWorld;

	if (!Child)
		return nullptr;
	
	return Child;
}
