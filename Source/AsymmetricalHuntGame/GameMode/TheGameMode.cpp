// Fill out your copyright notice in the Description page of Project Settings.

#include "TheGameMode.h"

#include "AsymmetricalHuntGame/Controller/ThePlayerController.h"
#include "AsymmetricalHuntGame/GameMode/GameState/TheGameState.h"
#include "AsymmetricalHuntGame/Controller/PlayerState/ThePlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogTheGameMode, Display, All);

void ATheGameMode::BeginPlay()
{
	if(HasAuthority())
	{
		S_SpawnCharacters();
	}
}

ATheGameMode::ATheGameMode()
{
	PlayerControllerClass = AThePlayerController::StaticClass();
	GameStateClass = ATheGameState::StaticClass();
	PlayerStateClass = AThePlayerState::StaticClass();
}

void ATheGameMode::S_SpawnCharacters_Implementation()
{
	OnSurvivorSpawn.Broadcast();
	OnHunterSpawn.Broadcast();
	UE_LOG(LogTheGameMode, Display, TEXT("TheGameMode Working"));
}

