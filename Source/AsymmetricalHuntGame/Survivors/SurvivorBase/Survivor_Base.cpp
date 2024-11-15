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
	
	_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_Camera->SetupAttachment(_Collision);
	_Camera->SetRelativeLocation(FVector(-10.0f, 0.0f, 60.0f));
	_Camera->bUsePawnControlRotation = true;

	_CharacterMovement->AirControl = 5.0f;


}

// Called when the game starts or when spawned
void ASurvivor_Base::BeginPlay()
{
	Super::BeginPlay();
	
	_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
}

void ASurvivor_Base::IAAction_Implementation(const FInputActionInstance& Instance)
{
	bool BoolValue = Instance.GetValue().Get<bool>();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("FIRE!"));
}

void ASurvivor_Base::IASprint_Implementation(const FInputActionInstance& Instance)
{
	_CharacterMovement->MaxWalkSpeed = _SprintSpeed;
}

void ASurvivor_Base::IAStopSprinting_Implementation(const FInputActionInstance& Instance)
{
	_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
}

void ASurvivor_Base::IACrouch_Implementation(const FInputActionInstance& Instance)
{
	_Collision->SetCapsuleSize(_Collision->GetScaledCapsuleRadius(), 10.0f, true);
	_CharacterMovement->MaxWalkSpeed = _CrouchSpeed;
	
}

void ASurvivor_Base::IAStand_Implementation(const FInputActionInstance& Instance)
{
	_Collision->SetCapsuleSize(_Collision->GetScaledCapsuleRadius(), 68.0f, true);
	_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
}

void ASurvivor_Base::IAJump_Implementation(const FInputActionInstance& Instance)
{
	Jump();
	UE_LOG(LogSurvivorBase, Display, TEXT("Jumping"));
}


