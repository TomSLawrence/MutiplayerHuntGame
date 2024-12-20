﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "UObject/Interface.h"
#include "IAInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UIAInterface : public UInterface
{
	GENERATED_BODY()
};

class ASYMMETRICALHUNTGAME_API IIAInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IAAction(const FInputActionInstance& Instance);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IAStopAction(const FInputActionInstance& Instance);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IASprint(const FInputActionInstance& Instance);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IAStopSprinting(const FInputActionInstance& Instance);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IACrouch(const FInputActionInstance& Instance);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IAStand(const FInputActionInstance& Instance);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IAJump(const FInputActionInstance& Instance);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IAMelee(const FInputActionInstance& Instance);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IAStopMelee(const FInputActionInstance& Instance);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IABlock(const FInputActionInstance& Instance);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IAStopBlocking(const FInputActionInstance& Instance);

	UFUNCTION(BlueprintNativeEvent,  BlueprintCallable)
	void IAInteract(const FInputActionInstance& Instance);
};
