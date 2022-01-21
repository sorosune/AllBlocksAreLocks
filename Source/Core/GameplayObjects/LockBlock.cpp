// Fill out your copyright notice in the Description page of Project Settings.


#include "LockBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Core/Actors/BlocksPlayer.h"

ALockBlock::ALockBlock()
{

}

void ALockBlock::OnBulletHit(ABullet* Bullet)
{
	PreYeet();
	Bullet->Destroy();
	ABlocksPlayer* Player = Cast<ABlocksPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{
		if (Player->WorldNum == WorldNum)
			Player->DisableWorldSwap();
		else
			Player->EnableWorldSwap();
	}
}