#include "ThePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "AsymmetricalHuntGame/Survivors/Survivor_Craig/Survivor_Craig.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogSurvivorController, Display, All);

void AThePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(_Move, ETriggerEvent::Triggered, this, &AThePlayerController::MoveInput);
		EnhancedInputComponent->BindAction(_Look, ETriggerEvent::Triggered, this, &AThePlayerController::LookInput);
		EnhancedInputComponent->BindAction(_Action, ETriggerEvent::Triggered, this, &AThePlayerController::ActionInput);
		EnhancedInputComponent->BindAction(_Jump, ETriggerEvent::Triggered, this, &AThePlayerController::JumpInput);
		EnhancedInputComponent->BindAction(_Sprint, ETriggerEvent::Triggered, this, &AThePlayerController::SprintInput);
		EnhancedInputComponent->BindAction(_Sprint, ETriggerEvent::Completed, this, &AThePlayerController::StopSprintingInput);
		EnhancedInputComponent->BindAction(_Crouch, ETriggerEvent::Triggered, this, &AThePlayerController::CrouchInput);
		EnhancedInputComponent->BindAction(_Crouch, ETriggerEvent::Completed, this, &AThePlayerController::StandInput);
		EnhancedInputComponent->BindAction(_Aim, ETriggerEvent::Triggered, this, &AThePlayerController::AimInput);
		EnhancedInputComponent->BindAction(_Shoot, ETriggerEvent::Triggered, this, &AThePlayerController::ShootInput);

	}
}
void AThePlayerController::BeginPlay()
{
	Super::BeginPlay();

	PossessSurvivorCharacter();
}

void AThePlayerController::PossessSurvivorCharacter()
{
	this->UnPossess();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = GetInstigator();

	FVector SurvivorSpawnLocation(UKismetMathLibrary::RandomIntegerInRange(-2000,2000),UKismetMathLibrary::RandomIntegerInRange(-2000, 2000),5.0f);
	FRotator SurvivorSpawnRotation(0.0f, 0.0f, 0.0f); 

	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if(_InputMappingContext)
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(_InputMappingContext,0);
		}
	}
	_PlayerCharacter = GetWorld()->SpawnActor<ACharacter>(ThePlayerCharacter, SurvivorSpawnLocation, SurvivorSpawnRotation, SpawnParams);
	
	if(UGameplayStatics::GetPlayerController(this, 0))
	{
		this->Possess(_PlayerCharacter);
		UE_LOG(LogSurvivorController, Display, TEXT("Possessed Player"));
	}
	
}
/*
void AThePlayerController::PossessHunterCharacter()
{
	this->UnPossess();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = GetInstigator();

	FVector SurvivorSpawnLocation(UKismetMathLibrary::RandomIntegerInRange(-2000,2000),UKismetMathLibrary::RandomIntegerInRange(-2000, 2000),5.0f);
	FRotator SurvivorSpawnRotation(0.0f, 0.0f, 0.0f); 

	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if(_InputMappingContext)
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(_InputMappingContext,0);
		}
	}
	_PlayerCharacter = GetWorld()->SpawnActor<ACharacter>(ThePlayerCharacter, SurvivorSpawnLocation, SurvivorSpawnRotation, SpawnParams);
	
	if(UGameplayStatics::GetPlayerController(this, 0))
	{
		this->Possess(_PlayerCharacter);
		UE_LOG(LogSurvivorController, Display, TEXT("Possessed Player"));
	}
}

*/


void AThePlayerController::MoveInput(const FInputActionInstance& Instance)
{
	Execute_IAMove(_PlayerCharacter, Instance);
}

void AThePlayerController::LookInput(const FInputActionInstance& Instance)
{
	Execute_IALook(_PlayerCharacter, Instance);
}

void AThePlayerController::ActionInput(const FInputActionInstance& Instance)
{
	Execute_IAAction(_PlayerCharacter, Instance);
}

void AThePlayerController::SprintInput(const FInputActionInstance& Instance)
{
	Execute_IASprint(_PlayerCharacter, Instance);
}

void AThePlayerController::StopSprintingInput(const FInputActionInstance& Instance)
{
	Execute_IAStopSprinting(_PlayerCharacter, Instance);
}

void AThePlayerController::CrouchInput(const FInputActionInstance& Instance)
{
	Execute_IACrouch(_PlayerCharacter, Instance);
}

void AThePlayerController::StandInput(const FInputActionInstance& Instance)
{
	Execute_IAStand(_PlayerCharacter, Instance);
}

void AThePlayerController::JumpInput(const FInputActionInstance& Instance)
{
	Execute_IAJump(_PlayerCharacter, Instance);
}

void AThePlayerController::ShootInput(const FInputActionInstance& Instance)
{
	Execute_IAShoot(_PlayerCharacter, Instance);
}

void AThePlayerController::AimInput(const FInputActionInstance& Instance)
{
	Execute_IAAim(_PlayerCharacter, Instance);
}

