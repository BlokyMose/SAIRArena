// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interact/InteractableBase.h"
#include "Interact/InterfaceInteract.h"
#include "Test/SAIR_ForceActor.h"
#include "SAIR_Bullet.h"
#include "SAIR_CharacterBase.generated.h"

UCLASS()
class SAIRARENA_API ASAIR_CharacterBase : public ACharacter //, public IInterfaceInteract
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAIR_CharacterBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Camera")
	USpringArmComponent* SpringArmCom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Camera")
	UCameraComponent* CameraCom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Player")
	UStaticMeshComponent* MeshCom;

	UPROPERTY(EditDefaultsOnly, Category="Spawning")
	TSubclassOf<ASAIR_ForceActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category="Timeline")
	UCurveFloat* CurveFloat;

protected:
	void MoveForward(float value);
	void ThrustMode();
	void MoveRight(float value);
	void TurnAtRate(float value);
	void LookUpAtRate(float value);
	void Turn(float value);
	void LookUp(float value);
	void InteractPress();
	void ThrustPress();
	UPROPERTY(VisibleAnywhere, Category= "Movement")
	bool bIsThrusting;
	UPROPERTY(VisibleAnywhere, Category= "Shooting")
	bool bIsFiring;
	UPROPERTY(VisibleAnywhere, Category= "Movement")
	bool bIsMovingForward;
	UPROPERTY(VisibleAnywhere, Category= "Movement")
	bool bIsMovingSide;
	UPROPERTY(EditAnywhere, Category= "Movement")
	float SpeedNormal;
	UPROPERTY(EditAnywhere, Category= "Movement")
	float SpeedThrust;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Camera")
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Camera")
	float BaseLookUpRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Interaction")
	float TraceDistance;
	UPROPERTY(EditAnywhere, Category= "Movement")
	float ZoomCameraNormal;
	UPROPERTY(EditAnywhere, Category= "Movement")
	float ZoomCameraThrust;
	UPROPERTY(EditAnywhere, Category= "Movement")
	float ZoomSpeed;
	UPROPERTY(EditAnywhere, Category= "Movement")
	float DivRotateWhenThrusting;

	UFUNCTION(BlueprintNativeEvent)
	void TraceForward();
	void TraceForward_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void MovingCallback(bool isMoving);
	
	UFUNCTION(BlueprintNativeEvent)
	void Thrusting(bool BPbIsThrusting);

	//Impulse
	UPROPERTY(EditAnywhere)
	float ImpulseForce;
	void FireForward();
	void Firing(float DeltaTime);

	//Radial Impulse
	UPROPERTY(EditAnywhere)
	bool bApplyRadialForce;
	UPROPERTY(EditAnywhere)
	float ImpactRadius;
	UPROPERTY(EditAnywhere)
	float RadialImpactForce;
	TArray<FHitResult> HitActors;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> CamShake;

	UFUNCTION()
	void SpawnObject(FVector loc, FRotator rot);

	UPROPERTY(EditAnywhere, Category = "Shooting");
	TSubclassOf<class ASAIR_Bullet> BulletClass;
	UPROPERTY(EditAnywhere, Category="Shooting")
	int RateOfFire;
	float LastShotTime;
	
public:	
	// Called to bind functionality to input

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Landed(const FHitResult& Hit) override;

private:
	AActor* FocusedActor;

	FVector StartScale;
	FVector TargetScale;
	FTimeline SquashTimeline;

	UFUNCTION()
	void SquashProgress(float Value);
};
	
