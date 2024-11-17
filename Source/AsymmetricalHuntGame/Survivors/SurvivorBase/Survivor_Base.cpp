#include "Survivor_Base.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogSurvivorBase, Display, All);

// Sets default values
ASurvivor_Base::ASurvivor_Base()
{
	_Collision = GetCapsuleComponent();
	_CharacterMovement = GetCharacterMovement();
	_PlayerVelocity = _CharacterMovement->GetLastUpdateVelocity();

	_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	_Mesh->SetupAttachment(_Collision);
	
	_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_Camera->SetupAttachment(_Collision);
	_Camera->SetRelativeLocation(FVector(-10.0f, 0.0f, 60.0f));
	_Camera->bUsePawnControlRotation = true;
	
	_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
	_CharacterMovement->MaxWalkSpeedCrouched = _CrouchSpeed;

	SetReplicates(true);
	SetReplicateMovement(true);
	_CharacterMovement->SetIsReplicated(true);

	_survivorHealth = 2;

}

// Called when the game starts or when spawned
void ASurvivor_Base::BeginPlay()
{
	Super::BeginPlay();
	
	_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
	_StandScale = FVector(2.0f, 2.0f, 2.0f);
	_CrouchScale = FVector(1.5f, 1.5f, 1.5f);

}

//Movement Functions

void ASurvivor_Base::IACharacterMove_Implementation(FVector _InputAxis)
{
	if(Controller != nullptr)
	{
		if(_InputAxis.Y != 0.0f)
		{
			AddMovementInput(GetActorForwardVector(), _InputAxis.Y);
		}
		if(_InputAxis.X != 0.0f)
		{
			AddMovementInput(GetActorRightVector(), _InputAxis.X);
		}
	}
}

void ASurvivor_Base::IACharacterLook_Implementation(FVector _InputAxis)
{
	if(Controller != nullptr)
	{
		if(_InputAxis.Y != 0.0f)
		{
			AddControllerPitchInput(_InputAxis.Y);
		}
		if(_InputAxis.X != 0.0f)
		{
			AddControllerYawInput(_InputAxis.X);
		}
	}
}

void ASurvivor_Base::IAAction_Implementation_Implementation(const FInputActionInstance& Instance)
{
	bool BoolValue = Instance.GetValue().Get<bool>();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("FIRE!"));
}

void ASurvivor_Base::IASprint_Implementation_Implementation(const FInputActionInstance& Instance)
{
	_CharacterMovement->MaxWalkSpeed = _SprintSpeed;
}

void ASurvivor_Base::IAStopSprinting_Implementation_Implementation(const FInputActionInstance& Instance)
{
	_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
}

void ASurvivor_Base::IACrouch_Implementation_Implementation(const FInputActionInstance& Instance)
{
	Crouch();
	_Mesh->SetWorldScale3D(_CrouchScale);
}

void ASurvivor_Base::IAStand_Implementation_Implementation(const FInputActionInstance& Instance)
{
	UnCrouch();
	_Mesh->SetWorldScale3D(_StandScale);
}

void ASurvivor_Base::IAJump_Implementation_Implementation(const FInputActionInstance& Instance)
{
	Jump();
}


//Character Functions
void ASurvivor_Base::S_SurvivorDamage_Implementation()
{
	Multi_SurvivorDamage();
}

void ASurvivor_Base::Multi_SurvivorDamage_Implementation()
{
	_survivorHealth--;
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("Hit!"));
	
	if(_survivorHealth <= 0)
	{
		Destroy();
	}
}

