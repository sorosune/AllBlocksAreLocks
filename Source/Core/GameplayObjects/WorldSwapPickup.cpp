// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldSwapPickup.h"
#include <Core/Actors/BlocksPlayer.h>

#include "Core/GameSystems/BlocksGameInstance.h"

// Sets default values
AWorldSwapPickup::AWorldSwapPickup()
{

}

// Called when the game starts or when spawned
void AWorldSwapPickup::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWorldSwapPickup::OnOverlapBegin);
	ABigFlip * flipper = UBlocksGameInstance::GetFlipper(this);
	if(flipper)
	{
		FAttachmentTransformRules rules(EAttachmentRule::KeepWorld, true);
		this->AttachToActor(flipper, rules);
	}
}

void AWorldSwapPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABlocksPlayer* Player = Cast<ABlocksPlayer>(OtherActor);
	if (Player)
	{
		Player->NumWorldSwaps += WorldSwapsToAdd;
		Destroy();
	}
}

