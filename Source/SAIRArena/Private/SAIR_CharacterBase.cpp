// Fill out your copyright notice in the Description page of Project Settings.
#include "SAIRArena/Public/SAIR_CharacterBase.h"

#include "AITypes.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SAIR_Bullet.h"

// Sets default values
ASAIR_CharacterBase::ASAIR_CharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshCom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	MeshCom -> SetupAttachment(RootComponent);
	
	SpringArmCom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmCom"));
	SpringArmCom -> SetupAttachment(RootComponent);
	SpringArmCom->TargetOffset.X = ZoomCameraNormal;

	CameraCom = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraCom"));
	CameraCom -> SetupAttachment(SpringArmCom);

	BaseTurnRate = 40.0f;
	BaseLookUpRate = 40.0f;
	TraceDistance = 2000;
	DivRotateWhenThrusting = 2.0f;
	SpeedNormal = 2048.0f;
	SpeedThrust = 20480.0f;
	bIsThrusting = false;
	bIsMovingForward = false;
	bIsMovingSide = false;

	bIsFiring = false;
	RateOfFire = 150;
	LastShotTime = 0;
	
	ZoomCameraNormal = 0.0f;
	ZoomCameraThrust = -200.0f;
	ZoomSpeed = 10.0f;
	
	ImpulseForce = 500.0f;
	bApplyRadialForce = true;
	RadialImpactForce = 2000.0f;
	ImpactRadius = 200.0f;
	
	StartScale = FVector(1,1,1);
	TargetScale = FVector(1.3f,1.3f,0.8f);
}

void ASAIR_CharacterBase::MoveForward(float value)
{
	if((Controller)&&(value !=0.0f))
	{
		bIsMovingForward = true;
		const FVector Direction = CameraCom->GetForwardVector();
		AddMovementInput(Direction, value);
		MovingCallback(true);
	}
	else
	{
		bIsMovingForward = false;
	}
	MovingCallback(bIsMovingForward?true:bIsMovingSide?true:false);
	ThrustMode();
}

void ASAIR_CharacterBase::ThrustMode()
{
	if(bIsMovingForward||bIsMovingSide) 
	{
		if(bIsThrusting == true)
		{
			Thrusting(true);
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CamShake,0.15f);
			if(SpringArmCom->TargetOffset.X > ZoomCameraThrust)
			{
				SpringArmCom->TargetOffset.X  -= ZoomSpeed;
			}
		}
		else
		{
			Thrusting(false);
			if(SpringArmCom->TargetOffset.X  < ZoomCameraNormal)
			{
				SpringArmCom->TargetOffset.X  += ZoomSpeed;
			}
		}
	}
	else
	{
		Thrusting(false);
		if(SpringArmCom->TargetOffset.X  < ZoomCameraNormal)
		{
			SpringArmCom->TargetOffset.X  += ZoomSpeed;
		}
	}
}

void ASAIR_CharacterBase::MoveRight(float value)
{
	if((Controller)&&(value !=0.0f))
	{
		bIsMovingSide = true;
		const FRotator Rotation  = Controller->GetControlRotation();
		const FRotator YawRotation(0,Rotation.Yaw,0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction,  value);
	}
	else
	{
		bIsMovingSide = false;
	}
	MovingCallback(bIsMovingForward?true:bIsMovingSide?true:false);
	ThrustMode();
}

