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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Point")
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Point")
	UObject* Data;
};

USTRUCT(BlueprintType)
struct FRect
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rect")
	FVector CenterLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rect")
	FVector Size;

	bool Contains(FPoint Node);
	bool Intersects(FRect Range);

};

UCLASS()
class FLOCKING_API UOctTree : public UObject
{
	GENERATED_BODY()
	
};
