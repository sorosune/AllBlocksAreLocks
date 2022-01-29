// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "BlocksPlayer.generated.h"

class ABaseBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActorFalling);

UCLASS(Blueprintable, BlueprintType)
class ALLBLOCKSARELOCKS_API ABlocksPlayer: public ACharacter
{
	GENERATED_BODY()

//======================================================================================
// Blueprint Public
public:

	// Blueprint Flags

	// Blueprint Variables

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	
	/** Z-Velocity after jump release */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReleaseDrop;
	
	// Getters

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Setters

	// Wrappers

	// External Virtual Functions

	// External Overrides

	// External Regular Functions

	UFUNCTION(BlueprintCallable)
	void Die() { UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this))); };

	UFUNCTION(BlueprintCallable)
	void EnableWorldSwap();

	UFUNCTION(BlueprintCallable)
	void DisableWorldSwap();

	// External Events

	UPROPERTY(BlueprintAssignable)
	FOnActorFalling dOnPlayerFalling;

//======================================================================================
// C++ Public
public:

	// Flags

	// Initialized Variables

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int WorldNum = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int NumWorldSwaps = 0;

	// 0 means we can manually swap
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int ManualSwapLocks = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bHasKey = false;

	// Constructor
	ABlocksPlayer();

	// Initializers, and Actor Lifecycle Functions

	virtual void BeginPlay() override;

	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;

	void OnBlockTouch(ABaseBlock* Block, UPrimitiveComponent* BlockMesh);

	virtual void Falling() override;

	virtual void StopJumping() override;;

//======================================================================================
// C++ Protected
protected:

	// Internal Variables

	// Internal Virtual Functions

	// Internal Overrides

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Internal Regular Functions

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for forwards/backward input */
	void MoveBackward(float Value);

	void FireBullet();

	template<int T>
	void ChangeAimDirection();

	void ManualWorldSwap();

	// Internal Events and Implementations 

//======================================================================================
// C++ Private
private:

	// Internal Variables

	FVector2D AimDirection = FVector2D(1, 0);

	// Overrides

	// Regular Functions
};
