// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GameplayObjects/BigFlip.h"
#include "Engine/GameInstance.h"
#include "BlocksGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ALLBLOCKSARELOCKS_API UBlocksGameInstance : public UGameInstance
{
	GENERATED_BODY()

//======================================================================================
// Blueprint Public
public:

	// Blueprint Flags

	// Blueprint Variables

	// Getters
	UFUNCTION(BlueprintCallable, meta=(WorldContext="Context"))
	static ABigFlip * GetFlipper(UObject * Context);
	
	
	// Setters

	// Wrappers

	// External Virtual Functions

	// External Overrides

	// External Regular Functions

	// External Events

//======================================================================================
// C++ Public
public:

	// Flags

	// Initialized Variables
	ABigFlip * Flipper;

	// Constructor
	UBlocksGameInstance();

	// Initializers, and Actor Lifecycle Functions

	//======================================================================================
	// C++ Protected
protected:

	// Internal Variables

	// Internal Virtual Functions

	// Internal Overrides

	// Internal Regular Functions

	// Internal Events and Implementations 

//======================================================================================
// C++ Private
private:

	// Internal Variables

	// Overrides

	// Regular Functions

};
