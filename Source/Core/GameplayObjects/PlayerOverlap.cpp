// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerOverlap.h"

// Sets default values
APlayerOverlap::APlayerOverlap()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionProfileName("OverlapPlayer");
}
