// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathBlock.h"

#include "Core/Actors/BlocksPlayer.h"

ADeathBlock::ADeathBlock()
{
	bIsYeetable = false;
}

void ADeathBlock::OnPlayerOverlap(ABlocksPlayer* Player, UPrimitiveComponent* OverlappedMesh)
{
	if (Player)
		Player->Die();
}