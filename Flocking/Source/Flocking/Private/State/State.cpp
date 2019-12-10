// Fill out your copyright notice in the Description page of Project Settings.


#include "Public\State\State.h"

void UFlocking::Enter(AActor* Owner)
{
	UE_LOG(LogTemp, Warning, TEXT("ENTER Flocking"));
	bIsActive = true;
}

void UFlocking::Execute(AActor* Owner)
{
	UE_LOG(LogTemp, Warning, TEXT("Execute Flocking"));

}

void UFlocking::Exit(AActor* Owner)
{
	UE_LOG(LogTemp, Warning, TEXT("Exit Flocking"));
	bIsActive = false;
}
