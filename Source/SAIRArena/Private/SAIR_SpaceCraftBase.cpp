// Fill out your copyright notice in the Description page of Project Settings.


#include "SAIRArena/Public/SAIR_SpaceCraftBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
// Sets default values
ASAIR_SpaceCraftBase::ASAIR_SpaceCraftBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	MeshCom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	MeshCom -> SetupAttachment(RootComponent);
	
	SpringArmCom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmCom"));
	SpringArmCom -> SetupAttachment(MeshCom);

	CameraCom = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraCom"));
	CameraCom -> SetupAttachment(SpringArmCom);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	
	BaseTurnRate = 45.0f;
	BaseLookUpRate = 45.0f;
}

void ASAIR_SpaceCraftBase::MoveForward(float value)
{
	if((Controller)&&(value !=0.0f))
	{
		const FRotator Rotation  = Controller->GetControlRotation();
		const FRotator YawRotation(0,Rotation.Yaw,0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);
	}
}

void ASAIR_SpaceCraftBase::MoveRight(float value)
{
	if((Controller)&&(value !=0.0f))
	{
		const FRotator Rotation  = Controller->GetControlRotation();
		const FRotator YawRotation(0,Rotation.Yaw,0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, value);
	}
}

void ASAIR_SpaceCraftBase::TurnAtRate(float value)
{
	AddControllerYawInput(value * BaseTurnRate * GetWorld()->GetDeltaSeconds());

}

void ASAIR_SpaceCraftBase::LookUpAtRate(float value)
{
	AddControllerPitchInput(value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

// Called to bind functionality to input
void ASAIR_SpaceCraftBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASAIR_SpaceCraftBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASAIR_SpaceCraftBase::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASAIR_SpaceCraftBase::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASAIR_SpaceCraftBase::LookUpAtRate);
}