void ASAIR_CharacterBase::TurnAtRate(float value)
{
	if(bIsThrusting)
	{
		AddControllerYawInput(value/DivRotateWhenThrusting * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
	else
	{
		AddControllerYawInput(value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void ASAIR_CharacterBase::LookUpAtRate(float value)
{
	if(bIsThrusting)
	{
		AddControllerPitchInput(value/DivRotateWhenThrusting * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
	else
	{
		AddControllerPitchInput(value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void ASAIR_CharacterBase::Turn(float value)
{
	if(bIsThrusting)
	{
		AddControllerYawInput(value/DivRotateWhenThrusting * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
	else
	{
		AddControllerYawInput(value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void ASAIR_CharacterBase::LookUp(float value)
{
	if(bIsThrusting)
	{
		AddControllerPitchInput(value/DivRotateWhenThrusting * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
	else
	{
		AddControllerPitchInput(value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void ASAIR_CharacterBase::InteractPress()
{
	TraceForward();

	if(FocusedActor)
	{
		IInterfaceInteract* interface = Cast<IInterfaceInteract>(FocusedActor);
		if(interface)
		{
			interface->Execute_OnInteract(FocusedActor, this);
		}
	}
}

void ASAIR_CharacterBase::ThrustPress()
{
	bIsThrusting = !bIsThrusting;
	
	if(bIsThrusting)
	{
		GetCharacterMovement()->MaxAcceleration = SpeedThrust;
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CamShake,0.5f);
	}
	else
	{
		GetCharacterMovement()->MaxAcceleration = SpeedNormal;
	}
}

void ASAIR_CharacterBase::TraceForward_Implementation()
{
	FVector Loc;
	FRotator Rot;
	FHitResult Hit;

	GetController()->GetPlayerViewPoint(Loc, Rot);
	FVector Start = Loc;
	FVector End = Start + (Rot.Vector()*TraceDistance);
	FCollisionQueryParams TraceParams;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit,Start,End, ECC_Visibility,TraceParams);

	// DrawDebugLine(GetWorld(), Start, End, FColor::Orange, false, 2.0f);

	if(bHit)
	{
		// DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5,5,5), FColor::Emerald, false, 2.0f);

		
		AActor* Interactable = Hit.GetActor();

		if(Interactable)
		{
			if(Interactable!=FocusedActor)
			{
				if(FocusedActor)
				{
					IInterfaceInteract* interface = Cast<IInterfaceInteract>(FocusedActor);
					if(interface)
					{
						interface->Execute_EndFocus(FocusedActor);
					}
				}

				IInterfaceInteract* interface = Cast<IInterfaceInteract>(Interactable);
				if(interface)
				{
					interface->Execute_StartFocus(Interactable);
				}

				FocusedActor = Interactable;
			}
		}
		else
		{
			if(FocusedActor)
			{
				IInterfaceInteract* interface = Cast<IInterfaceInteract>(FocusedActor);
				if(interface)
				{
					interface->Execute_EndFocus(FocusedActor);
				}
			}
			FocusedActor = nullptr;
		}
	}
}

void ASAIR_CharacterBase::MovingCallback_Implementation(bool isMoving)
{
}

void ASAIR_CharacterBase::Thrusting_Implementation(bool BPbIsThrusting)
{
	
}

void ASAIR_CharacterBase::FireForward()
{
	bIsFiring = !bIsFiring;
	//====================
	/*
	FVector Loc;
	FRotator Rot;
	FHitResult Hit;

	GetController()->GetPlayerViewPoint(Loc, Rot);
	FVector Start = Loc;
	FVector End = Start + (Rot.Vector()*TraceDistance);
	FCollisionQueryParams TraceParams;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit,Start,End, ECC_Visibility,TraceParams);

	if(bHit)
	{
		SpawnObject(Hit.Location, Rot);//for spawning
		if(Hit.GetActor()->IsRootComponentMovable())
		{
			UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(Hit.GetActor()->GetRootComponent());
			if(MeshComponent)
			{
				FVector CameraForward = CameraCom->GetForwardVector();
				MeshComponent->AddImpulse(CameraForward*ImpulseForce*MeshComponent->GetMass());
			}
		}
		if(bApplyRadialForce)
		{
			FCollisionShape ShepereCol = FCollisionShape::MakeSphere(ImpactRadius);
			bool bSweepHit = GetWorld()->SweepMultiByChannel(HitActors, Hit.Location, Hit.Location+FVector(0.01f,0.01f,0.01f),FQuat::Identity, ECC_WorldStatic,ShepereCol);

			DrawDebugSphere(GetWorld(), Hit.Location, ImpactRadius, 50, FColor::Orange, false, 2.0f);
			if(bSweepHit)
			{
				for(auto& HitImp:HitActors)
				{
					UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(HitImp.GetActor()->GetRootComponent());
					if(MeshComponent)
					{
						MeshComponent->AddRadialImpulse(HitImp.Location, ImpactRadius, RadialImpactForce, ERadialImpulseFalloff::RIF_Constant, true);
					}
				}
			}

		}
	}
	*/
}

void ASAIR_CharacterBase::Firing(float DeltaTime)
{
	LastShotTime -= DeltaTime;
	if (bIsFiring && LastShotTime<=0)
	{
		LastShotTime = 60.0f/RateOfFire;
		if(BulletClass)
		{
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CamShake,0.5f);

			FActorSpawnParameters spawnParams;
			spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			spawnParams.bNoFail = true;
			spawnParams.Owner = this;
			spawnParams.Instigator = this;
			FTransform bulletSpawnTransform;
			bulletSpawnTransform.SetLocation(GetActorForwardVector()*100.f+GetActorLocation());
			bulletSpawnTransform.SetRotation(GetActorRotation().Quaternion());
			bulletSpawnTransform.SetScale3D(FVector(1.f));
			GetWorld()->SpawnActor<ASAIR_Bullet>(BulletClass, bulletSpawnTransform, spawnParams);
		}
	}
}

void ASAIR_CharacterBase::SpawnObject(FVector loc, FRotator rot)
{
	FActorSpawnParameters ActorSpawnParameters;
	ASAIR_ForceActor* SpawnedActorRef = GetWorld()->SpawnActor<ASAIR_ForceActor>(ActorToSpawn, loc, rot, ActorSpawnParameters);
	// SpawnedActorRef->OnInteract(); //if you want to do something with it
}

void ASAIR_CharacterBase::Tick(float DeltaTime)
{ 
	TraceForward();
	Firing(DeltaTime);
	SquashTimeline.TickTimeline(DeltaTime);
}

// Called to bind functionality to input
void ASAIR_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	// PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ASAIR_CharacterBase::InteractPress);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASAIR_CharacterBase::FireForward);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASAIR_CharacterBase::FireForward);
	PlayerInputComponent->BindAction("Thrust", IE_Pressed, this, &ASAIR_CharacterBase::ThrustPress);
	PlayerInputComponent->BindAction("Thrust", IE_Released, this, &ASAIR_CharacterBase::ThrustPress);


	PlayerInputComponent->BindAxis("MoveForward", this, &ASAIR_CharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASAIR_CharacterBase::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &ASAIR_CharacterBase::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ASAIR_CharacterBase::LookUp);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASAIR_CharacterBase::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASAIR_CharacterBase::LookUpAtRate);
}

void ASAIR_CharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if(CurveFloat)
	{
		FOnTimelineFloatStatic TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("SquashProgress"));
		SquashTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
		SquashTimeline.SetLooping(false);
		SquashTimeline.PlayFromStart();
	}
}

void ASAIR_CharacterBase::SquashProgress(float Value)
{
	FVector NewScale =  FMath::Lerp(StartScale, TargetScale, Value);
	MeshCom->SetWorldScale3D(NewScale);
}
                                                                   