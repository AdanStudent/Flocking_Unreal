// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OctTree.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FPoint
{
	GENERATED_USTRUCT_BODY()

public:
	//keeping the location of the Point in 3d space
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Point")
	FVector Location;

	//keeping a reference of the object that it is representing
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Point")
	UObject* Data;
};

USTRUCT(BlueprintType)
struct FRect
{
	GENERATED_USTRUCT_BODY()

public:
	//used to keep the location of where the center of the rect is being drawn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rect")
	FVector CenterLocation;

	//how large in 3 dimensions is the rect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rect")
	FVector Size;

	//used to check if a point is within the bounds of this Rect
	bool Contains(FPoint Node);
	
	//Used to check if another Rect intersects with this one
	bool Intersects(FRect Range);

};

UCLASS()
class FLOCKING_API UOctTree : public UObject
{
	GENERATED_BODY()

public:
	//a list of all the nodes that were created and added to this part of the tree
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tree")
	TArray<FPoint*> Nodes;

	//Keeps a list of all the children that are created once this upper part of the tree is filledd
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tree")
	TArray<UOctTree*> Children;

	//tracks what is the limit of how many point can be in each child
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tree")
	int32 Capacity;

	//once it is filled this is marked as true to possibly divive or find another avaiable tree section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tree")
	bool bIsFilled;

	//used for the initial size of the parent tree
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tree")
	FRect* Boundary;

	//used to display the parent rect and all its children, including its points
	void Display();
	
	//is used to fill up the parent node and if it becomes filled will then call the Divide function
	void Insert(FPoint ToBeAdded);

	//Used to find all nodes that interset with the Range, and returns a collection of all Found nodes
	void Query(FRect* Range, TArray<FPoint*> &Found);

private:
	//used when an insert is needed and the current parent is filled
	void Divide();





	
};