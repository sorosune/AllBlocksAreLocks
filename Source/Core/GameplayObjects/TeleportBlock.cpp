// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportBlock.h"
#include "Core/GameSystems/BlocksGameInstance.h"
#include "Core/Actors/BlocksPlayer.h"

ATeleportBlock::ATeleportBlock()
{

}

void ATeleportBlock::OnBulletHit(ABullet* Bullet)
{
	if (PairedBlock)
	{
		FVector Direction = -(Bullet->GetActorLocation() - GetActorLocation());
		Direction.Normalize();
		PairedBlock->SpawnBullet(Direction);
	}
}

void ATeleportBlock::OnPlayerOverlap(ABlocksPlayer* Player, UPrimitiveComponent* OverlappedMesh)
{
	if (PairedBlock)
	{
		Player->GetMovementComponent()->Velocity = FVector(0, 0, 0);
		UBlocksGameInstance::GetFlipper(this)->TeleportPlayer(PairedBlock);
	}
}

