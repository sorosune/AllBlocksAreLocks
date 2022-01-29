// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GameplayObjects/BigFlip.h"
#include "Engine/GameInstance.h"
#include "BlocksGameInstance.generated.h"

UCLASS()
class ALLBLOCKSARELOCKS_API UBlocksGameInstance : public UGameInstance
{
	GENERATED_BODY()

//======================================================================================
// Blueprint Public
public:

	// Blueprint Flags

	// Blueprint Variables
	
	UPROPERTY(BlueprintReadOnly)
	float GameTimeSeconds;

	// Getters
	UFUNCTION(BlueprintCallable, meta=(WorldContext="Context"))
	static ABigFlip * GetFlipper(UObject* Context);

	UFUNCTION(BlueprintPure, meta=(WorldContext="Context"))
	static UBlocksGameInstance* GetMyGameInstance(const UObject* Context);

	// Setters

	UFUNCTION(BlueprintCallable, meta=(WorldContext="Context"))
	void SaveGameTimeInSeconds(const UObject* Context, float Value);
	
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
	UPROPERTY()
	ABigFlip * Flipper;

	// Constructor
	UBlocksGameInstance();

	// Initializers, and Actor Lifecycle Functions

	virtual void Init() override;

	virtual void Shutdown() override;

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
