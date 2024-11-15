﻿#include "Hunter_Base.h"

#include "AsymmetricalHuntGame/Controller/ThePlayerController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogHunterBase, Display, All);

AHunter_Base::AHunter_Base()
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
	_CharacterMovement->SetIsReplicated(true);


	_CrouchHeight = (FVector(1.0f, 1.0f, 0.5f));
	_StandHeight = (FVector(2.0f, 2.0f, 2.0f));
}



// Called when the game starts or when spawned
void AHunter_Base::BeginPlay()
{
	Super::BeginPlay();
	
	_Collision->OnComponentBeginOverlap.AddDynamic(this, &AHunter_Base::OnOverlapBegin);
	
	isAiming = false;
}

void AHunter_Base::IACharacterMove_Implementation(FVector _InputAxis)
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

void AHunter_Base::IACharacterLook_Implementation(FVector _InputAxis)
{
	if(Controller != nullptr)
	{
		if(_InputAxis.Y != 0.0f)
		{
			if(isAiming)
			{
				AddControllerPitchInput(_InputAxis.Y / _AimingSensitivity);
			}
			else
			{
				AddControllerPitchInput(_InputAxis.Y);
			}
		}
		if(_InputAxis.X != 0.0f)
		{
			if(isAiming)
			{
				AddControllerYawInput(_InputAxis.X / _AimingSensitivity);
			}
			else
			{
				AddControllerYawInput(_InputAxis.X);
			}
		}
	}
}

void AHunter_Base::IAAction_Implementation_Implementation(const FInputActionInstance& Instance)
{
	bool BoolValue = Instance.GetValue().Get<bool>();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("FIRE!"));
}

void AHunter_Base::IASprint_Implementation_Implementation(const FInputActionInstance& Instance)
{
	_CharacterMovement->MaxWalkSpeed = _SprintSpeed;
}

void AHunter_Base::IAStopSprinting_Implementation_Implementation(const FInputActionInstance& Instance)
{
	_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
}

void AHunter_Base::IACrouch_Implementation_Implementation(const FInputActionInstance& Instance)
{
	Crouch();
}

void AHunter_Base::IAStand_Implementation_Implementation(const FInputActionInstance& Instance)
{
	UnCrouch();
	
}

void AHunter_Base::IAJump_Implementation_Implementation(const FInputActionInstance& Instance)
{
	Jump();
}

void AHunter_Base::IAShoot_Implementation_Implementation(const FInputActionInstance& Instance)
{
	IIAInterface::IAShoot_Implementation(Instance);
}

void AHunter_Base::IAAim_Implementation_Implementation(const FInputActionInstance& Instance)
{
	isAiming = true;
	_Camera->SetFieldOfView(30.0f);
}

void AHunter_Base::IAStopAiming_Implementation_Implementation(const FInputActionInstance& Instance)
{
	isAiming = false;
	_Camera->SetFieldOfView(90.0f);
}
void AHunter_Base::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("Hit!"));
}
