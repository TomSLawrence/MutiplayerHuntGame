﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AsymmetricalHuntGame/Hunters/HunterBase/Hunter_Base.h"
#include "Hunter_Ghost.generated.h"

class AProjectile_Ghost;

UCLASS(Abstract)
class ASYMMETRICALHUNTGAME_API AHunter_Ghost : public AHunter_Base
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHunter_Ghost();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AProjectile_Ghost> _Projectile;

public:

	//Player Movement/Looking
	virtual void IACharacterMove(FVector _InputAxis) override;
	virtual void IACharacterLook(FVector _InputAxis) override;
	virtual void IAAction_Implementation(const FInputActionInstance& Instance) override;
	virtual void IASprint_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAStopSprinting_Implementation(const FInputActionInstance& Instance) override;
	virtual void IACrouch_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAStand_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAJump_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAShoot_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAAim_Implementation(const FInputActionInstance& Instance) override;

};

