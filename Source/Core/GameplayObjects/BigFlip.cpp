// Fill out your copyright notice in the Description page of Project Settings.


#include "BigFlip.h"

#include "BaseBlock.h"
#include "Core/Actors/BlocksPlayer.h"
#include "Core/GameSystems/BlocksGameInstance.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


bool ABigFlip::Flip()
{
	FHitResult Hit;
	ABlocksPlayer * player = Cast<ABlocksPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));
	if(!player)
		return false;
	FVector Position = player->GetActorLocation();
	Position = FVector(Position.X, Position.Y, -Position.Z);
	FVector Forward;
	if (GetWorld()->SweepSingleByObjectType(Hit,Position,Position,
		FQuat::Identity,	ECollisionChannel::ECC_WorldDynamic,
		FCollisionShape::MakeBox(FVector(45, 40, 90))))
			return false;
	FlipInternal(player);
	return true;
}

bool ABigFlip::TeleportPlayer(ABaseBlock* block)
{
	FHitResult Hit;
	ABlocksPlayer * player = Cast<ABlocksPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));
	if(!player)
		return false;
	FVector Position = block->GetActorLocation();
	if(Position.Z < 0)
		Position.Z -= 150;
	else
		Position.Z += 150;
	FVector Forward;
	if (GetWorld()->SweepSingleByObjectType(Hit,Position,Position,
		FQuat::Identity,	ECollisionChannel::ECC_WorldDynamic,
		FCollisionShape::MakeBox(FVector(45, 40, 90))))
			return false;
	if(Position.Z < 0)
		FlipInternal(player);
	Position.Z = abs(Position.Z);
	Position.Y = 0;
	player->SetActorLocation(Position, false, nullptr, ETeleportType::ResetPhysics);
	return true;
}

ABigFlip::ABigFlip()
{
}

void ABigFlip::BeginPlay()
{
	Super::BeginPlay();
	UBlocksGameInstance * GI = Cast<UBlocksGameInstance>(GetGameInstance());
	if(!GI)
		return;
	if(GI->Flipper)
	{
		Destroy();
		return;
	}
	GI->Flipper = this;
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseBlock::StaticClass(), FoundActors);
	for(AActor * act : FoundActors)
	{
		ABaseBlock * block = Cast<ABaseBlock>(act);
		if(block)
		{
			FAttachmentTransformRules rules(EAttachmentRule::KeepRelative, true);
			block->AttachToActor(this, rules);
		}
	}
}

void ABigFlip::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UBlocksGameInstance * GI = Cast<UBlocksGameInstance>(GetGameInstance());
	if(GI && GI->Flipper == this)
		GI->Flipper = nullptr;
}

void ABigFlip::FlipInternal(ABlocksPlayer * Player)
{
	Player->WorldNum = !Player->WorldNum;
	AddActorLocalRotation(FRotator(0, 0, 180),false, nullptr, ETeleportType::ResetPhysics);
	dOnFlip.Broadcast();
}



