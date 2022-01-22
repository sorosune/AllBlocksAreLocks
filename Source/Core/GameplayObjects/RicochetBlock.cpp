// Fill out your copyright notice in the Description page of Project Settings.


#include "RicochetBlock.h"

ARicochetBlock::ARicochetBlock()
{

}

void ARicochetBlock::OnBulletHit(ABullet* Bullet)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FVector Direction = Bullet->GetActorLocation() - GetActorLocation();
		Direction.Normalize();
		ABullet* NewBullet = World->SpawnActor<ABullet>(Bullet->GetActorLocation() + Direction * 20, Bullet->GetActorRotation());
		if (NewBullet)
		{
			NewBullet->Direction = Bullet->Direction;
			if (FMath::Abs(Direction.X) > FMath::Abs(Direction.Z))
			{
				float Z = NewBullet->Direction.Y;
				if(!Z)
					Z = FMath::Sign(Direction.Z) != 0 ? FMath::Sign(Direction.Z) : 1;
				NewBullet->Direction = FVector2D(-Bullet->Direction.X, Z);
				NewBullet->Direction.Normalize();
			}
			else if ((FMath::Abs(Direction.Z) - FMath::Abs(Direction.X)) < 0.1)
				NewBullet->Direction = FVector2D(-Bullet->Direction.X, -Bullet->Direction.Y);
			else
			{
				float X = NewBullet->Direction.X;
				if(!X)
					X = Direction.X >= 0 ? 1 : -1;
				NewBullet->Direction = FVector2D(X, -Bullet->Direction.Y);
				NewBullet->Direction.Normalize();
			}
		}
	}
}