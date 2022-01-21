// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "BaseBlock.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("StaticMesh'/Game/EnvironmentArt/Props/1M_Cube.1M_Cube'"));
	if (MeshFinder.Object)
	{
		Mesh->SetStaticMesh(MeshFinder.Object);
		Mesh->SetWorldScale3D(FVector(0.25, 0.25, 0.25));
		Mesh->SetCollisionProfileName("Bullet");
		Mesh->SetGenerateOverlapEvents(true);
	}
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlapBegin);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector LocalDirection = FVector(Direction.X, 0, Direction.Y);
	AddActorLocalOffset(Speed * LocalDirection * DeltaTime);
}

void ABullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Mesh->SetCollisionProfileName("NoCollision");
	ABaseBlock* BaseBlock = Cast<ABaseBlock>(OtherActor);
	if (BaseBlock)
		BaseBlock->OnBulletHit(this);
	Destroy();
}

