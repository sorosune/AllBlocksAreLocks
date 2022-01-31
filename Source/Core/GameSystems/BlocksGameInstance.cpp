// Fill out your copyright notice in the Description page of Project Settings.


#include "BlocksGameInstance.h"

#include "GameTimer.h"

ABigFlip* UBlocksGameInstance::GetFlipper(UObject* Context)
{
	UBlocksGameInstance * game = GetMyGameInstance(Context);
	if(!game)
		return nullptr;
	return game->Flipper;
}

UBlocksGameInstance* UBlocksGameInstance::GetMyGameInstance(const UObject* Context)
{
	UWorld * World = Context->GetWorld();
	if (!World)
		return nullptr;
	UBlocksGameInstance* GameInstance = Cast<UBlocksGameInstance>(World->GetGameInstance());
	if(!GameInstance)
		return nullptr;
	return GameInstance;
}

bool UBlocksGameInstance::LoadNextLevel(const UObject* Context)
{
	UWorld * World = Context->GetWorld();
	if (!World)
		return false;
	UBlocksGameInstance* GameInstance = Cast<UBlocksGameInstance>(World->GetGameInstance());
	if(GameInstance)
		return GameInstance->LoadLevelInternal();
	return false;
}

AGameTimer* UBlocksGameInstance::GetGameTimer(UObject* Context)
{
	UBlocksGameInstance* GI = GetMyGameInstance(Context);
	if (!GI)
		return nullptr;
	return GI->GameTimer;
}

UBlocksGameInstance::UBlocksGameInstance()
{
}

void UBlocksGameInstance::Init()
{
	Super::Init();

	GameTimeInSeconds = 0.f;
}

void UBlocksGameInstance::Shutdown()
{
	Super::Shutdown();

	GameTimeInSeconds = 0.f;
}

bool UBlocksGameInstance::LoadLevelInternal()
{
	if(!(LevelNum<LevelNames.Num() && LevelNames[LevelNum].Len()))
		return false;
	GameTimer->SaveTimer();
	UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelNames[LevelNum++]));
	return true;
}
