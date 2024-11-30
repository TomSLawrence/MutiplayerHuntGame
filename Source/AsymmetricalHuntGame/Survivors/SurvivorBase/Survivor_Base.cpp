#include "Survivor_Base.h"

#include "AsymmetricalHuntGame/Hunters/HunterBase/Hunter_Base.h"
#include "AsymmetricalHuntGame/Interactables/TheBeacon.h"
#include "AsymmetricalHuntGame/Interactables/TheClimb.h"
#include "AsymmetricalHuntGame/Interactables/TheFuse.h"
#include "AsymmetricalHuntGame/Interactables/TheVault.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogSurvivorBase, Display, All);

// Sets default values
ASurvivor_Base::ASurvivor_Base()
{
	_Collision = GetCapsuleComponent();
	_SurvivorActionCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Survivor Action Collision"));
	
	_SurvivorActionCollision->SetupAttachment(_Collision);
	_SurvivorActionCollision->SetCapsuleHalfHeight(120.0f);
	_SurvivorActionCollision->SetCapsuleRadius(120.0f);
	
	_CharacterMovement = GetCharacterMovement();
	_PlayerVelocity = _CharacterMovement->GetLastUpdateVelocity();

	_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	_Mesh->SetupAttachment(_Collision);
	_HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
	_HeadMesh->SetupAttachment(_Mesh);
	_EyeMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Eye1"));
	_EyeMesh1->SetupAttachment(_HeadMesh);
	_EyeMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Eye2"));
	_EyeMesh2->SetupAttachment(_HeadMesh);

	_PickupLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("Pickup Location"));
	_PickupLocation->SetupAttachment(_Collision);
	
	_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_Camera->SetupAttachment(_Collision);
	_Camera->SetRelativeLocation(FVector(-10.0f, 0.0f, 60.0f));
	_Camera->bUsePawnControlRotation = true;

	
	_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
	_CharacterMovement->MaxWalkSpeedCrouched = _CrouchSpeed;

	_Mesh->SetOnlyOwnerSee(false);
	_HeadMesh->SetOnlyOwnerSee(false);

	SetReplicates(true);
	SetReplicateMovement(true);
	_CharacterMovement->SetIsReplicated(true);

	//Initialising Variables
	
	_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
	_IsSprinting = false;

	_WalkSpeed = 500.0f;
	_SprintSpeed = 1000.0f;
	_CrouchSpeed = 300.0f;
	
	_SurvivorMaxHealth = 2.0f;
	canHeal = false;
	_HealTime = 0.0f;
	isDowned = false;
	
	_OverlappedSurvivor = nullptr;
	_OverlappedFuse = nullptr;
	_OverlappedVault = nullptr;
	_OverlappedClimb = nullptr;
	
	_isHoldingSurvivor = false;
	_isHoldingFuse = false;
	
	_canVault = false;
	_IsVaulting = false;
	_MaxVault = 1.0f;
	
	_IsClimbing = false;
	_MaxClimb = 1.0f;
	_TraceDistance = 50.0f;

	_canSlide = true;
	_IsSliding = false;
	_MaxSlide = 2.0f;
	_CurrentSlide = 0.0f;


}

void ASurvivor_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASurvivor_Base, _HealTime);
	DOREPLIFETIME(ASurvivor_Base, canHeal);
	DOREPLIFETIME(ASurvivor_Base, _SurvivorHealth);
	DOREPLIFETIME(ASurvivor_Base, isDowned);
	
	DOREPLIFETIME(ASurvivor_Base, _VaultLocation);
	DOREPLIFETIME(ASurvivor_Base, TargetVaultLocation);
	DOREPLIFETIME(ASurvivor_Base, _CurrentVault);
	DOREPLIFETIME(ASurvivor_Base, _VaultStartLocation);
	DOREPLIFETIME(ASurvivor_Base, _MaxVault);
	
	DOREPLIFETIME(ASurvivor_Base, _CurrentClimb);
	DOREPLIFETIME(ASurvivor_Base, _MaxClimb);
	DOREPLIFETIME(ASurvivor_Base, _ClimbStartLocation);
	DOREPLIFETIME(ASurvivor_Base, _ClimbLocation);
	DOREPLIFETIME(ASurvivor_Base, TargetClimbLocation);

	DOREPLIFETIME(ASurvivor_Base, _CurrentSlide);
	DOREPLIFETIME(ASurvivor_Base, _SlideStartLocation);
	DOREPLIFETIME(ASurvivor_Base, _SlideEndLocation);
	
}

