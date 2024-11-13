#include "ThePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "AsymmetricalHuntGame/Survivors/Survivor_Craig/Survivor_Craig.h"
#include "AsymmetricalHuntGame/GameMode/TheGameMode.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogThePlayerController, Display, All);

void AThePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
		
	if(IsLocalController())
	{
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
			EnhancedInputComponent->BindAction(_Aim, ETriggerEvent::Completed, this, &AThePlayerController::StopAiming);
			EnhancedInputComponent->BindAction(_Shoot, ETriggerEvent::Triggered, this, &AThePlayerController::ShootInput);
		}
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Set up input!"));
	}
}

void AThePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		UE_LOG(LogTemp, Warning, TEXT("Local player controller possessed character"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Non-local (server) player controller"));
	}
}

void AThePlayerController::PC_SpawnCharacters(ATheGameMode* _GameModeRef)
{
	if(_GameModeRef != nullptr)
	{
		_GameModeRef->GM_SpawnCharacters(this);
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(_HunterMappingContext,0);
		}
	}
}

void AThePlayerController::MoveInput(const FInputActionInstance& Instance)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Moving Client!"));
	if(GetCharacter()->IsLocallyControlled())
	{
		Execute_IAMove(GetCharacter(), Instance);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Nullptr"));
	}
}

void AThePlayerController::LookInput(const FInputActionInstance& Instance)
{
	if(GetCharacter()->IsLocallyControlled())
	{
		Execute_IALook(GetCharacter(), Instance);
	}
}

void AThePlayerController::ActionInput(const FInputActionInstance& Instance)
{
	if(GetCharacter())
	{
		Execute_IAAction(GetCharacter(), Instance);
	}
}

void AThePlayerController::SprintInput(const FInputActionInstance& Instance)
{
	if(GetCharacter())
	{
		Execute_IASprint(GetCharacter(), Instance);
	}
}

void AThePlayerController::StopSprintingInput(const FInputActionInstance& Instance)
{
	if(GetCharacter())
	{
		Execute_IAStopSprinting(GetCharacter(), Instance);
	}
}

void AThePlayerController::CrouchInput(const FInputActionInstance& Instance)
{
	if(GetCharacter())
	{
		Execute_IACrouch(GetCharacter(), Instance);
	}
}

void AThePlayerController::StandInput(const FInputActionInstance& Instance)
{
	if(GetCharacter())
	{
		Execute_IAStand(GetCharacter(), Instance);
	}
}

void AThePlayerController::JumpInput(const FInputActionInstance& Instance)
{
	if(GetCharacter())
	{
		Execute_IAJump(GetCharacter(), Instance);
	}
}

void AThePlayerController::ShootInput(const FInputActionInstance& Instance)
{
	if(GetCharacter())
	{
		Execute_IAShoot(GetCharacter(), Instance);
	}
}

void AThePlayerController::AimInput(const FInputActionInstance& Instance)
{
	if(GetCharacter())
	{
		Execute_IAAim(GetCharacter(), Instance);
	}
}

void AThePlayerController::StopAiming(const FInputActionInstance& Instance)
{
	if(GetCharacter())
	{
		Execute_IAStopAiming(GetCharacter(), Instance);
	}
}


