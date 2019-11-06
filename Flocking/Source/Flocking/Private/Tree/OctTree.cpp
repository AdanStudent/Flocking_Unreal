// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Tree\OctTree.h"
#include "DrawDebugHelpers.h"


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

	float size = 2000;
	Boundary.Size = FVector(size);
}

void UOctTree::Display()
{
	if (UWorld* World = GetWorld())
	{
		DrawDebugBox(World, Boundary.CenterLocation, Boundary.Size, FColor::Red);

		if (bIsFilled)
		{
			for (int32 i = 0; i < Children.Num(); i++)
			{
				Children[i]->Display();
			}
		}


		for (int32 i = 0; i < Nodes.Num(); i++)
		{
			DrawDebugPoint(World, Nodes[i].Location, 5, FColor::Green);
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
		for (int32 i = 0; i < Children.Num(); i++)
		{
			if (Children[i]->Insert(ToBeAdded))
			{
				//once one is found return true;
				return true;
			}
		}
	}

	return false;

}

void UOctTree::Query(FRect Range, TArray<FPoint> &Found)
{
}

void UOctTree::Divide()
{
}
