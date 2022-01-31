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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FString> LevelNames;

	// Getters
	UFUNCTION(BlueprintCallable, meta=(WorldContext="Context"))
	static ABigFlip * GetFlipper(UObject* Context);

	UFUNCTION(BlueprintPure, meta=(WorldContext="Context"))
	static UBlocksGameInstance* GetMyGameInstance(const UObject* Context);
	
	UFUNCTION(BlueprintCallable, meta=(WorldContext="Context"))
	static bool LoadNextLevel(const UObject* Context);

	UFUNCTION(BlueprintPure,meta=(WorldContext="Context"))
	static class AGameTimer* GetGameTimer(UObject* Context);
	
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
	UPROPERTY()
	ABigFlip * Flipper;

	UPROPERTY()
	AGameTimer* GameTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float GameTimeInSeconds;

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
	bool LoadLevelInternal();

	// Internal Events and Implementations 

//======================================================================================
// C++ Private
private:

	// Internal Variables
	int LevelNum = 0;

	// Overrides

	// Regular Functions

};
