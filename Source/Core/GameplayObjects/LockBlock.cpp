// Fill out your copyright notice in the Description page of Project Settings.


#include "LockBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Core/GameSystems/BlocksGameInstance.h"
#include "Core/Actors/BlocksPlayer.h"

ALockBlock::ALockBlock()
{

}

void ALockBlock::BeginPlay()
{
	Super::BeginPlay();
	LockPlayerWorldSwap();
	UBlocksGameInstance::GetFlipper(this)->dOnFlip.AddDynamic(this, &ALockBlock::LockPlayerWorldSwap);
}

void ALockBlock::OnBulletHit(ABullet* Bullet)
{
	PreYeet();
	Bullet->Destroy();
	LockPlayerWorldSwap();
}

void ALockBlock::LockPlayerWorldSwap()
{
	ABlocksPlayer* Player = Cast<ABlocksPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{
		if (Player->WorldNum == WorldNum)
			Player->DisableWorldSwap();
		else
			Player->EnableWorldSwap();
	}
}