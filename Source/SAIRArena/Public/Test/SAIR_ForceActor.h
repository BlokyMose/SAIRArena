// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SAIR_ForceActor.generated.h"

UCLASS()
class SAIRARENA_API ASAIR_ForceActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASAIR_ForceActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Force Cube")
	UStaticMeshComponent* MeshCom;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	float ForceStrength;
};
