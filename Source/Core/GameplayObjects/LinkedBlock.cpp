// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkedBlock.h"

ALinkedBlock::ALinkedBlock()
{
	LinkBlocks = true;
}

void ALinkedBlock::BeginPlay()
{
	Super::BeginPlay();
}

void ALinkedBlock::OnBulletHit(ABullet* Bullet)
{
	PreYeet();
}