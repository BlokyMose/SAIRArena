// Fill out your copyright notice in the Description page of Project Settings.
#include "SAIR_Bullet.h"

// Sets default values
ASAIR_Bullet::ASAIR_Bullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LifeDuration = 5.0f;
	BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>("BulletMovement");
	BulletMovement->InitialSpeed = 4000.0f;
	BulletMovement->MaxSpeed = 4000.0f;
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");
	SetRootComponent(BulletMesh);
	OnActorHit.AddDynamic(this, &ASAIR_Bullet::OnBulletHit);
}

// Called when the game starts or when spawned
void ASAIR_Bullet::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASAIR_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LifeDuration -= DeltaTime;
	if(LifeDuration<=0)
	{
		Destroy();
	}
}

void ASAIR_Bullet::OnBulletHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1,5,FColor::Red,"HIT");
	LifeDuration = 5.0f;
	Die();
}

void ASAIR_Bullet::Die_Implementation()
{
}

