// Fill out your copyright notice in the Description page of Project Settings.

#include "Test/SAIR_ForceActor.h"

// Sets default values
ASAIR_ForceActor::ASAIR_ForceActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshCom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ForceMesh"));
	MeshCom -> SetupAttachment(RootComponent);
	ForceStrength = 10000.0f;
}

// Called when the game starts or when spawned
void ASAIR_ForceActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASAIR_ForceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Up = MeshCom->GetUpVector();
	MeshCom->AddForce(Up*ForceStrength*MeshCom->GetMass());
}

