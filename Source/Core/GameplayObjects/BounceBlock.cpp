// Fill out your copyright notice in the Description page of Project Settings.


#include "BounceBlock.h"

ABounceBlock::ABounceBlock()
{

}

void ABounceBlock::OnBulletHit(ABullet* Bullet)
{
	PreYeet();
}

void ABounceBlock::OnPlayerOverlap(ABlocksPlayer* Player, UPrimitiveComponent* OverlappedMesh)
{
	UWorld* World = GetWorld();
	if (World && Player)
	{
		bOnCooldown = true;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("ResetOnCooldown"));
		World->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, BlockCooldown, false);
	}
}