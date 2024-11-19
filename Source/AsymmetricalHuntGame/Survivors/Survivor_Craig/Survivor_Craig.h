#pragma once

#include "CoreMinimal.h"
#include "AsymmetricalHuntGame/Survivors/SurvivorBase/Survivor_Base.h"
#include "Survivor_Craig.generated.h"

UCLASS()
class ASYMMETRICALHUNTGAME_API ASurvivor_Craig : public ASurvivor_Base
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASurvivor_Craig();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void IACharacterMove(FVector _InputAxis) override;
	virtual void IACharacterLook(FVector _InputAxis) override;
	virtual void IAAction_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAStopAction_Implementation(const FInputActionInstance& Instance) override;
	virtual void IASprint_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAStopSprinting_Implementation(const FInputActionInstance& Instance) override;
	virtual void IACrouch_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAStand_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAJump_Implementation(const FInputActionInstance& Instance) override;

	//Server Functions

	//Damage
	virtual void S_BaseSurvivorDamage() override;
	virtual void Multi_BaseSurvivorDamage() override;

	//Healing
	virtual void S_HealingSurvivorAction() override;
	virtual void Multi_HealingSurvivorAction() override;
	virtual void S_StopHealingSurvivor() override;
	virtual void Multi_StopHealingSurvivor() override;
	virtual void S_HealSurvivor() override;
	virtual void Multi_HealSurvivor() override;

	//Collisions
	virtual void OnSurvivorCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSurvivorCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	virtual void OnHunterCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnHunterCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};


