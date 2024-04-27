// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TheGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpawnSurvivorCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpawnHunterCharacter);

UCLASS(Abstract)
class ASYMMETRICALHUNTGAME_API ATheGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY()
	FSpawnHunterCharacter OnHunterSpawn;
	UPROPERTY()
	FSpawnSurvivorCharacter OnSurvivorSpawn;
	
protected:
	UFUNCTION(NetMulticast, Reliable)
	void SpawnCharacters();

};


