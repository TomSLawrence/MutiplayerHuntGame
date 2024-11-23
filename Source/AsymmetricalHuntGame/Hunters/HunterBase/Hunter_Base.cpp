#include "Hunter_Base.h"

#include "AsymmetricalHuntGame/Survivors/SurvivorBase/Survivor_Base.h"
#include "AsymmetricalHuntGame/Map/Assets/MyTree.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"


DEFINE_LOG_CATEGORY_STATIC(LogHunterBase, Display, All);

AHunter_Base::AHunter_Base()
{
	
	_Collision = GetCapsuleComponent();
	_CharacterMovement = GetCharacterMovement();

	_CharacterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Character_Mesh"));
	_CharacterMesh->SetupAttachment(_Collision);
	_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_Camera->SetupAttachment(_Collision);
	_Camera->SetRelativeLocation(FVector(-10.0f, 0.0f, 60.0f));
	_Camera->bUsePawnControlRotation = true;

	_HunterActionCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Hunter Action Collision"));
	_HunterActionCollision->SetupAttachment(_Collision);
	_HunterActionCollision->SetCapsuleHalfHeight(150.0f);
	_HunterActionCollision->SetCapsuleRadius(150.0f);

	_PickupLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("Pickup Location"));
	_PickupLocation->SetupAttachment(_Collision);
	_MeleeLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("Melee Location"));
	_MeleeLocation->SetupAttachment(_Collision);

	_WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon_Mesh"));
	_WeaponMesh->SetupAttachment(_Camera);
	
	
	_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
	_CharacterMovement->MaxWalkSpeedCrouched = _CrouchSpeed;
	_CharacterMovement->SetIsReplicated(true);

	_CharacterMesh->SetOnlyOwnerSee(false);

	SetReplicates(true);
	SetReplicateMovement(true);
	GetCharacterMovement()->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void AHunter_Base::BeginPlay()
{
	Super::BeginPlay();

	_CrouchScale = FVector(1.5f, 1.5f, 1.5f);
	_StandScale = FVector(2.0f, 2.0f, 2.0f);
	
	_RaisedWeaponLocation = FVector(110.0f, 0.0f, -40.0f);
	_LoweredWeaponLocation = FVector(60.f, 80.f, -40.f);
	_FiringDistance = 10000;

	_WalkSpeed = 300.0f;
	_CrouchSpeed = 150.0f;
	_BlockSpeed = 200.0f;
	_CarryingSpeed = 200.0f;
	
	isAiming = false;
	_SurvivorInteract = false;
	_isHoldingSurvivor = false;

	_IgnoredActors.Add(this);

	_HunterActionCollision->OnComponentBeginOverlap.AddDynamic(this, &AHunter_Base::OnHunterCollisionOverlap);
	_HunterActionCollision->OnComponentEndOverlap.AddDynamic(this, &AHunter_Base::OnHunterCollisionEndOverlap);
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

void AHunter_Base::IAStopAction_Implementation_Implementation(const FInputActionInstance& Instance)
{
	//Stop Action
}

void AHunter_Base::IASprint_Implementation_Implementation(const FInputActionInstance& Instance)
{
	if(!_isHoldingSurvivor)
	{
		_CharacterMovement->MaxWalkSpeed = _SprintSpeed;
	}
}

void AHunter_Base::IAStopSprinting_Implementation_Implementation(const FInputActionInstance& Instance)
{
	if(!_isHoldingSurvivor)
	{
		_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
	}
}

void AHunter_Base::IACrouch_Implementation_Implementation(const FInputActionInstance& Instance)
{
	Crouch();
	_CharacterMesh->SetWorldScale3D(_CrouchScale);
}

void AHunter_Base::IAStand_Implementation_Implementation(const FInputActionInstance& Instance)
{
	
	UnCrouch();
	_CharacterMesh->SetWorldScale3D(_StandScale);
	
}

void AHunter_Base::IAMelee_Implementation_Implementation(const FInputActionInstance& Instance)
{
	//Shooting Function called on individual hunters

	if(HasAuthority())
	{
		_StartLocation = _MeleeLocation->GetComponentLocation();

		FHitResult HitResult;

		bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), _StartLocation, _StartLocation, 30.0f, 
			TraceTypeQuery3, false, _IgnoredActors, EDrawDebugTrace::ForDuration, HitResult, true,
			FLinearColor::Red, FLinearColor::Green, 1.0f);

		
		if(bHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("Hit Survivor!"));
			if(ASurvivor_Base* HitSurvivor = Cast<ASurvivor_Base>(HitResult.GetActor()))
			{
				HitSurvivor->S_BaseSurvivorDamage();
				
			}
			else if(AMyTree* _HitTree = Cast<AMyTree>(HitResult.GetActor()))
			{
				if(_HitTree)
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("Hit Tree!"));
				}
			}
		
		}
	}
	
}

void AHunter_Base::IABlock_Implementation_Implementation(const FInputActionInstance& Instance)
{
	isAiming = true;
	_Camera->SetFieldOfView(30.0f);
	_WeaponMesh->SetRelativeLocation(_RaisedWeaponLocation);
	_CharacterMovement->MaxWalkSpeed = _BlockSpeed;
}

void AHunter_Base::IAStopBlocking_Implementation_Implementation(const FInputActionInstance& Instance)
{
	isAiming = false;
	_Camera->SetFieldOfView(90.0f);
	_WeaponMesh->SetRelativeLocation(_LoweredWeaponLocation);
	_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
}

void AHunter_Base::IAInteract_Implementation_Implementation(const FInputActionInstance& Instance)
{
	if(!_isHoldingSurvivor)
	{
		if(_OverlappedSurvivor)
		{
			if(_OverlappedSurvivor->isDowned)
			{
				_OverlappedSurvivor->_CharacterMovement->SetMovementMode(MOVE_None);
				_OverlappedSurvivor->AttachToComponent(_PickupLocation, FAttachmentTransformRules::SnapToTargetIncludingScale);
				_isHoldingSurvivor = true;
				_CharacterMovement->MaxWalkSpeed = _CarryingSpeed;
			}
			
		}
	}
	else
	{
		_OverlappedSurvivor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		_OverlappedSurvivor->_CharacterMovement->SetMovementMode(MOVE_Walking);
		_isHoldingSurvivor = false;
		_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
	}
}


void AHunter_Base::IAJump_Implementation_Implementation(const FInputActionInstance& Instance)
{
	
}





void AHunter_Base::OnHunterCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASurvivor_Base* _HitSurvivor = Cast<ASurvivor_Base>(OtherActor);
	_OverlappedSurvivor = _HitSurvivor;
	_SurvivorInteract = true;
}

void AHunter_Base::OnHunterCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(_OverlappedSurvivor)
	{
		_SurvivorInteract = false;
		_OverlappedSurvivor = nullptr;
	}
}

