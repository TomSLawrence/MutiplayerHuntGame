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



