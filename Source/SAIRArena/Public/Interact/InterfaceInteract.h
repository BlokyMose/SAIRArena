// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InterfaceInteract.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterfaceInteract : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SAIRARENA_API IInterfaceInteract
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	void OnInteract(AActor* Caller);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	void StartFocus();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	void EndFocus();

};
