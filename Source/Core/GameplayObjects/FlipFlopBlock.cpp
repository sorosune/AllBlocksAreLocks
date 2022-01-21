// Fill out your copyright notice in the Description page of Project Settings.


#include "FlipFlopBlock.h"

AFlipFlopBlock::AFlipFlopBlock()
{
	FlipFlopTime = 1;
}

void AFlipFlopBlock::OnBulletHit(ABullet* Bullet)
{
	UWorld* World = GetWorld();
	if (World)
	{
		if (!World->GetTimerManager().IsTimerActive(TimerHandle))
		{
			PreYeet();
			FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AFlipFlopBlock::PreYeet);
			World->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, FlipFlopTime, false);
		}
		else
		{
			PreYeet();
			World->GetTimerManager().ClearTimer(TimerHandle);
		}
	}
}