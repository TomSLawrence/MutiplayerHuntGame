﻿#include "Survivor_Craig.h"

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

void ASurvivor_Craig::OnHunterCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnHunterCollisionOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
	                                SweepResult);
}

void ASurvivor_Craig::OnHunterCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnHunterCollisionEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}


