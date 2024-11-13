#include "Hunter_Base.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogHunterBase, Display, All);

AHunter_Base::AHunter_Base()
{
	SetReplicates(true);
	SetReplicateMovement(true);
	
	_Collision = GetCapsuleComponent();
	_CharacterMovement = GetCharacterMovement();
	_PlayerVelocity = _CharacterMovement->GetLastUpdateVelocity();
	
	_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_Camera->SetupAttachment(_Collision);
	_Camera->SetRelativeLocation(FVector(-10.0f, 0.0f, 60.0f));
	_Camera->bUsePawnControlRotation = true;
	
	_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
	_CharacterMovement->MaxWalkSpeedCrouched = _CrouchSpeed;
}


// Called when the game starts or when spawned
void AHunter_Base::BeginPlay()
{
	Super::BeginPlay();


	isAiming = false;
}

void AHunter_Base::IAMove_Implementation(const FInputActionInstance& Instance)
{
	if(HasAuthority())
	{
		if(Controller != nullptr)
		{
			const FVector2d MoveValue = Instance.GetValue().Get<FVector2d>();

			if(MoveValue.Y != 0.f)
			{
				AddMovementInput(GetActorForwardVector(), MoveValue.Y);
			}
			if(MoveValue.X != 0.f)
			{
				AddMovementInput(GetActorRightVector(), MoveValue.X);
			}
		}
	}
	else
	{
		S_Move_Implementation(Instance);
	}
}

void AHunter_Base::IALook_Implementation(const FInputActionInstance& Instance)
{
	if(HasAuthority())
	{
		if(Controller != nullptr)
		{
			const FVector2d AxisValue = Instance.GetValue().Get<FVector2d>();

			if(AxisValue.Y != 0.f)
			{
				if(isAiming)
				{
					AddControllerPitchInput(AxisValue.Y / _AimingSensitivity);
				}
				else
				{
					AddControllerPitchInput(AxisValue.Y);
				}
			}
			if(AxisValue.X != 0.f)
			{
				if(isAiming)
				{
					AddControllerYawInput(AxisValue.X / _AimingSensitivity);
				}
				else
				{
					AddControllerYawInput(AxisValue.X);
				}
			}
		}
	}
	else
	{
		S_Look_Implementation(Instance);
	}
}

void AHunter_Base::IAAction_Implementation(const FInputActionInstance& Instance)
{
	S_Action_Implementation(Instance);
}

void AHunter_Base::IASprint_Implementation(const FInputActionInstance& Instance)
{
	S_Sprint_Implementation(Instance);
}

void AHunter_Base::IAStopSprinting_Implementation(const FInputActionInstance& Instance)
{
	S_StopSprinting_Implementation(Instance);
}

void AHunter_Base::IACrouch_Implementation(const FInputActionInstance& Instance)
{
	S_Crouch_Implementation(Instance);
}

void AHunter_Base::IAStand_Implementation(const FInputActionInstance& Instance)
{
	S_Stand_Implementation(Instance);
}

void AHunter_Base::IAJump_Implementation(const FInputActionInstance& Instance)
{
	S_Jump_Implementation(Instance);
}

void AHunter_Base::IAShoot_Implementation(const FInputActionInstance& Instance)
{
	S_Shoot_Implementation(Instance);
}

void AHunter_Base::IAAim_Implementation(const FInputActionInstance& Instance)
{
	S_Aim_Implementation(Instance);
}

void AHunter_Base::IAStopAiming_Implementation(const FInputActionInstance& Instance)
{
	S_StopAiming_Implementation(Instance);
}



void AHunter_Base::S_Move_Implementation_Implementation(const FInputActionInstance& Instance)
{
	if(Controller != nullptr)
	{
		const FVector2d MoveValue = Instance.GetValue().Get<FVector2d>();

		if(MoveValue.Y != 0.f)
		{
			AddMovementInput(GetActorForwardVector(), MoveValue.Y);
		}
		if(MoveValue.X != 0.f)
		{
			AddMovementInput(GetActorRightVector(), MoveValue.X);
		}
	}
}

void AHunter_Base::S_Look_Implementation_Implementation(const FInputActionInstance& Instance)
{
	if(Controller != nullptr)
	{
		const FVector2d AxisValue = Instance.GetValue().Get<FVector2d>();

		if(AxisValue.Y != 0.f)
		{
			if(isAiming)
			{
				AddControllerPitchInput(AxisValue.Y / _AimingSensitivity);
			}
			else
			{
				AddControllerPitchInput(AxisValue.Y);
			}
		}
		if(AxisValue.X != 0.f)
		{
			if(isAiming)
			{
				AddControllerYawInput(AxisValue.X / _AimingSensitivity);
			}
			else
			{
				AddControllerYawInput(AxisValue.X);
			}
		}
	}
}

void AHunter_Base::S_Action_Implementation_Implementation(const FInputActionInstance& Instance)
{
	bool BoolValue = Instance.GetValue().Get<bool>();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("FIRE!"));
}

void AHunter_Base::S_Sprint_Implementation_Implementation(const FInputActionInstance& Instance)
{
	_CharacterMovement->MaxWalkSpeed = _SprintSpeed;
}

void AHunter_Base::S_StopSprinting_Implementation_Implementation(const FInputActionInstance& Instance)
{
	_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
}

void AHunter_Base::S_Crouch_Implementation_Implementation(const FInputActionInstance& Instance)
{
	Crouch();
}

void AHunter_Base::S_Stand_Implementation_Implementation(const FInputActionInstance& Instance)
{
	UnCrouch();
}

void AHunter_Base::S_Jump_Implementation_Implementation(const FInputActionInstance& Instance)
{
	Jump();
}

void AHunter_Base::S_Shoot_Implementation_Implementation(const FInputActionInstance& Instance)
{
	IIAInterface::IAShoot_Implementation(Instance);
}

void AHunter_Base::S_Aim_Implementation_Implementation(const FInputActionInstance& Instance)
{
	isAiming = true;
	_Camera->SetFieldOfView(30.0f);
	UE_LOG(LogHunterBase, Display, TEXT("Aiming"));
}

void AHunter_Base::S_StopAiming_Implementation_Implementation(const FInputActionInstance& Instance)
{
	isAiming = false;
	_Camera->SetFieldOfView(90.0f);
}
