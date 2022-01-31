// Fill out your copyright notice in the Description page of Project Settings.


#include "GameTimer.h"

#include "BlocksGameInstance.h"


AGameTimer::AGameTimer()
{
	PrimaryActorTick.bCanEverTick = true;
}


void AGameTimer::BeginPlay()
{
	Super::BeginPlay();
	
	UBlocksGameInstance* GameInstance = UBlocksGameInstance::GetMyGameInstance(this);
	if (!GameInstance)
		return;
	
	if (GameInstance->GameTimer)
	{
		Destroy();
		return;
	}
	GameInstance->GameTimer = this;
	
	GameTimeInSeconds = GameInstance->GameTimeInSeconds;
}

void AGameTimer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UBlocksGameInstance* GameInstance = UBlocksGameInstance::GetMyGameInstance(this);
	if(GameInstance && GameInstance->GameTimer == this)
		GameInstance->GameTimer = nullptr;
}


void AGameTimer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GameTimeInSeconds += DeltaTime;
}

void AGameTimer::SaveTimer()
{
	if (UBlocksGameInstance* GameInstance = UBlocksGameInstance::GetMyGameInstance(this))
	{
		GameInstance->GameTimeInSeconds = GameTimeInSeconds;
	}
}


FText AGameTimer::GetTimerText()
{
	const FTimespan Timespan = FTimespan::FromSeconds(GameTimeInSeconds);

	const int32 Minutes = Timespan.GetMinutes();
	const int32 Seconds = Timespan.GetSeconds();
	const int32 Millis = Timespan.GetFractionMilli();

	const FString Result = FString::Printf(TEXT("%02d:%02d:%03d"), Minutes, Seconds, Millis);
	return FText::FromString(Result);
}