// Called when the game starts or when spawned
void ASurvivor_Base::BeginPlay()
{
	Super::BeginPlay();

	_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
	_CharacterMovement->MaxWalkSpeedCrouched = _CrouchSpeed;
	
	//Setting Survivor Health
	_SurvivorHealth = _SurvivorMaxHealth;

	_CollisionParams.AddIgnoredActor(this);
	
	_SurvivorActionCollision->OnComponentBeginOverlap.AddDynamic(this, &ASurvivor_Base::OnSurvivorActionCollisionOverlap);
	_SurvivorActionCollision->OnComponentEndOverlap.AddDynamic(this, &ASurvivor_Base::OnSurvivorActionCollisionEndOverlap);

	_Collision->OnComponentBeginOverlap.AddDynamic(this, &ASurvivor_Base::OnSurvivorCollisionOverlap);
	_Collision->OnComponentEndOverlap.AddDynamic(this, &ASurvivor_Base::OnSurvivorCollisionEndOverlap);
	
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
	//Any actions go here

	if(!isDowned && !_isHoldingSurvivor)
	{
		if(canHeal && !_isHoldingFuse)
		{
			S_HealingSurvivorAction();
		}
		else if(_CanRepair)
		{
			S_RepairingBeaconAction();
		}
		
	}
}

void ASurvivor_Base::IAStopAction_Implementation_Implementation(const FInputActionInstance& Instance)
{
	if(!isDowned && !_isHoldingSurvivor)
	{
		if(canHeal)
		{
			S_StopHealingSurvivor();
		}
		else if(_CanRepair)
		{
			S_StopRepairingBeaconAction();
		}
	}
}

void ASurvivor_Base::IASprint_Implementation_Implementation(const FInputActionInstance& Instance)
{
	if(!isDowned && !_isHoldingSurvivor)
	{
		if(!_IsSprinting)
		{
			_IsSprinting = true;
			_CharacterMovement->MaxWalkSpeed = _SprintSpeed;
		}
	}
}

void ASurvivor_Base::IAStopSprinting_Implementation_Implementation(const FInputActionInstance& Instance)
{
	if(!isDowned && !_isHoldingSurvivor)
	{
		_IsSprinting = false;
		_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
	}
}

void ASurvivor_Base::IACrouch_Implementation_Implementation(const FInputActionInstance& Instance)
{
	if(!isDowned && !_isHoldingSurvivor)
	{
		if(!_IsSliding && _IsSprinting && _canSlide)
		{
			_IsClimbing = false;
			_IsVaulting = false;
			
			_IsSliding = true;
			_CurrentSlide = 0.0f;
			
			_Collision->SetCapsuleHalfHeight(40.0f);
			
			S_Slide();
		}
		else if(!_IsSprinting)
		{
			Crouch();
		}
	}
}

void ASurvivor_Base::IAStand_Implementation_Implementation(const FInputActionInstance& Instance)
{
	if(!isDowned && !_isHoldingSurvivor)
	{
		_Collision->SetCapsuleHalfHeight(100.0f);
		UnCrouch();
		GetWorld()->GetTimerManager().ClearTimer(FTimerHandle);
		_IsSliding = false;
		_canSlide = true;
	}
}

void ASurvivor_Base::IAJump_Implementation_Implementation(const FInputActionInstance& Instance)
{
	if(!isDowned && !_isHoldingSurvivor)
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
			
			S_Vault();
		}
		else if(HitResult.bBlockingHit && !_IsClimbing)
		{
			_CurrentClimb = 0.0f;
			_ClimbStartLocation = GetActorLocation();
			S_Climb();
		}
		else if(!HitResult.bBlockingHit && !_IsVaulting && !_IsClimbing)
		{
			Jump();
		}
	}
}

void ASurvivor_Base::IAInteract_Implementation_Implementation(const FInputActionInstance& Instance)
{
	if(!isDowned)
	{
		if(_OverlappedSurvivor)
		{
			if(_OverlappedSurvivor->isDowned)
			{
				if(!_isHoldingSurvivor && !_isHoldingFuse)
				{
					_OverlappedSurvivor->_CharacterMovement->SetMovementMode(MOVE_None);
					_OverlappedSurvivor->AttachToComponent(_PickupLocation, FAttachmentTransformRules::SnapToTargetIncludingScale);
					_isHoldingSurvivor = true;
					canHeal = false;
				}
				else
				{
					_OverlappedSurvivor->_CharacterMovement->SetMovementMode(MOVE_Walking);
					_OverlappedSurvivor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
					_isHoldingSurvivor = false;
					canHeal = true;
				}
			}
		}
		if(_OverlappedFuse)
		{
			if(!_isHoldingSurvivor && !_isHoldingFuse)
			{
				_OverlappedFuse->AttachToComponent(_PickupLocation, FAttachmentTransformRules::SnapToTargetIncludingScale);
				_isHoldingFuse = true;
				canHeal = false;
			}
			else
			{
				_OverlappedFuse->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				_isHoldingFuse = false;
			}
		}
		
	}
}



