// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelKey.h"
#include <Core/Actors/BlocksPlayer.h>
#include <Core/HelperFiles/DefinedDebugHelpers.h>

// Sets default values
ALevelKey::ALevelKey()
{

}

// Called when the game starts or when spawned
void ALevelKey::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALevelKey::OnOverlapBegin);
}

void ALevelKey::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABlocksPlayer* Player = Cast<ABlocksPlayer>(OtherActor);
	if (Player)
		Player->bHasKey = true;
}

