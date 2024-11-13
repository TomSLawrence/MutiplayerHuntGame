// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TheGameMode.generated.h"

class ACharacter;
class AHunter_Ghost;
class AThePlayerController;

UCLASS(Abstract)
class ASYMMETRICALHUNTGAME_API ATheGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	ATheGameMode();
	
	UFUNCTION()
	void GM_SpawnCharacters(AThePlayerController* _PlayerControllerRef);
	
	UPROPERTY()
	TObjectPtr<ACharacter> _PlayerCharacter;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<AController*> _ConnectedPlayers;
	
protected:

	//Hunter Subclasses
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AHunter_Ghost> TheHunterCharacter;




	
	virtual void OnPostLogin(AController* NewPlayer) override;

};


