// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Tree\OctTree.h"
#include "DrawDebugHelpers.h"
#include "Public\Spawner.h"

bool FRect::Contains(FPoint Node)
{
	if ((Node.Location.X >= (CenterLocation.X) - Size.X) && ((Node.Location.X <= (CenterLocation.X) + Size.X)) &&
		(Node.Location.Y >= (CenterLocation.Y) - Size.Y) && ((Node.Location.Y <= (CenterLocation.Y) + Size.Y)) &&
		(Node.Location.Z >= (CenterLocation.Z) - Size.Z) && ((Node.Location.Z <= (CenterLocation.Z) + Size.Z)))
	{
		return true;
	}
	//else if ((Size.Y - CenterLocation.Y / 2) >= Node.Location.Y || (Size.Y + CenterLocation.Y / 2) <= Node.Location.Y)
	//{
	//	return true;
	//}
	//else if ((Size.Z - CenterLocation.Z / 2) >= Node.Location.Z || (Size.Z + CenterLocation.Z / 2) <= Node.Location.Z)
	//{
	//	return true;
	//}


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

	//FBox box;

	//for (int32 i = 0; i < 8; i++)
	//{
	//	UOctTree* Child = CreateDefaultSubobject<UOctTree>("Child");
	//	Child->Capacity = Capacity;
	//	Child->Boundary.Size = FVector(Boundary.Size/ 2);
	//	//Child->Boundary.CenterLocation = FVector()
	//	Children.Add(Child);
	//}

	//NewMapWorld = GetWorld();

	Boundary.Color = FColor::Black;
}

void UOctTree::Display()
{
	if (NewMapWorld)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *GetName())

		DrawDebugBox(NewMapWorld, Boundary.CenterLocation, Boundary.Size, Boundary.Color, true, -1.f, 0, 10);
		//UE_LOG(LogTemp, Warning, TEXT("Boundary Center: %s"), *Boundary.CenterLocation.ToString())


		for (int32 i = 0; i < Nodes.Num(); i++)
		{
			DrawDebugPoint(NewMapWorld, Nodes[i].Location, 5, Boundary.Color, true, -1);
			DrawDebugLine(NewMapWorld, Nodes[i].Location, Boundary.CenterLocation, Boundary.Color, true, -1.f, 0, 3);
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

	FVector NewSize = Boundary.Size;

	//Top North-East
	TNE = CreateChild(NewSize/2);
	FVector TNE_Corner = FVector(Boundary.CenterLocation.X, Boundary.CenterLocation.Y, Boundary.CenterLocation.Z) + FVector(NewSize.X, NewSize.Y, NewSize.Z);
	TNE->Boundary.CenterLocation = (Boundary.CenterLocation + FVector(TNE_Corner.X, TNE_Corner.Y, TNE_Corner.Z))/2;
	TNE->Boundary.Color = FColor::Red;

	//Top South-East
	TSE = CreateChild(NewSize/2);
	TSE->Boundary.CenterLocation = FVector(Boundary.CenterLocation.X, Boundary.CenterLocation.Y, Boundary.CenterLocation.Z) + FVector(-NewSize.X, NewSize.Y, NewSize.Z);
	TSE->Boundary.CenterLocation = (Boundary.CenterLocation + FVector(TSE->Boundary.CenterLocation.X, TSE->Boundary.CenterLocation.Y, TSE->Boundary.CenterLocation.Z))/2;
	TSE->Boundary.Color = FColor::Orange;
	
	//Top South-West
	TSW = CreateChild(NewSize/2);
	TSW->Boundary.CenterLocation = FVector(Boundary.CenterLocation.X, Boundary.CenterLocation.Y, Boundary.CenterLocation.Z) + FVector(-NewSize.X, -NewSize.Y, NewSize.Z);
	TSW->Boundary.CenterLocation = (Boundary.CenterLocation + FVector(TSW->Boundary.CenterLocation.X, TSW->Boundary.CenterLocation.Y, TSW->Boundary.CenterLocation.Z))/2;
	TSW->Boundary.Color = FColor::Yellow;
	
	//Top North-West
	TNW = CreateChild(NewSize/2);
	TNW->Boundary.CenterLocation = FVector(Boundary.CenterLocation.X, Boundary.CenterLocation.Y, Boundary.CenterLocation.Z) + FVector(NewSize.X, -NewSize.Y, NewSize.Z);
	TNW->Boundary.CenterLocation = (Boundary.CenterLocation + FVector(TNW->Boundary.CenterLocation.X, TNW->Boundary.CenterLocation.Y, TNW->Boundary.CenterLocation.Z)) / 2;
	TNW->Boundary.Color = FColor::Green;
	
	//Bottom North-East
	BNE = CreateChild(NewSize/2);
	BNE->Boundary.CenterLocation = FVector(Boundary.CenterLocation.X, Boundary.CenterLocation.Y, Boundary.CenterLocation.Z) + FVector(NewSize.X, NewSize.Y, -NewSize.Z);
	BNE->Boundary.CenterLocation = (Boundary.CenterLocation + FVector(BNE->Boundary.CenterLocation.X, BNE->Boundary.CenterLocation.Y, BNE->Boundary.CenterLocation.Z)) / 2;
	BNE->Boundary.Color = FColor::Blue;
	
	//Bottom South-East
	BSE = CreateChild(NewSize/2);
	BSE->Boundary.CenterLocation = FVector(Boundary.CenterLocation.X, Boundary.CenterLocation.Y, Boundary.CenterLocation.Z) + FVector(-NewSize.X, NewSize.Y, -NewSize.Z);
	BSE->Boundary.CenterLocation = (Boundary.CenterLocation + FVector(BSE->Boundary.CenterLocation.X, BSE->Boundary.CenterLocation.Y, BSE->Boundary.CenterLocation.Z)) / 2;
	BSE->Boundary.Color = FColor::Purple;
	
	//Bottom South-West
	BSW = CreateChild(NewSize/2);
	BSW->Boundary.CenterLocation = FVector(Boundary.CenterLocation.X, Boundary.CenterLocation.Y, Boundary.CenterLocation.Z) + FVector(-NewSize.X, -NewSize.Y, -NewSize.Z);
	BSW->Boundary.CenterLocation = (Boundary.CenterLocation + FVector(BSW->Boundary.CenterLocation.X, BSW->Boundary.CenterLocation.Y, BSW->Boundary.CenterLocation.Z)) / 2;
	BSW->Boundary.Color = FColor::Cyan;

	//Bottom North-West
	BNW = CreateChild(NewSize/2);
	BNW->Boundary.CenterLocation = FVector(Boundary.CenterLocation.X, Boundary.CenterLocation.Y, Boundary.CenterLocation.Z) + FVector(NewSize.X, -NewSize.Y, -NewSize.Z);
	BNW->Boundary.CenterLocation = (Boundary.CenterLocation + FVector(BNW->Boundary.CenterLocation.X, BNW->Boundary.CenterLocation.Y, BNW->Boundary.CenterLocation.Z)) / 2;
	BNW->Boundary.Color = FColor::White;
	
	
	bIsFilled = true;
}

UOctTree* UOctTree::CreateChild(const FVector &NewSize)
{
	if (auto* Child = SpawnerRef->TempTrees.Pop())
	{
		Child->SpawnerRef = SpawnerRef;
		Child->Capacity = Capacity;
		Child->Boundary.Size = NewSize;
		Child->NewMapWorld = NewMapWorld;

		return Child;
	}

	return nullptr;

}
