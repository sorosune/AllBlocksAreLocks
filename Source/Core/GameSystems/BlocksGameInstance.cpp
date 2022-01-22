// Fill out your copyright notice in the Description page of Project Settings.


#include "BlocksGameInstance.h"

ABigFlip* UBlocksGameInstance::GetFlipper(UObject* Context)
{
	UWorld * world = Context->GetWorld();
	if(!world)
		return nullptr;
	UBlocksGameInstance * game = Cast<UBlocksGameInstance>(world->GetGameInstance());
	if(!game)
		return nullptr;
	return game->Flipper;
}

UBlocksGameInstance::UBlocksGameInstance()
{

}
