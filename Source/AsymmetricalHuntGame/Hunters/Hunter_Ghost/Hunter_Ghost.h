// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AsymmetricalHuntGame/Hunters/HunterBase/Hunter_Base.h"
#include "Hunter_Ghost.generated.h"

UCLASS()
class ASYMMETRICALHUNTGAME_API AHunter_Ghost : public AHunter_Base
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHunter_Ghost();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};

