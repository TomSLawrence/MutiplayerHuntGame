// Fill out your copyright notice in the Description page of Project Settings.

#include "TheGameMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogTheGameMode, Display, All);

void ATheGameMode::BeginPlay()
{
	SpawnCharacters();
}

void ATheGameMode::SpawnCharacters()
{
	OnSurvivorSpawn.Broadcast();
	OnHunterSpawn.Broadcast();
	UE_LOG(LogTheGameMode, Display, TEXT("TheGameMode Working"));
}
