// Fill out your copyright notice in the Description page of Project Settings.


#include "FlipFlopRepeatBlock.h"

AFlipFlopRepeatBlock::AFlipFlopRepeatBlock()
{
	FlipFlopTime = 1;
	bFlipFlopRepeat = true;
}

void AFlipFlopRepeatBlock::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	if (World)
	{
		FTimerDelegate TimerDelegate; 
		TimerDelegate.BindUFunction(this, FName("PreYeet"));
		World->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, FlipFlopTime, bFlipFlopRepeat);
	}
}