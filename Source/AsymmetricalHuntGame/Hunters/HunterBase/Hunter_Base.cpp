#include "Hunter_Base.h"

#include "AsymmetricalHuntGame/Interactables/TheClimb.h"
#include "AsymmetricalHuntGame/Interactables/TheVault.h"
#include "AsymmetricalHuntGame/Survivors/SurvivorBase/Survivor_Base.h"
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

	_OverlappedClimb = nullptr;
	_OverlappedSurvivor = nullptr;
	_OverlappedVault = nullptr;
	
	_CharacterMovement->SetIsReplicated(true);

	_WalkSpeed = 500.0f;
	_CrouchSpeed = 300.0f;
	_BlockSpeed = 400.0f;
	_CarryingSpeed = 400.0f;
	_SprintSpeed = 1000.0f;
	_CooldownSpeed = 100.0f;

	_CanSprint = true;

	_canVault = false;
	_IsVaulting = false;
	_MaxVault = 1.0f;
	
	_IsClimbing = false;
	_MaxClimb = 1.0f;
	_TraceDistance = 50.0f;

	_canSlide = true;
	_IsSliding = false;
	_SlidePower = 500.0f;
	_MaxSlide = 2.0f;
	_CurrentSlide = 0.0f;
	_PlayerForward = FVector(0.0f,0.0f,0.0f);
	
	_SurvivorInteract = false;
	_isHoldingSurvivor = false;

	_RaisedWeaponLocation = FVector(110.0f, 0.0f, -40.0f);
	_LoweredWeaponLocation = FVector(60.f, 80.f, -40.f);
	_LungePower = 5000.0f;
	_IsSwinging = false;
	_CanSwing = true;
	_CurrentSwing = 0.0f;
	_CurrentCooldown = 0.0f;
	_MaxCooldown = 0.0f;
	_MaxSwing = 1.0f;
	_HitPlayer = false;

	_CharacterMesh->SetOnlyOwnerSee(false);
	
	SetReplicates(true);
	SetReplicateMovement(true);
	GetCharacterMovement()->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void AHunter_Base::BeginPlay()
{
	Super::BeginPlay();
	
	_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
	_CharacterMovement->MaxWalkSpeedCrouched = _CrouchSpeed;
	
	_IgnoredActors.Add(this);
	_CollisionParams.AddIgnoredActor(this);

	_HunterActionCollision->OnComponentBeginOverlap.AddDynamic(this, &AHunter_Base::OnHunterActionCollisionOverlap);
	_HunterActionCollision->OnComponentEndOverlap.AddDynamic(this, &AHunter_Base::OnHunterActionCollisionEndOverlap);

	_Collision->OnComponentBeginOverlap.AddDynamic(this, &AHunter_Base::OnHunterCollisionOverlap);
	_Collision->OnComponentEndOverlap.AddDynamic(this, &AHunter_Base::OnHunterCollisionEndOverlap);
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
			AddControllerPitchInput(_InputAxis.Y);
		}
		if(_InputAxis.X != 0.0f)
		{
			AddControllerYawInput(_InputAxis.X);
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
	if(!_isHoldingSurvivor && _CanSprint)
	{
		_IsSprinting = true;
		_CharacterMovement->MaxWalkSpeed = _SprintSpeed;
	}
}

void AHunter_Base::IAStopSprinting_Implementation_Implementation(const FInputActionInstance& Instance)
{
	if(!_isHoldingSurvivor && _CanSprint)
	{
		_IsSprinting = false;
		_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
	}
}

void AHunter_Base::IACrouch_Implementation_Implementation(const FInputActionInstance& Instance)
{
	if(!_isHoldingSurvivor)
	{
		if(!_IsSliding && _IsSprinting && _canSlide && !_IsSwinging)
		{
			_IsClimbing = false;
			_IsVaulting = false;
			
			_IsSliding = true;
			_CurrentSlide = 0.0f;
			
			_Collision->SetCapsuleHalfHeight(40.0f);
			GetWorld()->GetTimerManager().SetTimer(FSlideTimerHandle, this, &AHunter_Base::Multi_Slide, 0.02, true);
		}
		else if(!_IsSprinting)
		{
			Crouch();
		}
	}
	
}

void AHunter_Base::IAStand_Implementation_Implementation(const FInputActionInstance& Instance)
{
	if(!_isHoldingSurvivor)
	{
		_Collision->SetCapsuleHalfHeight(100.0f);
		UnCrouch();
		GetWorld()->GetTimerManager().ClearTimer(FSlideTimerHandle);
		_IsSliding = false;
		_canSlide = true;
	}
	
}

void AHunter_Base::IAMelee_Implementation_Implementation(const FInputActionInstance& Instance)
{
	//Melee Function called on individual hunters

	if(HasAuthority() && _CanSwing)
	{
		_CurrentSwing = 0.0f;
		_CurrentCooldown = 0.0f;
		Multi_HunterLunge();
	}
}

