#include "ThePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "AsymmetricalHuntGame/GameMode/TheGameMode.h"
#include "AsymmetricalHuntGame/Hunters/HunterBase/Hunter_Base.h"
#include "AsymmetricalHuntGame/Survivors/SurvivorBase/Survivor_Base.h"

DEFINE_LOG_CATEGORY_STATIC(LogThePlayerController, Display, All);


void AThePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
		
	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(_Move, ETriggerEvent::Triggered, this, &AThePlayerController::MoveInput);
		EnhancedInputComponent->BindAction(_Look, ETriggerEvent::Triggered, this, &AThePlayerController::LookInput);
		EnhancedInputComponent->BindAction(_Action, ETriggerEvent::Triggered, this, &AThePlayerController::ActionInput);
		EnhancedInputComponent->BindAction(_Action, ETriggerEvent::Completed, this, &AThePlayerController::StopActionInput);
		EnhancedInputComponent->BindAction(_Jump, ETriggerEvent::Triggered, this, &AThePlayerController::JumpInput);
		EnhancedInputComponent->BindAction(_Sprint, ETriggerEvent::Triggered, this, &AThePlayerController::SprintInput);
		//EnhancedInputComponent->BindAction(_Sprint, ETriggerEvent::Completed, this, &AThePlayerController::StopSprintingInput);
		EnhancedInputComponent->BindAction(_Crouch, ETriggerEvent::Triggered, this, &AThePlayerController::CrouchInput);
		EnhancedInputComponent->BindAction(_Crouch, ETriggerEvent::Completed, this, &AThePlayerController::StandInput);
		EnhancedInputComponent->BindAction(_Aim, ETriggerEvent::Triggered, this, &AThePlayerController::AimInput);
		EnhancedInputComponent->BindAction(_Aim, ETriggerEvent::Completed, this, &AThePlayerController::StopAiming);
		EnhancedInputComponent->BindAction(_Shoot, ETriggerEvent::Triggered, this, &AThePlayerController::ShootInput);
		EnhancedInputComponent->BindAction(_Interact, ETriggerEvent::Triggered, this, &AThePlayerController::Interact);
	}
}

void AThePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AThePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(this->IsLocalController() && HasAuthority())
	{
		isHunter = true;
		isSurvivor = false;
		
	}
	else if(this->IsLocalController() && !HasAuthority())
	{
		isSurvivor = true;
		isHunter = false;
	}
	
	SpawnCharacters();
	
	if(isHunter)
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(_HunterMappingContext,0);
		}
	}
	if(isSurvivor)
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(_SurvivorMappingContext,0);
			
		}
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
	if(this->IsLocalController() && HasAuthority())
	{
		AHunter_Base* _PossessedCharacter = Cast<AHunter_Base>(GetCharacter());

		if(_PossessedCharacter != nullptr)
		{
			const FVector MoveValue = Instance.GetValue().Get<FVector>();
			_PossessedCharacter->IACharacterMove(MoveValue);
		}
	}
	else
	{
		const FVector MoveValue = Instance.GetValue().Get<FVector>();
		S_MoveInput(MoveValue);
	}
}

void AThePlayerController::LookInput(const FInputActionInstance& Instance)
{
	if(this->IsLocalController() && HasAuthority())
	{
		AHunter_Base* _PossessedCharacter = Cast<AHunter_Base>(GetCharacter());

		if(_PossessedCharacter != nullptr)
		{
			const FVector LookValue = Instance.GetValue().Get<FVector>();
			_PossessedCharacter->IACharacterLook(LookValue);
		}
	}
	else
	{
		const FVector LookValue = Instance.GetValue().Get<FVector>();
		S_LookInput(LookValue);
	}
}

void AThePlayerController::ActionInput(const FInputActionInstance& Instance)
{
	if(this->IsLocalController() && HasAuthority())
	{
		Execute_IAAction(GetCharacter(), Instance);
	}
	else
	{
		S_ActionInput(Instance);
	}
}

