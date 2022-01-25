// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.h"
#include "Core/HelperFiles/DefinedDebugHelpers.h"
#include "BaseBlock.generated.h"

class ABlocksPlayer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnYeeted, int, WorldNum);

UCLASS()
class ALLBLOCKSARELOCKS_API ABaseBlock : public AActor
{
	GENERATED_BODY()

//======================================================================================
// Blueprint Public
public:

	// Blueprint Flags

	// Blueprint Variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FlipFlopTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFlipFlopRepeat = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BounceMagnitude = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ABaseBlock* PairedBlock = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPlayerTouching = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsYeetable = true;

	// Getters

	// Setters

	// Wrappers

	// External Virtual Functions

	// External Overrides

	// External Regular Functions

	UFUNCTION(BlueprintCallable)
	ABullet* SpawnBullet(FVector Direction);

	UFUNCTION(BlueprintCallable)
	bool MoveCharacterOnTop(ACharacter* Actor);

	// External Events

	UPROPERTY(BlueprintAssignable)
	FOnYeeted dOnYeeted;

//======================================================================================
// C++ Public
public:

	// Flags

	// Initialized Variables

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UStaticMeshComponent*> LinkedBlocks;

	TSet<ABaseBlock*> BlocksToIgnore;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int WorldNum;

	// Constructor
	ABaseBlock();

	// Initializers, and Actor Lifecycle Functions

	virtual void BeginPlay() override;

	virtual void OnBulletHit(ABullet* Bullet) { ; };

	virtual void OnPlayerOverlap(ABlocksPlayer* Player, UPrimitiveComponent* OverlappedMesh) { ; };

	UFUNCTION()
	virtual bool PreYeet();

	UFUNCTION()
	virtual void PostYeet();

	virtual void MoveBlock(FVector Direction, ABullet* Bullet = nullptr);

	// Returns true if the space is occupied.
	bool CheckGridSpace(FVector Position, FHitResult& HitResult);

//======================================================================================
// C++ Protected
protected:

	// Internal Variables

	FTimerHandle TimerHandle;

	// Internal Virtual Functions

	// Internal Overrides

	// Internal Regular Functions

	// Internal Events and Implementations 

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void ResetPlayerTouching();

//======================================================================================
// C++ Private
private:

	// Internal Variables

	// Overrides

	// Regular Functions
};