void AHunter_Base::IAStopMelee_Implementation_Implementation(const FInputActionInstance& Instance)
{
	if(HasAuthority())
	{
		Multi_MissedLunge();
	}
}

void AHunter_Base::IABlock_Implementation_Implementation(const FInputActionInstance& Instance)
{
	_Camera->SetFieldOfView(30.0f);
	_WeaponMesh->SetRelativeLocation(_RaisedWeaponLocation);
	_CharacterMovement->MaxWalkSpeed = _BlockSpeed;
}

void AHunter_Base::IAStopBlocking_Implementation_Implementation(const FInputActionInstance& Instance)
{
	_Camera->SetFieldOfView(90.0f);
	_WeaponMesh->SetRelativeLocation(_LoweredWeaponLocation);
	_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
}

void AHunter_Base::IAJump_Implementation_Implementation(const FInputActionInstance& Instance)
{
	if(!_isHoldingSurvivor)
	{
		FVector Start = _Camera->GetComponentLocation();
		FVector End = Start + _Camera->GetForwardVector() * _TraceDistance;

		FHitResult HitResult;
		
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel11, _CollisionParams);
		
		if(_canVault && !_IsVaulting)
		{
					
			_CurrentVault = 0.0f;
			_VaultStartLocation = GetActorLocation();
			_VaultLocation = _OverlappedVault->GetActorLocation();

			TargetVaultLocation = FVector::DotProduct(_VaultStartLocation - _VaultLocation ,_OverlappedVault->_EndLocationA - _VaultLocation) > 0
			? _OverlappedVault->_EndLocationB
			: _OverlappedVault->_EndLocationA;
			
			Multi_Vault();
		}
		else if(HitResult.bBlockingHit && !_IsClimbing && _CanClimb)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("Jumping!"));
			_CurrentClimb = 0.0f;
			_ClimbStartLocation = GetActorLocation();
			Multi_Climb();
		}
		else if(!HitResult.bBlockingHit && !_IsVaulting && !_IsClimbing)
		{
			Jump();
		}
	}
}

void AHunter_Base::IAInteract_Implementation_Implementation(const FInputActionInstance& Instance)
{
	if(!_isHoldingSurvivor)
	{
		if(_OverlappedSurvivor)
		{
			if(_OverlappedSurvivor->isDowned)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("Pickup Survivor!"));
				_OverlappedSurvivor->_CharacterMovement->SetMovementMode(MOVE_None);
				_OverlappedSurvivor->AttachToComponent(_PickupLocation, FAttachmentTransformRules::SnapToTargetIncludingScale);
				_isHoldingSurvivor = true;
				_CharacterMovement->MaxWalkSpeed = _CarryingSpeed;
			}
			
		}
	}
	else
	{
		_OverlappedSurvivor->_CharacterMovement->SetMovementMode(MOVE_Walking);
		_OverlappedSurvivor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		_isHoldingSurvivor = false;
		_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
	}
}

//Attacking

void AHunter_Base::Multi_HunterLunge_Implementation()
{
	if(!_IsSwinging)
	{
		_IsSliding = false;
		_IsClimbing = false;
		_IsVaulting = false;
		_IsSwinging = true;

		GetWorld()->GetTimerManager().SetTimer(FActionTimerHandle, this, &AHunter_Base::Multi_UpdateHunterLunge, 0.02, true);
		
	}
}

void AHunter_Base::Multi_UpdateHunterLunge()
{
	_CurrentSwing += (0.05f / _MaxSwing);
	
	if(_CurrentSwing <= _MaxSwing)
	{
		_CharacterMovement->AddForce(GetActorForwardVector() * _LungePower);

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
				GetWorld()->GetTimerManager().ClearTimer(FActionTimerHandle);
				Multi_HitLunge();
				HitSurvivor->S_BaseSurvivorDamage();
			}
			else
			{
				GetWorld()->GetTimerManager().ClearTimer(FActionTimerHandle);
				Multi_HitLunge();
			}
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(FActionTimerHandle);
		Multi_MissedLunge();
	}
}


void AHunter_Base::Multi_MissedLunge_Implementation()
{
	_IsSwinging = false;
	_CanSwing = true;

	GetWorld()->GetTimerManager().SetTimer(FActionTimerHandle, this, &AHunter_Base::Multi_LungeCooldown, 0.02, true);
}

void AHunter_Base::Multi_HitLunge_Implementation()
{
	_IsSwinging = false;
	_CanSwing = true;
	_HitPlayer = true;

	GetWorld()->GetTimerManager().SetTimer(FActionTimerHandle, this, &AHunter_Base::Multi_LungeCooldown, 0.02, true);
}

