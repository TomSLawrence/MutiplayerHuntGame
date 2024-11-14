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
}

void ATheGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	AThePlayerController* _NewPlayerController = Cast<AThePlayerController>(NewPlayer);
	_ConnectedPlayers.Add(_NewPlayerController);
	
}

void ATheGameMode::BeginPlay()
{
	
}


void ATheGameMode::GM_SpawnCharacters_Implementation(AThePlayerController* _PlayerController)
{

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = GetInstigator();

	FVector HunterSpawnLocation(UKismetMathLibrary::RandomIntegerInRange(-2000,2000),UKismetMathLibrary::RandomIntegerInRange(-2000, 2000),5.0f);
	FRotator HunterSpawnRotation(0.0f, 0.0f, 0.0f); 
	if(IsValid(TheHunterCharacter))
	{
		ACharacter* _PlayerCharacter = GetWorld()->SpawnActor<ACharacter>(TheHunterCharacter, HunterSpawnLocation, HunterSpawnRotation, SpawnParams);

		if(_PlayerController != nullptr)
		{
			_PlayerController->UnPossess();
			_PlayerController->Possess(_PlayerCharacter);
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("Possessed Character!"));
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
