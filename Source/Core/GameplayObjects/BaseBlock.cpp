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
}

// Called when the game starts or when spawned
void ABaseBlock::BeginPlay()
{
	Super::BeginPlay();
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ABaseBlock::OnOverlapBegin);
}

// Called every frame
void ABaseBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseBlock::UpdateFlipFlopTime(float DeltaTime)
{
	if (FlipFlopTime)
		FlipFlopTime -= DeltaTime;
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

