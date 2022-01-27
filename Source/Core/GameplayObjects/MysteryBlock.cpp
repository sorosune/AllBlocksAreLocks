// Fill out your copyright notice in the Description page of Project Settings.


#include "MysteryBlock.h"

AMysteryBlock::AMysteryBlock()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Start = CreateDefaultSubobject<USceneComponent>(TEXT("Start"));
	Target = CreateDefaultSubobject<USceneComponent>(TEXT("Target"));

	Mesh->SetupAttachment(RootComponent);
	Start->SetupAttachment(RootComponent);
	Target->SetupAttachment(RootComponent);
}

void AMysteryBlock::BeginPlay()
{
	Super::BeginPlay();
	
#if WITH_ENGINE
	// just to be able to see target location
	UWorld* World = GetWorld();
	if (World)
	{
		const FVector Bounds = FVector(50.f);
		DrawDebugBox(World, Start->GetComponentLocation(), Bounds, FColor::Magenta, true);
		DrawDebugBox(World, Target->GetComponentLocation(), Bounds, FColor::Magenta, true);
	}
#endif
}

bool AMysteryBlock::PreYeet()
{
	if (!bIsYeetable)
		return false;
	
	if (bIsAtStart)
		NextLocation = Target->GetComponentLocation();
	else
		NextLocation = Start->GetComponentLocation();

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
	bIsAtStart = !bIsAtStart;
	Mesh->SetWorldLocation(NextLocation);
}

void AMysteryBlock::OnBulletHit(ABullet* Bullet)
{
	Super::OnBulletHit(Bullet);
}