void AHunter_Base::Multi_LungeCooldown()
{
	if(!_IsSwinging)
	{
		_CurrentCooldown += (0.05/_MaxCooldown);
		if(_HitPlayer)
		{
			_MaxCooldown = 3.0f;

			if(_CurrentCooldown <= _MaxCooldown)
			{
				_CanSprint = false;
				_CharacterMovement->MaxWalkSpeed = _CooldownSpeed;
				_CanSwing = false;
				_canSlide = false;
				_CanClimb = false;
			}
			else if(_CurrentCooldown >= _MaxCooldown)
			{
				GetWorld()->GetTimerManager().ClearTimer(FActionTimerHandle);
				_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
				_CanSwing = true;
				_canSlide = true;
				_CanClimb = true;
				_CanSprint = true;
				_HitPlayer = false;
			}
		}
		else
		{
			_MaxCooldown = 2.0f;
		
			if(_CurrentCooldown <= _MaxCooldown)
			{
				_CanSprint = false;
				_CharacterMovement->MaxWalkSpeed =  _CooldownSpeed;
				_CanSwing = false;
				_canSlide = false;
				_CanClimb = false;
			}
			else if(_CurrentCooldown >= _MaxCooldown)
			{
				GetWorld()->GetTimerManager().ClearTimer(FActionTimerHandle);
				_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
				_CanSwing = true;
				_canSlide = true;
				_CanClimb = true;
				_CanSprint = true;
				_HitPlayer = false;
			}
		}
	}
	
}

//Vaulting

void AHunter_Base::Multi_Vault_Implementation()
{
	if(!_IsVaulting && !_IsSwinging)
	{
		_IsSliding = false;
		_IsClimbing = false;
		_IsVaulting = true;
		_canVault = false;
		
		GetWorld()->GetTimerManager().SetTimer(FActionTimerHandle, this, &AHunter_Base::Multi_UpdateVault, 0.02, true);
	}
}

void AHunter_Base::Multi_UpdateVault()
{
	if(_OverlappedVault)
	{
		_CurrentVault += (0.05f/_MaxVault);

		FVector NewLocation = FMath::Lerp(_VaultStartLocation, TargetVaultLocation, _CurrentVault);
		SetActorLocation(NewLocation);

		if(_CurrentVault >= _MaxVault)
		{
			GetWorld()->GetTimerManager().ClearTimer(FActionTimerHandle);
			_IsVaulting = false;
			_canVault = true;
		}
	}
}


//Climbing
void AHunter_Base::Multi_Climb_Implementation()
{
	if(!_IsClimbing && !_IsSwinging)
	{
		if(_OverlappedClimb)
		{
			_IsSliding = false;
			_IsVaulting = false;
			_IsClimbing = true;

			_Collision->SetCollisionResponseToChannel(ECC_GameTraceChannel10, ECR_Ignore);
			GetWorld()->GetTimerManager().SetTimer(FActionTimerHandle, this, &AHunter_Base::Multi_UpdateClimb, 0.02, true);
		}
	}
}

void AHunter_Base::Multi_UpdateClimb()
{
	_CurrentClimb += (0.05f/_MaxClimb);

	FVector NewLocation = FMath::Lerp(_ClimbStartLocation, FVector(GetActorLocation().X, GetActorLocation().Y,
		(_OverlappedClimb->GetActorLocation().Z * 2) + 50.0f), _CurrentClimb);
	
	SetActorLocation(NewLocation);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("Climbing!"));
	
	if(_CurrentClimb >= _MaxClimb)
	{
		_Collision->SetCollisionResponseToChannel(ECC_GameTraceChannel10, ECR_Block);
		GetWorld()->GetTimerManager().ClearTimer(FActionTimerHandle);
		_IsClimbing = false;
	}
}

void AHunter_Base::Multi_Slide_Implementation()
{
	if(_IsSliding && _canSlide)
	{
		_CurrentSlide += (0.05f / _MaxSlide);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("Sliding!"));
	}

	if(_CurrentSlide >= _MaxSlide)
	{
		_Collision->SetCapsuleHalfHeight(100.0f);
		GetWorld()->GetTimerManager().ClearTimer(FActionTimerHandle);
		_IsSliding = false;
		_canSlide = false;
	}
}

void AHunter_Base::OnHunterActionCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASurvivor_Base* _HitSurvivor = Cast<ASurvivor_Base>(OtherActor);
	_OverlappedSurvivor = _HitSurvivor;
	_SurvivorInteract = true;
}

void AHunter_Base::OnHunterActionCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(_OverlappedSurvivor)
	{
		_SurvivorInteract = false;
		_OverlappedSurvivor = nullptr;
	}
}

void AHunter_Base::OnHunterCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* _HitActor = Cast<AActor>(OtherActor);

	if(ATheVault* _Vault = Cast<ATheVault>(_HitActor))
	{
		_OverlappedVault = _Vault;
		_canVault = true;
	}
	else if (ATheClimb* _ClimbingWall = Cast<ATheClimb>(_HitActor))
	{
		_OverlappedClimb = _ClimbingWall;
	}

}

void AHunter_Base::OnHunterCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(_OverlappedVault)
	{
		_canVault = false;
		_OverlappedVault = nullptr;
		_CurrentVault = 0.0f;
		_IsSliding = false;
	}
	else if (_OverlappedClimb)
	{
		_OverlappedClimb = nullptr;
		_CurrentClimb = 0.0f;
		_IsClimbing = false;
	}
}