//Functions

//---------------------------------------------------------------------------------------

//Movement Functions


//Vaulting
void ASurvivor_Base::S_Vault_Implementation()
{
	Multi_Vault();
}

void ASurvivor_Base::Multi_Vault_Implementation()
{
	if(!_IsVaulting && _OverlappedVault)
	{
		_IsSliding = false;
		_IsClimbing = false;
		_IsVaulting = true;
		_canVault = false;
		
		GetWorld()->GetTimerManager().SetTimer(FTimerHandle, this, &ASurvivor_Base::S_UpdateVault, 0.02, true);
	}
}

void ASurvivor_Base::S_UpdateVault_Implementation()
{
	Multi_UpdateVault();
}

void ASurvivor_Base::Multi_UpdateVault_Implementation()
{
	_CurrentVault += (0.05f/_MaxVault);

	FVector NewLocation = FMath::Lerp(_VaultStartLocation, TargetVaultLocation, _CurrentVault);
	SetActorLocation(NewLocation);

	if(_CurrentVault >= _MaxVault)
	{
		GetWorld()->GetTimerManager().ClearTimer(FTimerHandle);
		_IsVaulting = false;
		_canVault = true;
	}
}

//---------------------------------------------------------------------------------------

//Climbing

void ASurvivor_Base::S_Climb_Implementation()
{
	Multi_Climb();
}

void ASurvivor_Base::Multi_Climb_Implementation()
{
	if(!_IsClimbing && _OverlappedClimb)
	{
		_IsSliding = false;
		_IsVaulting = false;
		_IsClimbing = true;
		
		_CurrentClimb = 0.0f;

		_Collision->SetCollisionResponseToChannel(ECC_GameTraceChannel10, ECR_Ignore);
		GetWorld()->GetTimerManager().SetTimer(FTimerHandle, this, &ASurvivor_Base::S_UpdateClimb, 0.02, true);
	}
}

void ASurvivor_Base::S_UpdateClimb_Implementation()
{
	Multi_UpdateClimb();
}

void ASurvivor_Base::Multi_UpdateClimb_Implementation()
{
	if(_OverlappedClimb)
	{
		_CurrentClimb += (0.05f/_MaxClimb);

		FVector NewLocation = FMath::Lerp(_ClimbStartLocation, FVector(GetActorLocation().X, GetActorLocation().Y,
			(_OverlappedClimb->GetActorLocation().Z * 2) + 50.0f), _CurrentClimb);
		
		SetActorLocation(NewLocation);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("Climbing!"));
		
		if(_CurrentClimb >= _MaxClimb)
		{
			_Collision->SetCollisionResponseToChannel(ECC_GameTraceChannel10, ECR_Block);
			GetWorld()->GetTimerManager().ClearTimer(FTimerHandle);
			_IsClimbing = false;
		}
	}
}

//---------------------------------------------------------------------------------------

//Sliding

void ASurvivor_Base::S_Slide_Implementation()
{
	Multi_Slide();
}

void ASurvivor_Base::Multi_Slide_Implementation()
{
	if(_IsSliding && _canSlide)
	{
		GetWorld()->GetTimerManager().SetTimer(FTimerHandle, this, &ASurvivor_Base::S_UpdateSlide, 0.02, true);
	}
}

void ASurvivor_Base::S_UpdateSlide_Implementation()
{
	Multi_UpdateSlide();
}

void ASurvivor_Base::Multi_UpdateSlide_Implementation()
{
	_CurrentSlide += (0.05f / _MaxSlide);

	if(_CurrentSlide >= _MaxSlide)
	{
		_Collision->SetCapsuleHalfHeight(100.0f);
		GetWorld()->GetTimerManager().ClearTimer(FTimerHandle);
		_IsSliding = false;
		_canSlide = false;
	}
}

//---------------------------------------------------------------------------------------
//Damage
void ASurvivor_Base::S_BaseSurvivorDamage_Implementation()
{
	Multi_BaseSurvivorDamage();
}

void ASurvivor_Base::Multi_BaseSurvivorDamage_Implementation()
{
	_SurvivorHealth--;
	
	if(_SurvivorHealth <= 0)
	{
		S_SurvivorDowned();
	}
	
	
}

//---------------------------------------------------------------------------------------

//Healing
void ASurvivor_Base::S_HealingSurvivorAction_Implementation()
{
	Multi_HealingSurvivorAction();
}

