// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Tree\OctTree.h"
#include "DrawDebugHelpers.h"
#include "Public\Spawner.h"

bool FRect::Contains(FPoint Node)
{
	float Distance = FVector::DistSquared(CenterLocation, Node.Location);
	if (Distance < Size.X * Size.X)
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
}

void UOctTree::Display()
{
	if (UWorld* World = GetWorld())
	{

		if (bIsFilled)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *GetName())
			DrawDebugBox(GetWorld(), Boundary.CenterLocation, Boundary.Size, FColor::Red, true, -1.f, 0, 5);

			for (int32 i = 0; i < Children.Num(); i++)
			{
				Children[i]->Display();
			}
		}


		for (int32 i = 0; i < Nodes.Num(); i++)
		{
			DrawDebugPoint(World, Nodes[i].Location, 5, FColor::Green, true, -1);
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
		if (Children[0]->Insert(ToBeAdded) || Children[1]->Insert(ToBeAdded) || Children[2]->Insert(ToBeAdded) || Children[3]->Insert(ToBeAdded) || 
			Children[4]->Insert(ToBeAdded) || Children[5]->Insert(ToBeAdded) || Children[6]->Insert(ToBeAdded) || Children[7]->Insert(ToBeAdded))
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

	if (Children.Num() < 1)
	{
		return 1;
	}

	int count = 1;
	for (int i = 0; i < Children.Num(); i++)
	{
		count += Children[i]->HowManyChildren();
	}

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
	auto* TNE = CreateChild(NewSize);
	TNE->Boundary.CenterLocation = NewSize;
	Children.Add(TNE);

	//Top South-East
	auto* TSE = CreateChild(NewSize);
	TSE->Boundary.CenterLocation = FVector(-NewSize.X, NewSize.Y, NewSize.Z);
	Children.Add(TSE);

	//Top South-West
	auto* TSW = CreateChild(NewSize);
	TSW->Boundary.CenterLocation = FVector(-NewSize.X, -NewSize.Y, NewSize.Z);
	Children.Add(TSW);

	//Top North-West
	auto* TNW = CreateChild(NewSize);
	TNW->Boundary.CenterLocation = FVector(NewSize.X, -NewSize.Y, NewSize.Z);
	Children.Add(TNW);

	//Bottom North-East
	auto* BNE = CreateChild(NewSize);
	BNE->Boundary.CenterLocation = FVector(NewSize.X, NewSize.Y, -NewSize.Z);
	Children.Add(BNE);

	//Bottom South-East
	auto* BSE = CreateChild(NewSize);
	BSE->Boundary.CenterLocation = FVector(-NewSize.X, NewSize.Y, -NewSize.Z);
	Children.Add(BSE);

	//Bottom South-West
	auto* BSW = CreateChild(NewSize);
	BSW->Boundary.CenterLocation = FVector(-NewSize);
	Children.Add(BSW);

	//Bottom North-West
	auto* BNW = CreateChild(NewSize);
	BNW->Boundary.CenterLocation = FVector(NewSize.X, -NewSize.Y, -NewSize.Z);
	Children.Add(BNW);
	
	bIsFilled = true;
}

UOctTree* UOctTree::CreateChild(const FVector &NewSize)
{
	auto* Child = SpawnerRef->TempTrees.Pop();
	Child->SpawnerRef = SpawnerRef;
	Child->Capacity = Capacity;
	Child->Boundary.Size = NewSize;

	if (!Child)
		return nullptr;
	
	return Child;
}
