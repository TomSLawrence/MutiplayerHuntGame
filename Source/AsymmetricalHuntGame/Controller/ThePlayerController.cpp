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
}

void AThePlayerController::BeginPlay()
{
	Super::BeginPlay();
	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(_HunterMappingContext,0);
	}
}

void AThePlayerController::PC_SpawnCharacters(ATheGameMode* _GameModeRef)
{
	if(_GameModeRef != nullptr)
	{
		_GameModeRef->GM_SpawnCharacters(this);
	}
}


//Local Player Controller Inputs

void AThePlayerController::MoveInput(const FInputActionInstance& Instance)
{
	const FVector2d MoveValue = Instance.GetValue().Get<FVector2d>();
	_MoveX = MoveValue.X;
	_MoveY = MoveValue.Y;
	
	if(HasAuthority())
	{
		Execute_IAMove(GetCharacter(), Instance);
	}
	else
	{
		S_MoveInput(_MoveX, _MoveY);
	}
}

void AThePlayerController::LookInput(const FInputActionInstance& Instance)
{
	const FVector2d LookValue = Instance.GetValue().Get<FVector2d>();
	_LookX = LookValue.X;
	_LookY = LookValue.Y;
	
	if(HasAuthority())
	{
		Execute_IALook(GetCharacter(), Instance);
	}
	else
	{
		S_LookInput(_LookX, _LookY);
	}
}

void AThePlayerController::ActionInput(const FInputActionInstance& Instance)
{
	if(HasAuthority())
	{
		Execute_IAAction(GetCharacter(), Instance);
	}
	else
	{
		S_ActionInput(Instance);
	}
}

void AThePlayerController::SprintInput(const FInputActionInstance& Instance)
{
	if(HasAuthority())
	{
		Execute_IASprint(GetCharacter(), Instance);
	}
	else
	{
		S_SprintInput(Instance);
	}
}

void AThePlayerController::StopSprintingInput(const FInputActionInstance& Instance)
{
	if(HasAuthority())
	{
		Execute_IAStopSprinting(GetCharacter(), Instance);
	}
	else
	{
		S_StopSprintingInput(Instance);
	}
}

void AThePlayerController::CrouchInput(const FInputActionInstance& Instance)
{
	if(HasAuthority())
	{
		Execute_IACrouch(GetCharacter(), Instance);
	}
	else
	{
		S_CrouchInput(Instance);
	}
}

void AThePlayerController::StandInput(const FInputActionInstance& Instance)
{
	if(HasAuthority())
	{
		Execute_IAStand(GetCharacter(), Instance);
	}
	else
	{
		S_StandInput(Instance);
	}
}

void AThePlayerController::JumpInput(const FInputActionInstance& Instance)
{
	if(HasAuthority())
	{
		Execute_IAJump(GetCharacter(), Instance);
	}
	else
	{
		S_JumpInput(Instance);
	}
}

void AThePlayerController::ShootInput(const FInputActionInstance& Instance)
{
	if(HasAuthority())
	{
		Execute_IAShoot(GetCharacter(), Instance);
	}
	else
	{
		S_ShootInput(Instance);
	}
}

void AThePlayerController::AimInput(const FInputActionInstance& Instance)
{
	if(HasAuthority())
	{
		Execute_IAAim(GetCharacter(), Instance);
	}
	else
	{
		S_AimInput(Instance);
	}
}

void AThePlayerController::StopAiming(const FInputActionInstance& Instance)
{
	if(HasAuthority())
	{
		Execute_IAStopAiming(GetCharacter(), Instance);
	}
	else
	{
		S_StopAiming(Instance);
	}
}






//Server Functions

void AThePlayerController::S_MoveInput_Implementation(float _MoveX, float _MoveY)
{
	Execute_IAMove(GetCharacter(), _MoveX, _MoveY);
}

void AThePlayerController::S_LookInput_Implementation(float _LookX, float _LookY)
{
	Execute_IALook(GetCharacter(), _LookX, _LookY);
}

void AThePlayerController::S_ActionInput_Implementation(const FInputActionInstance& Instance)
{
	Execute_IAAction(GetCharacter(), Instance);
}

void AThePlayerController::S_SprintInput_Implementation(const FInputActionInstance& Instance)
{
	Execute_IASprint(GetCharacter(), Instance);
}

void AThePlayerController::S_StopSprintingInput_Implementation(const FInputActionInstance& Instance)
{
	Execute_IAStopSprinting(GetCharacter(), Instance);
}

void AThePlayerController::S_CrouchInput_Implementation(const FInputActionInstance& Instance)
{
	Execute_IACrouch(GetCharacter(), Instance);
}

void AThePlayerController::S_StandInput_Implementation(const FInputActionInstance& Instance)
{
	Execute_IAStand(GetCharacter(), Instance);
}

void AThePlayerController::S_JumpInput_Implementation(const FInputActionInstance& Instance)
{
	Execute_IAJump(GetCharacter(), Instance);
}

void AThePlayerController::S_ShootInput_Implementation(const FInputActionInstance& Instance)
{
	Execute_IAShoot(GetCharacter(), Instance);
}

void AThePlayerController::S_AimInput_Implementation(const FInputActionInstance& Instance)
{
	Execute_IAAim(GetCharacter(), Instance);
}


void AThePlayerController::S_StopAiming_Implementation(const FInputActionInstance& Instance)
{
	Execute_IAStopAiming(GetCharacter(), Instance);
}



//Server Validation

bool AThePlayerController::S_MoveInput_Validate(float _MoveX, float _MoveY)
{
	return true;
}

bool AThePlayerController::S_LookInput_Validate(const FInputActionInstance& Instance)
{
	return true;
}

bool AThePlayerController::S_ActionInput_Validate(const FInputActionInstance& Instance)
{
	return true;
}

bool AThePlayerController::S_SprintInput_Validate(const FInputActionInstance& Instance)
{
	return true;
}

bool AThePlayerController::S_StopSprintingInput_Validate(const FInputActionInstance& Instance)
{
	return true;
}

bool AThePlayerController::S_CrouchInput_Validate(const FInputActionInstance& Instance)
{
	return true;
}

bool AThePlayerController::S_StandInput_Validate(const FInputActionInstance& Instance)
{
	return true;
}

bool AThePlayerController::S_JumpInput_Validate(const FInputActionInstance& Instance)
{
	return true;
}

bool AThePlayerController::S_ShootInput_Validate(const FInputActionInstance& Instance)
{
	return true;
}

bool AThePlayerController::S_AimInput_Validate(const FInputActionInstance& Instance)
{
	return true;
}

bool AThePlayerController::S_StopAiming_Validate(const FInputActionInstance& Instance)
{
	return true;
}
