// Fill out your copyright notice in the Description page of Project Settings.


#include "PushBlock.h"

APushBlock::APushBlock()
{

}

void APushBlock::OnBulletHit(ABullet* Bullet)
{
	FVector HitLocation = Bullet->GetActorLocation();
	FVector Direction = -(HitLocation - GetActorLocation());
	Direction.Normalize();
	Direction.Y = 0;

	if (FMath::Abs(Direction.X) > FMath::Abs(Direction.Z))
	{
		Direction.X /= FMath::Abs(Direction.X);
		Direction.Z = 0;
	}
	else if ((FMath::Abs(Direction.Z) - FMath::Abs(Direction.X)) < 0.027)
	{
		if (Bullet->Direction.X > Bullet->Direction.Y)
		{
			Direction.X /= FMath::Abs(Direction.X);
			Direction.Z = 0;
		}
		else
		{
			Direction.X = 0;
			Direction.Z /= FMath::Abs(Direction.Z);
		}
	}
	else
	{
		Direction.X = 0;
		Direction.Z /= FMath::Abs(Direction.Z);
	}
	UWorld* World = GetWorld();
	if (World)
	{
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		FHitResult Hit;
		FVector StartPosition = GetActorLocation();
		FVector EndPosition = StartPosition + 100 * Direction;
		FVector Forward;
		if (World->SweepSingleByChannel(Hit,
			StartPosition,
			EndPosition,
			GetActorRotation().Quaternion(),
			ECollisionChannel::ECC_Camera,
			FCollisionShape::MakeBox(FVector(45, 45, 45)),
			Params))
		{
			return;
		}
		else
		{
			SetActorLocation(EndPosition);
		}
	}
}
