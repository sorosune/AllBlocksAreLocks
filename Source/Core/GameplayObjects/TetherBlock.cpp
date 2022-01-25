// Fill out your copyright notice in the Description page of Project Settings.


#include "TetherBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Core/Actors/BlocksPlayer.h"
#include <Core/GameSystems/BlocksGameInstance.h>

ATetherBlock::ATetherBlock()
{

}

void ATetherBlock::OnBulletHit(ABullet* Bullet)
{
	UWorld* World = GetWorld();
	if (World)
	{
		ABlocksPlayer* Player = Cast<ABlocksPlayer>(UGameplayStatics::GetPlayerCharacter(World, 0));
		PreYeet();
		if (Player && bIsPlayerTouching)
			UBlocksGameInstance::GetFlipper(this)->TeleportPlayer(this);
	}
}