void ASurvivor_Base::Multi_HealingSurvivorAction_Implementation()
{
	if(HasAuthority() && _OverlappedSurvivor->_SurvivorHealth < _SurvivorMaxHealth)
	{
		if(!_OverlappedSurvivor->isDowned)
		{
			if(_OverlappedSurvivor->_HealTime < 10)
			{
				_CharacterMovement->SetMovementMode(MOVE_None);
				_OverlappedSurvivor->GetCharacterMovement()->SetMovementMode(MOVE_None);
				
				if(!FTimerHandle.IsValid())
				{
					GetWorld()->GetTimerManager().SetTimer(FTimerHandle, this,
					&ASurvivor_Base::S_HealSurvivor, 1.0f, true);
					GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("Healing!"));
				}
			}
			else
			{
				_OverlappedSurvivor->_SurvivorHealth = _SurvivorMaxHealth;
				canHeal = false;
				S_StopHealingSurvivor();
				_OverlappedSurvivor->_HealTime = 0.0f;
			}
				
		}
		else
		{
			if(_OverlappedSurvivor->_HealTime < 5)
			{
				_CharacterMovement->SetMovementMode(MOVE_None);
				_OverlappedSurvivor->GetCharacterMovement()->SetMovementMode(MOVE_None);
				
				if(!FTimerHandle.IsValid())
				{
					GetWorld()->GetTimerManager().SetTimer(FTimerHandle, this,
					&ASurvivor_Base::S_HealSurvivor, 1.0f, true);
					GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("Healing!"));
				}
			}
			else
			{
				S_StopHealingSurvivor();
				_OverlappedSurvivor->S_SurvivorRevived();
			}
		}
			
	}
	else if(!_OverlappedSurvivor)
	{
		canHeal = false;
		S_StopHealingSurvivor();
	}
}

void ASurvivor_Base::S_HealSurvivor_Implementation()
{
	Multi_HealSurvivor();
}

void ASurvivor_Base::Multi_HealSurvivor_Implementation()
{
	if(FTimerHandle.IsValid())
	{
		if(_OverlappedSurvivor->_SurvivorHealth < _SurvivorMaxHealth)
		{
			_OverlappedSurvivor->_HealTime += 1.0f;
		}
	}
}

void ASurvivor_Base::S_StopHealingSurvivor_Implementation()
{
	Multi_StopHealingSurvivor();
}

void ASurvivor_Base::Multi_StopHealingSurvivor_Implementation()
{
	_CharacterMovement->SetMovementMode(MOVE_Walking);
	if(_OverlappedSurvivor)
	{
		_OverlappedSurvivor->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
	GetWorldTimerManager().ClearTimer(FTimerHandle);
}

//---------------------------------------------------------------------------------------
//Survivor Downed
void ASurvivor_Base::S_SurvivorDowned_Implementation()
{
	Multi_SurvivorDowned();
}

void ASurvivor_Base::Multi_SurvivorDowned_Implementation()
{
	isDowned = true;
	_Camera->SetRelativeLocation(FVector(30.0f, 0.0f, -70.0f));
	_Mesh->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));
	_Collision->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Ignore);
	_Collision->SetCollisionObjectType(ECC_GameTraceChannel8);
	_CharacterMovement->MaxWalkSpeed = 100.0f;
	
	if(_isHoldingFuse)
	{
		_OverlappedFuse->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		_isHoldingFuse = false;
	}
}

void ASurvivor_Base::S_SurvivorRevived_Implementation()
{
	Multi_SurvivorRevived();
}

void ASurvivor_Base::Multi_SurvivorRevived_Implementation()
{
	_SurvivorHealth = 1;
	_Camera->SetRelativeLocation(FVector(30.0f, 0.0f, 100.0f));
	_Collision->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Overlap);
	_Collision->SetCollisionObjectType(ECC_GameTraceChannel2);
	_Mesh->SetWorldRotation(FRotator(-0.0f, 0.0f, 0.0f));
	_CharacterMovement->SetMovementMode(MOVE_Walking);
	isDowned = false;
	_HealTime = 0.0f;
}

//---------------------------------------------------------------------------------------

//Searching Chests 

void ASurvivor_Base::S_SearchingChestAction_Implementation()
{
	Multi_SearchingChestAction();
}

void ASurvivor_Base::Multi_SearchingChestAction_Implementation()
{
	//Searching Chests
}
void ASurvivor_Base::S_StopSearchingChestAction_Implementation()
{
	Multi_StopSearchingChestAction();
}

