﻿#pragma once

#include "CoreMinimal.h"
#include "AsymmetricalHuntGame/Controller/Interfaces/IAInterface.h"
#include "GameFramework/Character.h"
#include "Survivor_Base.generated.h"

class UCapsuleComponent;
class UCameraComponent;
class UCharacterMovementComponent;

UCLASS(Abstract)
class ASYMMETRICALHUNTGAME_API ASurvivor_Base : public ACharacter, public IIAInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASurvivor_Base();

	//Player Movement/Looking
	virtual void IAMove_Implementation(const FInputActionInstance& Instance) override;
	virtual void IALook_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAAction_Implementation(const FInputActionInstance& Instance) override;
	virtual void IASprint_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAStopSprinting_Implementation(const FInputActionInstance& Instance) override;
	virtual void IACrouch_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAStand_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAJump_Implementation(const FInputActionInstance& Instance) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Player Components
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> _Mesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> _Collision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> _Camera;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCharacterMovementComponent> _CharacterMovement;

	//Movement Variables
	UPROPERTY(EditAnywhere)
	float _SprintSpeed;
	UPROPERTY(EditAnywhere)
	float _WalkSpeed;
	UPROPERTY(EditAnywhere)
	float _CrouchSpeed;

	//Player Velocity
	UPROPERTY()
	FVector _PlayerVelocity;

public:

	UCameraComponent* Get_Camera() const
	{
		return _Camera;
	}
};


