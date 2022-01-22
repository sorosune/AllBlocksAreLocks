// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBlock.h"
#include "Core/Actors/BlocksPlayer.h"

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
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ABaseBlock::OnOverlapBegin);
	WorldNum = GetActorLocation().Z < 0;
}

void ABaseBlock::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABlocksPlayer* Player = Cast<ABlocksPlayer>(OtherActor);
	if (Player)
	{
		OnPlayerOverlap(Player);
		Player->OnBlockTouch(this);
	}
}

bool ABaseBlock::PreYeet()
{
	if (LinkedBlocks.Num())
		return LinkedPreYeet();
	UWorld* World = GetWorld();
	if (World)
	{
		TArray<FHitResult> Hits;
		FVector Position = FVector(GetActorLocation().X, GetActorLocation().Y, -GetActorLocation().Z);
		FVector Forward;
		if (World->SweepMultiByChannel(Hits,
			Position,
			Position,
			GetActorRotation().Quaternion(),
			ECollisionChannel::ECC_Camera,
			FCollisionShape::MakeBox(FVector(50, 50, 50))))
		{
			for (auto& Hit : Hits)
			{
				if ((Hit.GetComponent()->GetComponentLocation() - Position).SizeSquared() > 99.9 * 99.9)
					continue;
				ABaseBlock* OtherBlock = Cast<ABaseBlock>(Hit.GetActor());
				if (OtherBlock)
				{
					if (!BlocksToIgnore.Contains(OtherBlock))
					{
						OtherBlock->BlocksToIgnore.Add(this);
						OtherBlock->BlocksToIgnore = OtherBlock->BlocksToIgnore.Union(BlocksToIgnore);
						if (!OtherBlock->PreYeet())
							return false;
					}
				}
				else
					return false;
			}
		}
		PostYeet();
		return true;
	}
	return false;
}

bool ABaseBlock::LinkedPreYeet()
{
	UWorld* World = GetWorld();
	if (World)
	{
		TSet<ABaseBlock*> BlocksToYeet;
		for (auto& Block : LinkedBlocks)
		{
			TArray<FHitResult> Hits;
			FVector Position = FVector(Block->GetComponentLocation().X, Block->GetComponentLocation().Y, -Block->GetComponentLocation().Z);
			FVector Forward;
			if (World->SweepMultiByChannel(Hits,
				Position,
				Position,
				GetActorRotation().Quaternion(),
				ECollisionChannel::ECC_Camera,
				FCollisionShape::MakeBox(FVector(50, 50, 50))))
			{
				for (auto& Hit : Hits)
				{
					if ((Hit.GetComponent()->GetComponentLocation() - Position).SizeSquared() > 99.9 * 99.9)
						continue;
					ABaseBlock* OtherBlock = Cast<ABaseBlock>(Hit.GetActor());
					if (OtherBlock)
					{
						if (!BlocksToIgnore.Contains(OtherBlock))
							BlocksToYeet.Add(OtherBlock);
					}
					else
						return false;
				}
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
		if (GetActorLocation().Z > 0)
			Distance = -(LinkedBlocks[i]->GetComponentLocation() - Mesh->GetComponentLocation()).Z;
		else
			Distance = (LinkedBlocks[i]->GetComponentLocation() - Mesh->GetComponentLocation()).Z;
		LinkedBlocks[i]->AddLocalOffset(FVector(0, 0, Distance * 2 * FMath::Sign(GetActorLocation().Z)));
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
			FCollisionShape::MakeBox(FVector(50, 45, 45)),
			Params))
		{
			SetActorLocation(EndPosition);
		}
	}
}
