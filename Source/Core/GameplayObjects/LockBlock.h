// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseBlock.h"
#include "LockBlock.generated.h"

/**
 * 
 */
UCLASS()
class ALLBLOCKSARELOCKS_API ALockBlock : public ABaseBlock
{
	GENERATED_BODY()

//======================================================================================
// Blueprint Public
public:

	// Blueprint Flags

	// Blueprint Variables

	// Getters

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

	// Constructor
	ALockBlock();

	// Initializers, and Actor Lifecycle Functions

	virtual void OnBulletHit(ABullet* Bullet) override;

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
