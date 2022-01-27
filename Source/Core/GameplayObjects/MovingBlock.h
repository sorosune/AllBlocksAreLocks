// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseBlock.h"
#include "MovingBlock.generated.h"

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
	float MoveSpeed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(MakeEditWidget))
	USceneComponent* Start;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(MakeEditWidget))
	USceneComponent* Target;

	// Getters

	// Setters
	
	UFUNCTION(BlueprintCallable)
	void SetTimeline(UPARAM(ref) UTimelineComponent* TimelineComponent);

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

	//======================================================================================
	// C++ Protected
	protected:

	// Internal Variables
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TEnumAsByte<ETimelineDirection::Type> MovementDirection;

	// Internal Virtual Functions

	// Internal Overrides

	// Internal Regular Functions

	// Internal Events and Implementations
	
	UFUNCTION(BlueprintCallable)
	void Move(ETimelineDirection::Type Direction, float Alpha);

	UFUNCTION(BlueprintImplementableEvent)
	void ReverseMovement(ETimelineDirection::Type CurrentDirection);

	//======================================================================================
	// C++ Private
	private:

	// Internal Variables
	
	UPROPERTY()
	UTimelineComponent* Timeline;

	// Overrides

	// Regular Functions
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