void ASurvivor_Base::Multi_StopSearchingChestAction_Implementation()
{
	_CharacterMovement->SetMovementMode(MOVE_Walking);
	GetWorldTimerManager().ClearTimer(FTimerHandle);
}

//Searching Chests
void ASurvivor_Base::S_SearchChests_Implementation()
{
	Multi_SearchChests();
}

void ASurvivor_Base::Multi_SearchChests_Implementation()
{
	if(FTimerHandle.IsValid())
	{
		//Search Chests
	}
}



//Fixing Beacons


void ASurvivor_Base::S_RepairingBeaconAction_Implementation()
{
	Multi_RepairingBeaconAction();
}

void ASurvivor_Base::Multi_RepairingBeaconAction_Implementation()
{
	if(HasAuthority())
	{
		if(_OverlappedBeacon->_HasFuse || _isHoldingFuse)
		{
			
			if(!_OverlappedBeacon->_HasFuse && _isHoldingFuse)
			{
				_OverlappedBeacon->_HasFuse = true;
				_isHoldingFuse = false;
				_OverlappedBeacon->_LightMesh->SetVisibility(true);
				_OverlappedFuse->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				_OverlappedFuse->Destroy();
			}
			else if(_OverlappedBeacon->_RepairTime < _OverlappedBeacon->_MaxRepairTime)
			{
				_CharacterMovement->SetMovementMode(MOVE_None);
				if(!FTimerHandle.IsValid())
				{
					GetWorld()->GetTimerManager().SetTimer(FTimerHandle, this,
						&ASurvivor_Base::S_RepairBeacons, 1.0f, true);
				
				}
			}
			else if(_OverlappedBeacon->_RepairTime >= _OverlappedBeacon->_MaxRepairTime)
			{
				_OverlappedBeacon->_isCompleted = true;
				S_StopRepairingBeaconAction();
			}
		
		}
		else if(!_OverlappedBeacon)
		{
			_CanRepair = false;
			S_StopRepairingBeaconAction();
		}
	}
	
}

void ASurvivor_Base::S_StopRepairingBeaconAction_Implementation()
{
	Multi_StopRepairingBeaconAction();
}

void ASurvivor_Base::Multi_StopRepairingBeaconAction_Implementation()
{
	_CharacterMovement->SetMovementMode(MOVE_Walking);
	GetWorldTimerManager().ClearTimer(FTimerHandle);
}

void ASurvivor_Base::S_RepairBeacons_Implementation()
{
	Multi_RepairBeacons();
}

void ASurvivor_Base::Multi_RepairBeacons_Implementation()
{
	if(FTimerHandle.IsValid())
	{
		_OverlappedBeacon->_RepairTime += 1.0f;
	}
}




//Collisions

void ASurvivor_Base::OnSurvivorCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* _HitActor = Cast<AActor>(OtherActor);

	if(ATheVault* _Vault = Cast<ATheVault>(_HitActor))
	{
		_OverlappedVault = _Vault;
		_canVault = true;
	}
	else if(ATheClimb* _ClimbingWall = Cast<ATheClimb>(_HitActor))
	{
		_OverlappedClimb = _ClimbingWall;
	}
}

void ASurvivor_Base::OnSurvivorCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(_OverlappedVault)
	{
		_canVault = false;
		_OverlappedVault = nullptr;
	}
	else if(_OverlappedClimb)
	{
		_OverlappedClimb = nullptr;
	}
}

void ASurvivor_Base::OnSurvivorActionCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* _HitActor = Cast<AActor>(OtherActor);

	if(ASurvivor_Base* _HitSurvivor = Cast<ASurvivor_Base>(_HitActor))
	{
		_OverlappedSurvivor = _HitSurvivor;
		if(_OverlappedSurvivor->_SurvivorHealth < _SurvivorMaxHealth)
		{
			canHeal = true;
		}
	}
	else if(ATheFuse* HitFuse = Cast<ATheFuse>(_HitActor))
	{
		_OverlappedFuse = HitFuse;
	}
	else if(ATheBeacon* HitBeacon = Cast<ATheBeacon>(_HitActor))
	{
		_OverlappedBeacon = HitBeacon;
		if(_OverlappedBeacon->_RepairTime < _OverlappedBeacon->_MaxRepairTime)
		{
			_CanRepair = true;
		}
	}
}

void ASurvivor_Base::OnSurvivorActionCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(_OverlappedSurvivor)
	{
		canHeal = false;
		_OverlappedSurvivor = nullptr;
	}
	else if(_OverlappedFuse)
	{
		_OverlappedFuse = nullptr;
	}
	else if(_OverlappedBeacon)
	{
		_CanRepair = false;
		_OverlappedBeacon = nullptr;
	}
}


