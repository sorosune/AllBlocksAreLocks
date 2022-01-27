// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingBlock.h"
#include "Core/Actors/BlocksPlayer.h"


AMovingBlock::AMovingBlock()
{
}

void AMovingBlock::BeginPlay()
{
	Super::BeginPlay();	
	// Box Extent is technically half-bounds
	Bounds = Mesh->CalcBounds(FTransform::Identity).BoxExtent * 2;

	// NumBlocksToMove = (horizontal offset, vertical offset)
	StartLocation = TargetLocation = GetActorLocation();
	TargetLocation.X += NumBlocksToMove.X * Bounds.X;
	TargetLocation.Z += NumBlocksToMove.Y * Bounds.Z;
	
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AMovingBlock::OnOverlapBegin);

	if (Timeline)
	{
		Timeline->SetPlayRate(MoveSpeed);
		Timeline->SetLooping(true);
		Timeline->PlayFromStart();
	}
	
#if WITH_ENGINE
	// just to be able to see target location
	UWorld* World = GetWorld();
	if (World)
	{
		DrawDebugBox(World, StartLocation, Bounds/2.f, FColor::Green, true);
		DrawDebugBox(World, TargetLocation, Bounds/2.f, FColor::Green, true);
	}
#endif
}

void AMovingBlock::SetTimeline(UTimelineComponent* TimelineComponent)
{
	Timeline = TimelineComponent;
}

void AMovingBlock::Move(ETimelineDirection::Type Direction, float Alpha)
{
	MovementDirection = Direction;
	const FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);
	SetActorLocation(NewLocation);
}

void AMovingBlock::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseBlock* Block = Cast<ABaseBlock>(OtherActor);
	ABlocksPlayer* Player = Cast<ABlocksPlayer>(OtherActor);

	if (Block || Player)
	{
		if (MovementDirection == ETimelineDirection::Type::Forward)
			MovementDirection = ETimelineDirection::Type::Backward;
		else
			MovementDirection = ETimelineDirection::Type::Forward;
		ReverseMovement(MovementDirection);
	}
}
