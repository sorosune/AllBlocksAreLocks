// Fill out your copyright notice in the Description page of Project Settings.


#include "BlocksGameInstance.h"

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

void UBlocksGameInstance::SaveGameTimeInSeconds(const UObject* Context, float Value)
{
	if (!GetMyGameInstance(Context))
		return;
	GameTimeSeconds = Value;
}

UBlocksGameInstance::UBlocksGameInstance()
{
}

void UBlocksGameInstance::Init()
{
	Super::Init();
	V_LOG("INIT");
	GameTimeSeconds = 0.f;
}

void UBlocksGameInstance::Shutdown()
{
	Super::Shutdown();
	V_LOG("SHUTDOWN");
	GameTimeSeconds = 0.f;
}

bool UBlocksGameInstance::LoadLevelInternal()
{
	if(!(LevelNum<LevelNames.Num() && LevelNames[LevelNum].Len()))
		return false;
	UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelNames[LevelNum++]));
	return true;
}
