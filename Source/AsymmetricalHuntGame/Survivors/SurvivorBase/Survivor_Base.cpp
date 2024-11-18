#include "Survivor_Base.h"

#include "AsymmetricalHuntGame/Hunters/HunterBase/Projectile_Base.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogSurvivorBase, Display, All);

// Sets default values
ASurvivor_Base::ASurvivor_Base()
{
	_Collision = GetCapsuleComponent();
	_ActionCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Action Collision"));
	_HeadCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Head Collision"));
	_HeadCollision->SetupAttachment(_Collision);
	_ActionCollision->SetupAttachment(_Collision);
	_ActionCollision->SetCapsuleHalfHeight(120.0f);
	_ActionCollision->SetCapsuleRadius(120.0f);
	_HeadCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
	
	_CharacterMovement = GetCharacterMovement();
	_PlayerVelocity = _CharacterMovement->GetLastUpdateVelocity();

	_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	_Mesh->SetupAttachment(_Collision);
	_HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
	_HeadMesh->SetupAttachment(_Collision);
	_EyeMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Eye1"));
	_EyeMesh1->SetupAttachment(_HeadMesh);
	_EyeMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Eye2"));
	_EyeMesh2->SetupAttachment(_HeadMesh);
	
	_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_Camera->SetupAttachment(_Collision);
	_Camera->SetRelativeLocation(FVector(30.0f, 0.0f, 70.0f));
	_Camera->bUsePawnControlRotation = true;
	
	_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
	_CharacterMovement->MaxWalkSpeedCrouched = _CrouchSpeed;

	SetReplicates(true);
	SetReplicateMovement(true);
	_CharacterMovement->SetIsReplicated(true);

	_SurvivorMaxHealth = 2.0f;


}

void ASurvivor_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASurvivor_Base, _HealTime);
	DOREPLIFETIME(ASurvivor_Base, canHeal);
	DOREPLIFETIME(ASurvivor_Base, _SurvivorHealth);
}


// Called when the game starts or when spawned
void ASurvivor_Base::BeginPlay()
{
	Super::BeginPlay();
	
	_CharacterMovement->MaxWalkSpeed = _WalkSpeed;
	_StandScale = FVector(2.0f, 2.0f, 2.0f);
	_CrouchScale = FVector(1.5f, 1.5f, 1.5f);
	canHeal = false;
	_HealTime = 0.0f;
	_OverlappedSurvivor = nullptr;
	_SurvivorHealth = _SurvivorMaxHealth;

	_ActionCollision->OnComponentBeginOverlap.AddDynamic(this, &ASurvivor_Base::OnActionCollisionOverlap);
	_ActionCollision->OnComponentEndOverlap.AddDynamic(this, &ASurvivor_Base::OnActionCollisionEndOverlap);

	_HeadCollision->OnComponentBeginOverlap.AddDynamic(this, &ASurvivor_Base::OnHeadCollisionOverlap);

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

	if(canHeal)
	{
		S_HealingSurvivorAction();
	}
}

void ASurvivor_Base::IAStopAction_Implementation_Implementation(const FInputActionInstance& Instance)
{
	S_StopHealingSurvivor();
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


//Server Functions

//Damage
void ASurvivor_Base::S_BaseSurvivorDamage_Implementation()
{
	Multi_BaseSurvivorDamage();
}

void ASurvivor_Base::Multi_BaseSurvivorDamage_Implementation()
{
	_SurvivorHealth--;
	
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("Hit!"));
	
	if(_SurvivorHealth <= 0)
	{
		Destroy();
	}
}

void ASurvivor_Base::S_HeadSurvivorDamage_Implementation(AProjectile_Base* _Projectile)
{
	Multi_HeadSurvivorDamage(_Projectile);
}

void ASurvivor_Base::Multi_HeadSurvivorDamage_Implementation(AProjectile_Base* _Projectile)
{
	_SurvivorHealth = 0;
	_Projectile->Destroy();
	Destroy();
}





//Healing
void ASurvivor_Base::S_HealingSurvivorAction_Implementation()
{
	Multi_HealingSurvivorAction();
}

void ASurvivor_Base::Multi_HealingSurvivorAction_Implementation()
{
	if(HasAuthority())
	{
		if(_OverlappedSurvivor->_SurvivorHealth < _SurvivorMaxHealth)
		{
			if(_OverlappedSurvivor->_HealTime < 10)
			{
				_CharacterMovement->SetMovementMode(MOVE_None);
				_OverlappedSurvivor->GetCharacterMovement()->SetMovementMode(MOVE_None);
				
				if(!FHealHandle.IsValid())
				{
					GetWorld()->GetTimerManager().SetTimer(FHealHandle, this,
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
		else if(!_OverlappedSurvivor)
		{
			canHeal = false;
			S_StopHealingSurvivor();
			_CharacterMovement->SetMovementMode(MOVE_Walking);
			_OverlappedSurvivor->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
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
	GetWorldTimerManager().ClearTimer(FHealHandle);
}

void ASurvivor_Base::S_HealSurvivor_Implementation()
{
	Multi_HealSurvivor();
}

void ASurvivor_Base::Multi_HealSurvivor_Implementation()
{
	if(FHealHandle.IsValid())
	{
		if(_OverlappedSurvivor->_SurvivorHealth < _SurvivorMaxHealth)
		{
			_OverlappedSurvivor->_HealTime += 1.0f;
		}
	}
}







//Collisions

void ASurvivor_Base::OnActionCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASurvivor_Base* _HitSurvivor = Cast<ASurvivor_Base>(OtherActor);
	_OverlappedSurvivor = _HitSurvivor;
	
	if(_OverlappedSurvivor->_SurvivorHealth < _SurvivorMaxHealth)
	{
		canHeal = true;
	}
}

void ASurvivor_Base::OnActionCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASurvivor_Base* _HitSurvivor = Cast<ASurvivor_Base>(OtherActor);
	if(_HitSurvivor)
	{
		_OverlappedSurvivor = nullptr;
		canHeal = false;
	}
}

void ASurvivor_Base::OnHeadCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AProjectile_Base* _ProjectileHit = Cast<AProjectile_Base>(OtherActor);
	if(_ProjectileHit)
	{
		S_HeadSurvivorDamage(_ProjectileHit);
	}
}


