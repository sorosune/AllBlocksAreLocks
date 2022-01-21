// Fill out your copyright notice in the Description page of Project Settings.


#include "YeetBlock.h"

AYeetBlock::AYeetBlock()
{

}

void AYeetBlock::OnBulletHit(ABullet* Bullet)
{
	PreYeet();
	Bullet->Destroy();
}
