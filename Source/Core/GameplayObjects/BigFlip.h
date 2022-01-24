// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/GameplayObjects/BaseBlock.h"
#include "BigFlip.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ALLBLOCKSARELOCKS_API ABigFlip : public AActor
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
	UFUNCTION(BlueprintCallable)
	bool Flip();
	
	UFUNCTION(BlueprintCallable)
	bool TeleportPlayer(ABaseBlock * block);

	// External Events

	//======================================================================================
	// C++ Public
public:

	// Flags

	// Initialized Variables

	// Constructor
	/** Sets default values for this actor's properties*/
	ABigFlip();

	// Initializers, and Actor Lifecycle Functions
	/** Called when the game starts or when spawned*/
	virtual void BeginPlay() override;
	
	/** Called when the game ends or when destroyed*/
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

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
