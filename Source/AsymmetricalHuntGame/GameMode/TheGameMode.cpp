// Fill out your copyright notice in the Description page of Project Settings.

#include "TheGameMode.h"

#include "AsymmetricalHuntGame/Controller/ThePlayerController.h"
#include "AsymmetricalHuntGame/GameMode/GameState/TheGameState.h"
#include "AsymmetricalHuntGame/Controller/PlayerState/ThePlayerState.h"
#include "GameFramework/Character.h"
#include "AsymmetricalHuntGame/Hunters/Hunter_Ghost/Hunter_Ghost.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogTheGameMode, Display, All);

ATheGameMode::ATheGameMode()
{
	PlayerControllerClass = AThePlayerController::StaticClass();
	GameStateClass = ATheGameState::StaticClass();
	PlayerStateClass = AThePlayerState::StaticClass();

	APlayerController* _ControllerRef = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	_PlayerController = Cast<AThePlayerController>(_ControllerRef);

	_PlayerCharacter = Cast<ACharacter>(GetClass());
}

void ATheGameMode::BeginPlay()
{
	if(_PlayerController)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Controller Ref Good!"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Controller Ref Bad!"));
	}
	SpawnCharacters_Implementation();

	ATheGameState* _GameState = GetWorld()->GetGameState<ATheGameState>();
	if (_GameState)
	{
		int _NumberOfPlayers = _GameState->PlayerArray.Num();
		UE_LOG(LogTemp, Log, TEXT("Number of players in the game: %d"), _NumberOfPlayers);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Can't get number of players!"));
	}
}


void ATheGameMode::SpawnCharacters_Implementation()
{
	if(HasAuthority())
	{
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* _ControllerRef = Iterator->Get();
			if (_PlayerController && _PlayerController->IsLocalController())
			{
				// Set spawn location and rotation (you can customize this for each player)
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				SpawnParams.Owner = GetOwner();
				SpawnParams.Instigator = GetInstigator();

				FVector HunterSpawnLocation(UKismetMathLibrary::RandomIntegerInRange(-2000,2000),UKismetMathLibrary::RandomIntegerInRange(-2000, 2000),5.0f);
				FRotator HunterSpawnRotation(0.0f, 0.0f, 0.0f); 

				// Spawn the character on the server
				_PlayerCharacter = GetWorld()->SpawnActor<ACharacter>(TheHunterCharacter, HunterSpawnLocation, HunterSpawnRotation, SpawnParams);
				
				if(_PlayerCharacter)
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Character Spawned!"));
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Character Not Spawned!"));
				}
				
				if (_PlayerCharacter)
				{
					// Ensure the character is set to replicate
					_PlayerCharacter->SetReplicates(true);
                
					// Have the PlayerController possess the character

					if(_PlayerController != nullptr)
					{
						_PlayerController->UnPossess();
						_PlayerController->Possess(_PlayerCharacter);
						GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Possessed Server!"));
					}
					else if (_PlayerController == nullptr)
					{
						GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Not Possessed!"));
					}
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Not Local!"));
			}
		}
	}
}

/*
void ATheGameMode::PossessSurvivorCharacter_Implementation()
{
	if(HasAuthority())
	{
		UGameplayStatics::GetPlayerController(this, 0)->UnPossess();
	
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = GetInstigator();

		FVector SurvivorSpawnLocation(UKismetMathLibrary::RandomIntegerInRange(-2000,2000),UKismetMathLibrary::RandomIntegerInRange(-2000, 2000),5.0f);
		FRotator SurvivorSpawnRotation(0.0f, 0.0f, 0.0f); 

		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(_SurvivorMappingContext,0);
		}
		_PlayerCharacter = GetWorld()->SpawnActor<ACharacter>(TheSurvivorCharacter, SurvivorSpawnLocation, SurvivorSpawnRotation, SpawnParams);
	
		if(UGameplayStatics::GetPlayerController(this, 0))
		{
			UGameplayStatics::GetPlayerController(this, 0)->Possess(_PlayerCharacter);
		}
	}
}



void ATheGameMode::SpawnCharacters()
{
	PossessHunterCharacter_Implementation();
	//PossessSurvivorCharacter_Implementation();
	UE_LOG(LogTheGameMode, Display, TEXT("TheGameMode Working"));
}

*/
