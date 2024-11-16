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
	virtual void IASprint_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAStopSprinting_Implementation(const FInputActionInstance& Instance) override;
	virtual void IACrouch_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAStand_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAJump_Implementation(const FInputActionInstance& Instance) override;
};


