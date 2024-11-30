// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TheGameMode.generated.h"

class AMyBush;
class AMyTree;
class ASurvivor_Craig;
class ACharacter;
class AHunter_Ghost;
class AThePlayerController;

UCLASS(Abstract)
class ASYMMETRICALHUNTGAME_API ATheGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	ATheGameMode();
	
	UFUNCTION(Server, Reliable)
	void GM_SpawnCharacters(AThePlayerController* _PlayerControllerRef);
	UFUNCTION(Server, Reliable)
	void S_SpawnMap();
	UFUNCTION(NetMulticast, Reliable)
	void Multi_SpawnMap();

	
	UPROPERTY(BlueprintReadOnly)
	TArray<AThePlayerController*> _ConnectedPlayers;
protected:

	//Hunter Subclasses
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AHunter_Ghost> _TheHunterCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASurvivor_Craig> _TheSurvivorCharacter;
	
	virtual void OnPostLogin(AController* NewPlayer) override;

};


