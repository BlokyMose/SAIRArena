// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"

#include "SAIR_SpaceCraftBase.generated.h"

UCLASS()
class SAIRARENA_API ASAIR_SpaceCraftBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASAIR_SpaceCraftBase();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Camera")
	USpringArmComponent* SpringArmCom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Camera")
	UCameraComponent* CameraCom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Player")
	UStaticMeshComponent* MeshCom;

protected:

	UFloatingPawnMovement* FloatingPawnMovement;
	
	void MoveForward(float value);
	void MoveRight(float value);
	void TurnAtRate(float value);
	void LookUpAtRate(float value);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Camera")
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Camera")
	float BaseLookUpRate;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
