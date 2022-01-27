// Fill out your copyright notice in the Description page of Project Settings.


#include "MysteryBlock.h"

AMysteryBlock::AMysteryBlock()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMysteryBlock::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	TargetLocation = TargetLocation + StartLocation;	// transform to world
	HomeWorld = WorldNum;
	
#if WITH_ENGINE
	// just to be able to see target location
	UWorld* World = GetWorld();
	if (World)
	{
		DrawDebugBox(World, StartLocation, FVector(50.f), FColor::Magenta, true);
		DrawDebugBox(World, TargetLocation, FVector(50.f), FColor::Magenta, true);
	}
#endif
}

bool AMysteryBlock::PreYeet()
{
	if (!bIsYeetable)
		return false;
	
	if (WorldNum == HomeWorld)
		NextLocation = TargetLocation;
	else
		NextLocation = StartLocation;

	FHitResult Hit;
	if (!CheckGridSpace(NextLocation, Hit))
	{
		PostYeet();
		return true;
	}
	return false;
}

void AMysteryBlock::PostYeet()
{
	WorldNum = !WorldNum;
	SetActorLocation(NextLocation);
	dOnYeeted.Broadcast(WorldNum);
}

void AMysteryBlock::OnBulletHit(ABullet* Bullet)
{
	Super::OnBulletHit(Bullet);
}
