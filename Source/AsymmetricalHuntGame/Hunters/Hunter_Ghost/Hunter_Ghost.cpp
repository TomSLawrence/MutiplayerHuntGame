#include "Hunter_Ghost.h"

#include "AsymmetricalHuntGame/Controller/ThePlayerController.h"


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

void AHunter_Ghost::IAMelee_Implementation(const FInputActionInstance& Instance)
{
	Super::IAMelee_Implementation(Instance);
}

void AHunter_Ghost::IABlock_Implementation(const FInputActionInstance& Instance)
{
	Super::IABlock_Implementation(Instance);
}

void AHunter_Ghost::IAStopBlocking_Implementation(const FInputActionInstance& Instance)
{
	Super::IAStopBlocking_Implementation(Instance);
}

void AHunter_Ghost::IAInteract_Implementation(const FInputActionInstance& Instance)
{
	Super::IAInteract_Implementation(Instance);
}

void AHunter_Ghost::OnHunterCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnHunterCollisionOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
									SweepResult);
}

void AHunter_Ghost::OnHunterCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnHunterCollisionEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
