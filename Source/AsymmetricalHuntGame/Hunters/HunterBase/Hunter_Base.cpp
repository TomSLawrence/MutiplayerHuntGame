#include "Hunter_Base.h"

#include "AsymmetricalHuntGame/Interactables/TheClimb.h"
#include "AsymmetricalHuntGame/Interactables/TheVault.h"
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

	_OverlappedClimb = nullptr;
	_OverlappedSurvivor = nullptr;
	_OverlappedVault = nullptr;
	
	_CharacterMovement->SetIsReplicated(true);

	_WalkSpeed = 500.0f;
	_CrouchSpeed = 300.0f;
	_BlockSpeed = 400.0f;
	_CarryingSpeed = 400.0f;
	_SprintSpeed = 1000.0f;

	_canVault = false;
	_IsVaulting = false;
	_MaxVault = 1.0f;
	
	_IsClimbing = false;
	_MaxClimb = 1.0f;
	_TraceDistance = 10.0f;

	_canSlide = true;
	_IsSliding = false;
	_MaxSlide = 1.0f;
	
	_SurvivorInteract = false;
	_isHoldingSurvivor = false;

	_RaisedWeaponLocation = FVector(110.0f, 0.0f, -40.0f);
	_LoweredWeaponLocation = FVector(60.f, 80.f, -40.f);

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

	_HunterActionCollision->OnComponentBeginOverlap.AddDynamic(this, &AHunter_Base::OnHunterCollisionOverlap);
	_HunterActionCollision->OnComponentEndOverlap.AddDynamic(this, &AHunter_Base::OnHunterCollisionEndOverlap);

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
	if(!_isHoldingSurvivor)
	{
		_IsSprinting = true;
		_CharacterMovement->MaxWalkSpeed = _SprintSpeed;
	}
}

void AHunter_Base::IAStopSprinting_Implementation_Implementation(const FInputActionInstance& Instance)
{
	if(!_isHoldingSurvivor)
	{
		_IsSprinting = false;
		_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
	}
}

void AHunter_Base::IACrouch_Implementation_Implementation(const FInputActionInstance& Instance)
{
	if(!_isHoldingSurvivor)
	{
		if(!_IsSliding && _IsSprinting && _canSlide)
		{
			_SlideStartLocation = GetActorLocation();
			_SlideEndLocation = GetActorLocation() + (GetActorForwardVector() * 500.0f);
			_Collision->SetCapsuleHalfHeight(40.0f);
			_CurrentSlide = 0.0f;
			
			Multi_Slide();
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("Sliding!"));
		}
		else
		{
			Crouch();
		}
	}
	
}

void AHunter_Base::IAStand_Implementation_Implementation(const FInputActionInstance& Instance)
{
	if(!_isHoldingSurvivor)
	{
		UnCrouch();
		_canSlide = true;
	}
	
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
		FVector Start = GetActorLocation();
		FVector End = Start + GetActorForwardVector() * _TraceDistance;

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel10);
		
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
		else if(HitResult.bBlockingHit && !_IsClimbing)
		{
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
		_OverlappedSurvivor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		_OverlappedSurvivor->_CharacterMovement->SetMovementMode(MOVE_Walking);
		_isHoldingSurvivor = false;
		_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
	}
}

//Vaulting

void AHunter_Base::Multi_Vault_Implementation()
{
	if(!_IsVaulting)
	{
		_IsSliding = false;
		_IsClimbing = false;
		_IsVaulting = true;
		_canVault = false;

		_Collision->SetCollisionResponseToChannel(ECC_GameTraceChannel10, ECR_Ignore);
		GetWorld()->GetTimerManager().SetTimer(FTimerHandle, this, &AHunter_Base::Multi_UpdateVault, 0.02, true);
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
			_Collision->SetCollisionResponseToChannel(ECC_GameTraceChannel10, ECR_Block);
			GetWorld()->GetTimerManager().ClearTimer(FTimerHandle);
			_IsVaulting = false;
			_canVault = true;
		}
	}
}


//Climbing
void AHunter_Base::Multi_Climb_Implementation()
{
	if(!_IsClimbing)
	{
		if(_OverlappedClimb)
		{
			_IsSliding = false;
			_IsVaulting = false;
			_IsClimbing = true;

			_Collision->SetCollisionResponseToChannel(ECC_GameTraceChannel10, ECR_Ignore);
			GetWorld()->GetTimerManager().SetTimer(FTimerHandle, this, &AHunter_Base::Multi_UpdateClimb, 0.02, true);
		}
	}
}

void AHunter_Base::Multi_UpdateClimb()
{
	if(_OverlappedClimb)
	{
		_CurrentClimb += (0.05f/_MaxClimb);

		FVector NewLocation = FMath::Lerp(_ClimbStartLocation, FVector(GetActorLocation().X, GetActorLocation().Y,(_OverlappedClimb->GetActorLocation().Z * 2) + 50.0f), _CurrentClimb);
		SetActorLocation(NewLocation);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("Climbing!"));
	}
	if(_CurrentClimb >= _MaxClimb)
	{
		_Collision->SetCollisionResponseToChannel(ECC_GameTraceChannel10, ECR_Block);
		GetWorld()->GetTimerManager().ClearTimer(FTimerHandle);
		_IsClimbing = false;
	}
}

void AHunter_Base::Multi_Slide_Implementation()
{
	if(!_IsSliding)
	{
		_IsClimbing = false;
		_IsVaulting = false;
		_canSlide = false;
		
		_IsSliding = true;
		GetWorld()->GetTimerManager().SetTimer(FTimerHandle, this, &AHunter_Base::Multi_UpdateSlide, 0.02, true);
	}
}

void AHunter_Base::Multi_UpdateSlide()
{
	_CurrentSlide += (0.05f/_MaxSlide);
	
	FVector NewLocation = FMath::Lerp(_SlideStartLocation, _SlideEndLocation, _CurrentSlide);
	SetActorLocation(NewLocation);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("Sliding!"));

	if(_CurrentSlide >= _MaxSlide)
	{
		GetWorld()->GetTimerManager().ClearTimer(FTimerHandle);
		_IsSliding = false;
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
	}
	else if (_OverlappedClimb)
	{
		_OverlappedClimb = nullptr;
	}
}

