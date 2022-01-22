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

void ABaseBlock::PreYeet()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FHitResult Hit;
		FVector Position = FVector(GetActorLocation().X, GetActorLocation().Y, -GetActorLocation().Z);
		FVector Forward;
		if (World->SweepSingleByChannel(Hit,
			Position,
			Position,
			GetActorRotation().Quaternion(),
			ECollisionChannel::ECC_Camera,
			FCollisionShape::MakeBox(FVector(45, 45, 45))))
		{
			ABaseBlock* OtherBlock = Cast<ABaseBlock>(Hit.GetActor());
			if (OtherBlock)
			{
				OtherBlock->PostYeet();
			}
			else if (Hit.GetActor())
				return;
		}
	}
	PostYeet();
}

void ABaseBlock::PostYeet()
{
	WorldNum = !WorldNum;
	SetActorLocation(FVector(GetActorLocation().X, -GetActorLocation().Y, -GetActorLocation().Z));
	dOnYeeted.Broadcast(WorldNum);
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
			FCollisionShape::MakeBox(FVector(45, 45, 45)),
			Params))
		{
			SetActorLocation(EndPosition);
		}
	}
}
