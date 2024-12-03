#include "Survivor_Craig.h"

DEFINE_LOG_CATEGORY_STATIC(LogSurvivorCraig, Display, All);

// Sets default values
ASurvivor_Craig::ASurvivor_Craig()
{
	
}

// Called when the game starts or when spawned
void ASurvivor_Craig::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void ASurvivor_Craig::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASurvivor_Craig::IACharacterMove(FVector _InputAxis)
{
	Super::IACharacterMove(_InputAxis);
}

void ASurvivor_Craig::IACharacterLook(FVector _InputAxis)
{
	Super::IACharacterLook(_InputAxis);
}

void ASurvivor_Craig::IAAction_Implementation(const FInputActionInstance& Instance)
{
	Super::IAAction_Implementation(Instance);
}

void ASurvivor_Craig::IAStopAction_Implementation(const FInputActionInstance& Instance)
{
	Super::IAStopAction_Implementation(Instance);
}

void ASurvivor_Craig::IASprint_Implementation(const FInputActionInstance& Instance)
{
	Super::IASprint_Implementation(Instance);
}

void ASurvivor_Craig::IAStopSprinting_Implementation(const FInputActionInstance& Instance)
{
	Super::IAStopSprinting_Implementation(Instance);
}

void ASurvivor_Craig::IACrouch_Implementation(const FInputActionInstance& Instance)
{
	Super::IACrouch_Implementation(Instance);
}

void ASurvivor_Craig::IAStand_Implementation(const FInputActionInstance& Instance)
{
	Super::IAStand_Implementation(Instance);
}

void ASurvivor_Craig::IAJump_Implementation(const FInputActionInstance& Instance)
{
	Super::IAJump_Implementation(Instance);
}

void ASurvivor_Craig::IAInteract_Implementation(const FInputActionInstance& Instance)
{
	Super::IAInteract_Implementation(Instance);
}


//Movement Functions
void ASurvivor_Craig::S_Vault()
{
	Super::S_Vault();
}

void ASurvivor_Craig::Multi_Vault()
{
	Super::Multi_Vault();
}

void ASurvivor_Craig::S_UpdateVault()
{
	Super::S_UpdateVault();
}

void ASurvivor_Craig::Multi_UpdateVault()
{
	Super::Multi_UpdateVault();
}

void ASurvivor_Craig::S_Climb()
{
	Super::S_Climb();
}

void ASurvivor_Craig::Multi_Climb()
{
	Super::Multi_Climb();
}

void ASurvivor_Craig::S_UpdateClimb()
{
	Super::S_UpdateClimb();
}

void ASurvivor_Craig::Multi_UpdateClimb()
{
	Super::Multi_UpdateClimb();
}

void ASurvivor_Craig::S_Slide()
{
	Super::S_Slide();
}

void ASurvivor_Craig::Multi_Slide()
{
	Super::Multi_Slide();
}

void ASurvivor_Craig::S_CharacterStand()
{
	Super::S_CharacterStand();
}

void ASurvivor_Craig::Multi_CharacterStand()
{
	Super::Multi_CharacterStand();
}


//Server functions
void ASurvivor_Craig::S_BaseSurvivorDamage()
{
	Super::S_BaseSurvivorDamage();
}

void ASurvivor_Craig::Multi_BaseSurvivorDamage()
{
	Super::Multi_BaseSurvivorDamage();
}

void ASurvivor_Craig::S_HealingSurvivorAction()
{
	Super::S_HealingSurvivorAction();
}

void ASurvivor_Craig::Multi_HealingSurvivorAction()
{
	Super::Multi_HealingSurvivorAction();
}





void ASurvivor_Craig::S_HealSurvivor()
{
	Super::S_HealSurvivor();
}

void ASurvivor_Craig::S_StopHealingSurvivor()
{
	Super::S_StopHealingSurvivor();
}

void ASurvivor_Craig::Multi_StopHealingSurvivor()
{
	Super::Multi_StopHealingSurvivor();
}

void ASurvivor_Craig::Multi_HealSurvivor()
{
	Super::Multi_HealSurvivor();
}



void ASurvivor_Craig::OnSurvivorCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSurvivorCollisionOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
	                                  SweepResult);
}

void ASurvivor_Craig::OnSurvivorCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSurvivorCollisionEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}




