// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YeetBlock.h"
#include "MysteryBlock.generated.h"


UCLASS()
class ALLBLOCKSARELOCKS_API AMysteryBlock : public AYeetBlock
{
	GENERATED_BODY()

//======================================================================================
// Blueprint Public
public:
	// Blueprint Flags

	// Blueprint Variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(MakeEditWidget))
	FVector TargetLocation;

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
	AMysteryBlock();

	// Initializers, and Actor Lifecycle Functions

	virtual void BeginPlay() override;
	
	virtual bool PreYeet() override;
	
	virtual void PostYeet() override;

//======================================================================================
// C++ Protected
protected:
	// Internal Variables

	// Internal Virtual Functions

	// Internal Overrides

	virtual void OnBulletHit(ABullet* Bullet) override;

	// Internal Regular Functions

	// Internal Events and Implementations 

//======================================================================================
// C++ Private
private:
	// Internal Variables
	
	FVector StartLocation;
	FVector NextLocation;
	int HomeWorld;

	// Overrides

	// Regular Functions
};
