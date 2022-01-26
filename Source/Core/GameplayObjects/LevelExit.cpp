// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelExit.h"
#include <Core/Actors/BlocksPlayer.h>
#include <Core/HelperFiles/DefinedDebugHelpers.h>

// Sets default values
ALevelExit::ALevelExit()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionProfileName("LevelExit");
}

// Called when the game starts or when spawned
void ALevelExit::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALevelExit::OnOverlapBegin);
}

void ALevelExit::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ABlocksPlayer>(OtherActor))
		SCREENMSG("Load Next Level");
}

