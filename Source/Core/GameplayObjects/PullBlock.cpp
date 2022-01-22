// Fill out your copyright notice in the Description page of Project Settings.


#include "PullBlock.h"

APullBlock::APullBlock()
{

}

void APullBlock::OnBulletHit(ABullet* Bullet)
{
	FVector HitLocation = Bullet->GetActorLocation();
	FVector Direction = HitLocation - GetActorLocation();
	Direction.Normalize();
	Direction.Y = 0;
	MoveBlock(Direction, Bullet);
}