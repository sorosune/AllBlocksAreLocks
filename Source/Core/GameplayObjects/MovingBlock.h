// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseBlock.h"
#include "MovingBlock.generated.h"

/**
 * 
 */
UCLASS()
class ALLBLOCKSARELOCKS_API AMovingBlock : public ABaseBlock
{
	GENERATED_BODY()
	
	//======================================================================================
	// Blueprint Public
	public:

	// Blueprint Flags

	// Blueprint Variables
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* MovementCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

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
	AMovingBlock();

	// Initializers, and Actor Lifecycle Functions

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	//======================================================================================
	// C++ Protected
	protected:

	// Internal Variables
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FVector StartLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector EndLocation;

	/** X=horizontal movement, Y=vertical movement in grid units */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D NumBlocksToMove = FVector2D(0);

	// Internal Virtual Functions

	// Internal Overrides

	// Internal Regular Functions

	// Internal Events and Implementations 

	//======================================================================================
	// C++ Private
	private:

	// Internal Variables
	FVector Bounds;

	FTimeline MoveTimeline;

	// Overrides

	// Regular Functions
	UFUNCTION()
	void Move(float Value);

};
