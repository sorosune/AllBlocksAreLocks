// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingBlock.h"
#include "Core/Actors/BlocksPlayer.h"


AMovingBlock::AMovingBlock()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Start = CreateDefaultSubobject<USceneComponent>(TEXT("Start"));
	Target = CreateDefaultSubobject<USceneComponent>(TEXT("Target"));

	Mesh->SetupAttachment(RootComponent);
	Start->SetupAttachment(RootComponent);
	Target->SetupAttachment(RootComponent);
}

void AMovingBlock::BeginPlay()
{
	Super::BeginPlay();	

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
		const FVector Bounds = FVector(50.f);
		DrawDebugBox(World, Start->GetComponentLocation(), Bounds, FColor::Green, true);
		DrawDebugBox(World, Target->GetComponentLocation(), Bounds, FColor::Green, true);
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

	const FVector NewLocation = FMath::Lerp(Start->GetRelativeLocation(), Target->GetRelativeLocation(), Alpha);
	Mesh->SetRelativeLocation(NewLocation);
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
