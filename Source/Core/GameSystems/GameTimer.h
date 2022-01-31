// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameTimer.generated.h"

UCLASS()
class ALLBLOCKSARELOCKS_API AGameTimer : public AActor
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
	void SaveTimer();

	UFUNCTION(BlueprintCallable)
	FText GetTimerText();

	// External Events

//======================================================================================
// C++ Public
public:

	// Flags

	// Initialized Variables

	// Constructor
	AGameTimer();

	// Initializers, and Actor Lifecycle Functions

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaTime) override;

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

	float GameTimeInSeconds;

	// Overrides

	// Regular Functions

	// FString PadWithZeros(FString Input, int DesiredNumCharacters);
	
};