void AThePlayerController::StopActionInput(const FInputActionInstance& Instance)
{
	if(this->IsLocalController() && HasAuthority())
	{
		Execute_IAStopAction(GetCharacter(), Instance);
	}
	else
	{
		S_StopActionInput(Instance);
	}
}

void AThePlayerController::SprintInput(const FInputActionInstance& Instance)
{
	if(this->IsLocalController() && HasAuthority())
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
	if(this->IsLocalController() && HasAuthority())
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
	if(this->IsLocalController() && HasAuthority())
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
	if(this->IsLocalController() && HasAuthority())
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
	if(this->IsLocalController() && HasAuthority())
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
	if(this->IsLocalController() && HasAuthority())
	{
		Execute_IAMelee(GetCharacter(), Instance);
	}
	else
	{
		S_ShootInput(Instance);
	}
}

void AThePlayerController::AimInput(const FInputActionInstance& Instance)
{
	if(this->IsLocalController() && HasAuthority())
	{
		Execute_IABlock(GetCharacter(), Instance);
	}
	else
	{
		S_AimInput(Instance);
	}
}

void AThePlayerController::StopAiming(const FInputActionInstance& Instance)
{
	if(this->IsLocalController() && HasAuthority())
	{
		Execute_IAStopBlocking(GetCharacter(), Instance);
	}
	else
	{
		S_StopAiming(Instance);
	}
}

void AThePlayerController::Interact(const FInputActionInstance& Instance)
{
	if(this->IsLocalController() && HasAuthority())
	{
		Execute_IAInteract(GetCharacter(), Instance);
	}
	else
	{
		S_Interact(Instance);
	}
}


//Server Functions

void AThePlayerController::S_MoveInput_Implementation(const FVector _PlayerInput)
{
	ASurvivor_Base* _PossessedCharacter = Cast<ASurvivor_Base>(GetCharacter());

	if(_PossessedCharacter != nullptr)
	{
		_PossessedCharacter->IACharacterMove(_PlayerInput);
	}
	
}

void AThePlayerController::S_LookInput_Implementation(const FVector _PlayerInput)
{
	ASurvivor_Base* _PossessedCharacter = Cast<ASurvivor_Base>(GetCharacter());

	if(_PossessedCharacter != nullptr)
	{
		_PossessedCharacter->IACharacterLook(_PlayerInput);
	}
}

void AThePlayerController::S_ActionInput_Implementation(const FInputActionInstance& Instance)
{
	Execute_IAAction(GetCharacter(), Instance);
}

void AThePlayerController::S_StopActionInput_Implementation(const FInputActionInstance& Instance)
{
	Execute_IAStopAction(GetCharacter(), Instance);
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
	Execute_IAMelee(GetCharacter(), Instance);
}

void AThePlayerController::S_AimInput_Implementation(const FInputActionInstance& Instance)
{
	Execute_IABlock(GetCharacter(), Instance);
}

void AThePlayerController::S_StopAiming_Implementation(const FInputActionInstance& Instance)
{
	Execute_IAStopBlocking(GetCharacter(), Instance);
}

void AThePlayerController::S_Interact_Implementation(const FInputActionInstance& Instance)
{
	Execute_IAInteract(GetCharacter(), Instance);
}


//Server Validation

bool AThePlayerController::S_MoveInput_Validate(const FVector _PlayerInput)
{
	return true;
}

bool AThePlayerController::S_LookInput_Validate(const FVector _PlayerInput)
{
	return true;
}

bool AThePlayerController::S_ActionInput_Validate(const FInputActionInstance& Instance)
{
	return true;
}

bool AThePlayerController::S_StopActionInput_Validate(const FInputActionInstance& Instance)
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
bool AThePlayerController::S_Interact_Validate(const FInputActionInstance& Instance)
{
	return true;
}

