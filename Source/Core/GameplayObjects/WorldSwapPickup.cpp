// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldSwapPickup.h"
#include <Core/Actors/BlocksPlayer.h>

// Sets default values
AWorldSwapPickup::AWorldSwapPickup()
{

}

// Called when the game starts or when spawned
void AWorldSwapPickup::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWorldSwapPickup::OnOverlapBegin);
}

void AWorldSwapPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABlocksPlayer* Player = Cast<ABlocksPlayer>(OtherActor);
	if (Player)
		Player->NumWorldSwaps += WorldSwapsToAdd;
}

