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
	if(LinkBlocks)
		GetComponents(LinkedBlocks);
}

void ABaseBlock::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ABlocksPlayer* Player = Cast<ABlocksPlayer>(OtherActor);
	if (Player && !bOnCooldown)
	{
		OnPlayerOverlap(Player, OtherComp);
		Player->OnBlockTouch(this, HitComp);
		bIsPlayerTouching = true;
		if (!Player->dOnPlayerFalling.IsAlreadyBound(this, &ABaseBlock::ResetPlayerTouching))
			Player->dOnPlayerFalling.AddDynamic(this, &ABaseBlock::ResetPlayerTouching);
	}
	RootComponent->SetAbsolute(false,true,false);
}

void ABaseBlock::ResetPlayerTouching()
{
	UWorld* World = GetWorld();
	if (World)
	{
		bIsPlayerTouching = false;
		Cast<ABlocksPlayer>(UGameplayStatics::GetPlayerCharacter(World, 0))->dOnPlayerFalling.RemoveDynamic(this, &ABaseBlock::ResetPlayerTouching);
	}
}

ABullet* ABaseBlock::SpawnBullet(FVector Direction)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FVector Position;
		ABullet* NewBullet = nullptr;
		if (FMath::Abs(Direction.X) >= FMath::Abs(Direction.Z))
		{
			Position = GetActorLocation() + 125 * GetActorForwardVector() * FMath::Sign(Direction.X);
			NewBullet = World->SpawnActor<ABullet>(Position, FRotator(1, 0, 0));
			if(!NewBullet)
				return nullptr;
			NewBullet->Direction = FVector2D(FMath::Sign(Direction.X), 0);
			NewBullet->Direction.Normalize();
		}
		else
		{
			Position = GetActorLocation() + 125 * GetActorUpVector() * FMath::Sign(Direction.Z);
			NewBullet = World->SpawnActor<ABullet>(Position, FRotator(1, 0, 0));
			if(!NewBullet)
				return nullptr;
			NewBullet->Direction = FVector2D(0, FMath::Sign(Direction.Z));
			NewBullet->Direction.Normalize();
		}
		return NewBullet;
	}
	return nullptr;
}

bool ABaseBlock::MoveCharacterOnTop(ACharacter* Actor)
{
	UWorld* World = GetWorld();
	if (Actor && World)
	{
		FHitResult Hit1;
		FHitResult Hit2;
		FVector Position1 = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 100);
		FVector Position2 = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 200);
		if (!CheckGridSpace(Position1, Hit1) && !CheckGridSpace(Position2, Hit2))
		{
			FVector NewPosition = FVector(GetActorLocation().X, GetActorLocation().Y, -GetActorLocation().Z + 150);
			return Actor->SetActorLocation(NewPosition);
		}
	}
	return false;
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
			if (CheckGridSpace(Position, Hit))
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
	ABlocksPlayer* Player = Cast<ABlocksPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->GetCharacterMovement()->SetBase(nullptr);
	SetActorLocation(FVector(GetActorLocation().X, -GetActorLocation().Y, -GetActorLocation().Z),
		false, nullptr, ETeleportType::ResetPhysics);
	for (int i = 0; i < LinkedBlocks.Num(); i++)
	{
		if (LinkedBlocks[i] == Mesh)
			continue;
		float Distance;
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
	FHitResult Hit;
	FVector Position = GetActorLocation() + 100 * Direction;
	if (!CheckGridSpace(Position, Hit))
	{
		SetActorLocation(Position);
	}
}

bool ABaseBlock::CheckGridSpace(FVector Position, FHitResult& HitResult)
{
	UWorld* World = GetWorld();
	if (World)
		return World->SweepSingleByChannel(HitResult,
										   Position,
										   Position,
			                               FQuat::Identity,
										   ECollisionChannel::ECC_Camera,
										   FCollisionShape::MakeBox(FVector(49.9, 50, 49.9)));
	return false;
}
