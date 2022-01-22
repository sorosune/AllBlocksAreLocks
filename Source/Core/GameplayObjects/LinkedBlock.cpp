// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkedBlock.h"

ALinkedBlock::ALinkedBlock()
{

}

void ALinkedBlock::BeginPlay()
{
	Super::BeginPlay();
	GetComponents(LinkedBlocks);
}

void ALinkedBlock::OnBulletHit(ABullet* Bullet)
{
	PreYeet();
}