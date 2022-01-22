// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingBlock.h"

AMovingBlock::AMovingBlock()
{
}

void AMovingBlock::BeginPlay()
{
	Super::BeginPlay();

	if (MovementCurve)
	{
		// Like a delegate. Contains the signature of the function that runs on timeline tick
		FOnTimelineFloat TimelineCallback;
		// FOnTimelineEventStatic TimelineFinishedCallback;

		TimelineCallback.BindUFunction(this, FName("Move"));
		MoveTimeline.AddInterpFloat(MovementCurve, TimelineCallback);
		MoveTimeline.SetLooping(true);
	}

	// Box Extent is technically half-bounds
	Bounds = Mesh->CalcBounds(FTransform::Identity).BoxExtent * 2;

	// NumBlocksToMove = (horizontal offset, vertical offset)
	StartLocation = EndLocation = GetActorLocation();
	EndLocation.X += NumBlocksToMove.X * Bounds.X;
	EndLocation.Z += NumBlocksToMove.Y * Bounds.Z;

	MoveTimeline.PlayFromStart();
	
#if WITH_ENGINE
	// just to be able to see target location
	UWorld* World = GetWorld();
	if (World)
	{
		DrawDebugBox(World, EndLocation, Bounds/2.f, FColor::Green, true);
	}
#endif
}

void AMovingBlock::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	MoveTimeline.TickTimeline(DeltaSeconds * MoveSpeed);
}

void AMovingBlock::Move(float Value)
{
	FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Value);
	SetActorLocation(NewLocation);
}
