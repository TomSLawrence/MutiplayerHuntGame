// Fill out your copyright notice in the Description page of Project Settings.

#include "TheGameMode.h"

#include "AsymmetricalHuntGame/Controller/ThePlayerController.h"
#include "AsymmetricalHuntGame/GameMode/GameState/TheGameState.h"
#include "AsymmetricalHuntGame/Controller/PlayerState/ThePlayerState.h"
#include "GameFramework/Character.h"
#include "AsymmetricalHuntGame/Hunters/Hunter_Ghost/Hunter_Ghost.h"
#include "AsymmetricalHuntGame/Survivors/Survivor_Craig/Survivor_Craig.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogTheGameMode, Display, All);

ATheGameMode::ATheGameMode()
{
	PlayerControllerClass = AThePlayerController::StaticClass();
	GameStateClass = ATheGameState::StaticClass();
	PlayerStateClass = AThePlayerState::StaticClass();
	
}

void ATheGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	AThePlayerController* _NewPlayerController = Cast<AThePlayerController>(NewPlayer);
	_ConnectedPlayers.Add(_NewPlayerController);
	
}

void ATheGameMode::BeginPlay()
{
	S_SpawnMap(); 
}

void ATheGameMode::S_SpawnMap_Implementation()
{
	Multi_SpawnMap();
}

void ATheGameMode::Multi_SpawnMap_Implementation()
{
	//Spawn Map
}



void ATheGameMode::GM_SpawnCharacters_Implementation(AThePlayerController* _PlayerController)
{

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = GetInstigator();

	FVector CharacterSpawnLocation(UKismetMathLibrary::RandomIntegerInRange(-2000,2000),UKismetMathLibrary::RandomIntegerInRange(-5000, 5000),5);
	FRotator CharacterSpawnRotation(0.0f, 0.0f, 0.0f); 

	if(IsValid(_TheHunterCharacter) && IsValid(_TheSurvivorCharacter))
	{
		if(_PlayerController != nullptr)
		{
			if(_PlayerController->HasAuthority())
				
				if(_PlayerController->isHunter && !_PlayerController->isSurvivor)
				{
					ACharacter* _PlayerCharacter = GetWorld()->SpawnActor<ACharacter>(_TheHunterCharacter, CharacterSpawnLocation, CharacterSpawnRotation, SpawnParams);
					_PlayerController->UnPossess();
					_PlayerController->Possess(_PlayerCharacter);
				}
				else if(_PlayerController->isSurvivor && !_PlayerController->isHunter)
				{
					ACharacter* _PlayerCharacter = GetWorld()->SpawnActor<ACharacter>(_TheSurvivorCharacter, CharacterSpawnLocation, CharacterSpawnRotation, SpawnParams);
					_PlayerController->UnPossess();
					_PlayerController->Possess(_PlayerCharacter);
				}
		}
	}
}
