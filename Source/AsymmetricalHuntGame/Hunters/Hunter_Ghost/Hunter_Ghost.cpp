#include "Hunter_Ghost.h"

#include "AsymmetricalHuntGame/Controller/ThePlayerController.h"
#include "Components/ArrowComponent.h"
#include "AsymmetricalHuntGame/Hunters/Hunter_Ghost/Projectile_Ghost.h"


// Sets default values
AHunter_Ghost::AHunter_Ghost()
{
	
}

// Called when the game starts or when spawned
void AHunter_Ghost::BeginPlay()
{
	Super::BeginPlay();
}

void AHunter_Ghost::IACharacterMove(FVector _InputAxis)
{
	Super::IACharacterMove(_InputAxis);
}

void AHunter_Ghost::IACharacterLook(FVector _InputAxis)
{
	Super::IACharacterLook(_InputAxis);
}

void AHunter_Ghost::IAAction_Implementation(const FInputActionInstance& Instance)
{
	Super::IAAction_Implementation(Instance);
}

void AHunter_Ghost::IASprint_Implementation(const FInputActionInstance& Instance)
{
	Super::IASprint_Implementation(Instance);
}

void AHunter_Ghost::IAStopSprinting_Implementation(const FInputActionInstance& Instance)
{
	Super::IAStopSprinting_Implementation(Instance);
}

void AHunter_Ghost::IACrouch_Implementation(const FInputActionInstance& Instance)
{
	Super::IACrouch_Implementation(Instance);
}

void AHunter_Ghost::IAStand_Implementation(const FInputActionInstance& Instance)
{
	Super::IAStand_Implementation(Instance);
}

void AHunter_Ghost::IAJump_Implementation(const FInputActionInstance& Instance)
{
	Super::IAJump_Implementation(Instance);
}

void AHunter_Ghost::IAShoot_Implementation(const FInputActionInstance& Instance)
{
	if(HasAuthority())
	{
		FActorSpawnParameters _ProjectileSpawnParams;
		_ProjectileSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		_ProjectileSpawnParams.Owner = GetOwner();
		_ProjectileSpawnParams.Instigator = GetInstigator();
		GetWorld()->SpawnActor(_Projectile, &_ProjectileSpawn->GetComponentTransform(), _ProjectileSpawnParams);
	}
}

void AHunter_Ghost::IAAim_Implementation(const FInputActionInstance& Instance)
{
	Super::IAAim_Implementation(Instance);
}
