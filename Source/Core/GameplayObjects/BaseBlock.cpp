// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBlock.h"
#include "Core/Actors/BlocksPlayer.h"
#include <Core/GameSystems/BlocksGameInstance.h>

// Sets default values
ABaseBlock::ABaseBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->CastShadow = 0;
}

// Called when the game starts or when spawned
void ABaseBlock::BeginPlay()
{
	Super::BeginPlay();
	Mesh->OnComponentHit.AddDynamic(this, &ABaseBlock::OnHit);
	WorldNum = GetActorLocation().Z < 0;
}

void ABaseBlock::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ABlocksPlayer* Player = Cast<ABlocksPlayer>(OtherActor);
	if (Player)
	{
		OnPlayerOverlap(Player, OtherComp);
		Player->OnBlockTouch(this, HitComp);
	}
}

bool ABaseBlock::PreYeet()
{
	if (!bIsYeetable)
		return false;
	TSet<UStaticMeshComponent*> OurBlocks;
	OurBlocks.Append(LinkedBlocks);
	OurBlocks.Add(Mesh);
	UWorld* World = GetWorld();
	if (World)
	{
		TSet<ABaseBlock*> BlocksToYeet;
		for (auto& Block : OurBlocks)
		{
			FHitResult Hit;
			FVector Position = FVector(Block->GetComponentLocation().X, Block->GetComponentLocation().Y, -Block->GetComponentLocation().Z);
			FVector Forward;
			if (World->SweepSingleByChannel(Hit,
				Position,
				Position,
				FQuat::Identity,
				ECollisionChannel::ECC_Camera,
				FCollisionShape::MakeBox(FVector(49.9, 50, 49.9))))
			{
				ABaseBlock* OtherBlock = Cast<ABaseBlock>(Hit.GetActor());
				if (OtherBlock && OtherBlock->bIsYeetable)
				{
					if (!BlocksToIgnore.Contains(OtherBlock))
						BlocksToYeet.Add(OtherBlock);
				}
				else
					return false;
			}
		}
		for (auto& BlockToYeet : BlocksToYeet)
		{
			BlockToYeet->BlocksToIgnore.Add(this);
			BlockToYeet->BlocksToIgnore = BlockToYeet->BlocksToIgnore.Union(BlocksToIgnore);
			if (!BlockToYeet->PreYeet())
				return false;
		}
		PostYeet();
		return true;
	}
	return false;
}

void ABaseBlock::PostYeet()
{
	WorldNum = !WorldNum;
	SetActorLocation(FVector(GetActorLocation().X, -GetActorLocation().Y, -GetActorLocation().Z));
	for (int i = 0; i < LinkedBlocks.Num(); i++)
	{
		if (LinkedBlocks[i] == Mesh)
			continue;
		float Distance;
		ABigFlip* BigFlip = UBlocksGameInstance::GetFlipper(this);
		if (GetActorLocation().Z > 0)
			Distance = -(LinkedBlocks[i]->GetComponentLocation() - Mesh->GetComponentLocation()).Z;
		else
			Distance = (LinkedBlocks[i]->GetComponentLocation() - Mesh->GetComponentLocation()).Z;
		LinkedBlocks[i]->AddWorldOffset(FVector(0, 0, Distance * 2 * FMath::Sign(GetActorLocation().Z)));
	}
	dOnYeeted.Broadcast(WorldNum);
	BlocksToIgnore.Empty();
}

void ABaseBlock::MoveBlock(FVector Direction, ABullet* Bullet)
{
	if (FMath::Abs(Direction.X) > FMath::Abs(Direction.Z))
	{
		Direction.X /= FMath::Abs(Direction.X);
		Direction.Z = 0;
	}
	else if ((FMath::Abs(Direction.Z) - FMath::Abs(Direction.X)) < 0.027 && Bullet)
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
		if (!World->SweepSingleByChannel(Hit,
			StartPosition,
			EndPosition,
			GetActorRotation().Quaternion(),
			ECollisionChannel::ECC_Camera,
			FCollisionShape::MakeBox(FVector(49.9, 50, 49.9)),
			Params))
		{
			SetActorLocation(EndPosition);
		}
	}
}
