// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IState.h"
#include "State.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class FLOCKING_API UState : public UObject, public IIState
{
	GENERATED_BODY()

public:
	UState();

	bool bIsActive;

	virtual void Enter(AActor* Owner) override PURE_VIRTUAL(UState::Enter, ;);
	virtual void Execute(AActor* Owner) override PURE_VIRTUAL(UState::Execute, ;);
	virtual void Exit(AActor* Owner) override PURE_VIRTUAL(UState::Exit, ;);


};

UCLASS()
class FLOCKING_API UFlocking : public UState
{
	GENERATED_BODY()

public:
	UFlocking();


	virtual void Enter(AActor* Owner) override;
	virtual void Execute(AActor* Owner) override;
	virtual void Exit(AActor* Owner) override;


};


