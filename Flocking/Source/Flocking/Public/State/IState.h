// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IState.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIState : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FLOCKING_API IIState
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void Enter(AActor* Owner) PURE_VIRTUAL(IIState::Enter, ;);
	virtual void Execute(AActor* Owner) PURE_VIRTUAL(IIState::Execute, ;);
	virtual void Exit(AActor* Owner) PURE_VIRTUAL(IIState::Exit, ;);

};

