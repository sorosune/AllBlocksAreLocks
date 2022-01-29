// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlocksPlayer.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Core/GameplayObjects/Bullet.h"
#include "Core/GameplayObjects/BounceBlock.h"
#include "Core/HelperFiles/DefinedDebugHelpers.h"
#include "Core/GameSystems/BlocksGameInstance.h"

ABlocksPlayer::ABlocksPlayer()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void ABlocksPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bullet Actions
	PlayerInputComponent->BindAction("FireBullet", IE_Pressed, this, &ABlocksPlayer::FireBullet);
	PlayerInputComponent->BindAction("AimUp", IE_Pressed, this, &ABlocksPlayer::ChangeAimDirection<0>);
	PlayerInputComponent->BindAction("AimDown", IE_Pressed, this, &ABlocksPlayer::ChangeAimDirection<1>);
	PlayerInputComponent->BindAction("AimDiagonalUp", IE_Pressed, this, &ABlocksPlayer::ChangeAimDirection<2>);
	PlayerInputComponent->BindAction("AimDiagonalDown", IE_Pressed, this, &ABlocksPlayer::ChangeAimDirection<3>);
	PlayerInputComponent->BindAction("AimLeft", IE_Pressed, this, &ABlocksPlayer::ChangeAimDirection<4>);
	PlayerInputComponent->BindAction("AimRight", IE_Pressed, this, &ABlocksPlayer::ChangeAimDirection<5>);
	PlayerInputComponent->BindAction("DaBigFlip", IE_Pressed, this, &ABlocksPlayer::ManualWorldSwap);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABlocksPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveBackward", this, &ABlocksPlayer::MoveBackward);
}

void ABlocksPlayer::BeginPlay()
{
	Super::BeginPlay();
	SetActorRotation(FRotator(0, 0, 0));
}

void ABlocksPlayer::TickActor(float DeltaTime, ELevelTick Tick, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, Tick, ThisTickFunction);
	FVector pos = GetActorLocation();
	if(GetActorLocation().Z < 0)
	{
		ABigFlip * flipper = UBlocksGameInstance::GetFlipper(this);
		if(flipper)
		{
			if(flipper->Flip())
			{
				pos.Z = 0.01;
				FVector newVel = GetMovementComponent()->Velocity;
				SetActorLocation(pos,false,nullptr, ETeleportType::TeleportPhysics);
				newVel.Z *= -.90;
				GetMovementComponent()->Velocity = newVel;
			}
		}
	}
}

void ABlocksPlayer::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator YawRotation(0, 0, 0);
		FVector Location = GetActorLocation();
		SetActorLocation(FVector(Location.X, 0, Location.Z));
		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABlocksPlayer::MoveBackward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator YawRotation(0, 180, 0);
		FVector Location = GetActorLocation();
		SetActorLocation(FVector(Location.X, 0, Location.Z));
		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABlocksPlayer::FireBullet()
{
	FVector AimDirection3D = FVector(AimDirection.X, 0, AimDirection.Y);
	FVector Location = GetActorLocation() + (110 * AimDirection3D);
	UWorld* World = GetWorld();
	if (World)
	{
		ABullet* Bullet = World->SpawnActor<ABullet>(Location, FRotator(0, 0, 0));
		if(Bullet)
			Bullet->Direction = AimDirection;
	}
}

template<int T>
void ABlocksPlayer::ChangeAimDirection()
{
	FVector NewDirection;
	switch (T)
	{
	case 0: // Up
		AimDirection = FVector2D(0, 1);
		break;
	case 1: // Down
		AimDirection = FVector2D(0, -1);
		break;
	case 2: // Diagonal Up
		NewDirection = FVector(0, 0, 1) + GetActorForwardVector();
		AimDirection = FVector2D(NewDirection.X, NewDirection.Z);
		AimDirection.Normalize();
		break;
	case 3: // Diagonal Down
		NewDirection = FVector(0, 0, -1) + GetActorForwardVector();
		AimDirection = FVector2D(NewDirection.X, NewDirection.Z);
		AimDirection.Normalize();
		break;
	case 4: // Left
		AimDirection = FVector2D(-1, 0);
		break;
	case 5: // Right
		AimDirection = FVector2D(1, 0);
		break;
	}
}

void ABlocksPlayer::ManualWorldSwap()
{
	if (!ManualSwapLocks && NumWorldSwaps)
	{
		if (UBlocksGameInstance::GetFlipper(this)->Flip())
		{
			WorldNum = !WorldNum;
			NumWorldSwaps--;
		}
	}
}

void ABlocksPlayer::EnableWorldSwap()
{
	if (ManualSwapLocks)
		ManualSwapLocks--;
}

void ABlocksPlayer::DisableWorldSwap()
{
	ManualSwapLocks++;
}

void ABlocksPlayer::OnBlockTouch(ABaseBlock* Block, UPrimitiveComponent* BlockMesh)
{
	if (Cast<ABounceBlock>(Block) && BlockMesh)
	{
		SCREENMSG("BOUNCE");
		FVector Velocity = GetCharacterMovement()->GetLastUpdateVelocity();
		float DeltaX = 0;
		float DeltaZ = 0;
		FVector Direction = GetActorLocation() - BlockMesh->GetComponentLocation();
		Direction.Normalize();
		if (FMath::Abs(Direction.X) > FMath::Abs(Direction.Z))
		{
			DeltaX = (FMath::Abs(Velocity.X) + Block->BounceMagnitude) * -FMath::Sign(Velocity.X);
			GetCharacterMovement()->AddImpulse(FVector(DeltaX, Velocity.Y, Velocity.Z), true);
		}
		else if ((FMath::Abs(Direction.Z) - FMath::Abs(Direction.X)) < 0.5)
		{
			DeltaX = (FMath::Abs(Velocity.X) + Block->BounceMagnitude) * -FMath::Sign(Velocity.X);
			DeltaZ = (FMath::Abs(Velocity.Z) + Block->BounceMagnitude) * -FMath::Sign(Velocity.Z);
			GetCharacterMovement()->AddImpulse(FVector(DeltaX, Velocity.Y, DeltaZ), true);
		}
		else
		{
			DeltaZ = (FMath::Abs(Velocity.Z) + Block->BounceMagnitude) * -FMath::Sign(Velocity.Z);
			GetCharacterMovement()->AddImpulse(FVector(Velocity.X, Velocity.Y, DeltaZ), true);
		}
	}
}

void ABlocksPlayer::Falling()
{
	Super::Falling();
	dOnPlayerFalling.Broadcast();
}

void ABlocksPlayer::StopJumping()
{
	Super::StopJumping();
	FVector newVel = GetCharacterMovement()->Velocity;
	newVel.Z = FMath::Min(ReleaseDrop, newVel.Z);
	GetCharacterMovement()->Velocity = newVel;
}
