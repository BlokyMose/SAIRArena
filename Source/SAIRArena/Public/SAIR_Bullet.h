// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SAIR_Bullet.generated.h"

UCLASS()
class SAIRARENA_API ASAIR_Bullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASAIR_Bullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Components");
	class UProjectileMovementComponent* BulletMovement;
	UPROPERTY(EditAnywhere, Category="Components")
	class UStaticMeshComponent* BulletMesh;
	UPROPERTY(BlueprintReadWrite, Category="Bullet General")
	float LifeDuration;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnBulletHit (AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit );

	UFUNCTION(BlueprintNativeEvent)
	void Die();
};
