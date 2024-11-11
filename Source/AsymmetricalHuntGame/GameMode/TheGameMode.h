// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TheGameMode.generated.h"


UCLASS(Abstract)
class ASYMMETRICALHUNTGAME_API ATheGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	ATheGameMode();
	
protected:

	//Character Subclass
	UPROPERTY()
	TObjectPtr<ACharacter> _PlayerCharacter;

	//Hunter Subclasses
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AHunter_Ghost> TheHunterCharacter;
	
	void SpawnCharacters();

	UFUNCTION(NetMulticast, Reliable)
	void PossessHunterCharacter();
	UFUNCTION(NetMulticast, Reliable)
	void PossessSurvivorCharacter();

};


