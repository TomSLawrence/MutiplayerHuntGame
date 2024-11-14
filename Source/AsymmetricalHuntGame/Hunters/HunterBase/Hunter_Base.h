// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AsymmetricalHuntGame/Controller/Interfaces/IAInterface.h"
#include "Hunter_Base.generated.h"

class UCapsuleComponent;
class UCameraComponent;
class UCharacterMovementComponent;

UCLASS(Abstract)
class ASYMMETRICALHUNTGAME_API AHunter_Base : public ACharacter, public IIAInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHunter_Base();

	//Player Movement/Looking Client-Side
	virtual void IAMove_Implementation(float _MoveX, float _MoveY) override;
	virtual void IALook_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAAction_Implementation(const FInputActionInstance& Instance) override;
	virtual void IASprint_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAStopSprinting_Implementation(const FInputActionInstance& Instance) override;
	virtual void IACrouch_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAStand_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAJump_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAShoot_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAAim_Implementation(const FInputActionInstance& Instance) override;
	virtual void IAStopAiming_Implementation(const FInputActionInstance& Instance) override;


	//Player Components
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> _Mesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> _WeaponMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> _Collision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> _Camera;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCharacterMovementComponent> _CharacterMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Movement Variables
	UPROPERTY(EditAnywhere)
	float _SprintSpeed;
	UPROPERTY(EditAnywhere)
	float _WalkSpeed;
	UPROPERTY(EditAnywhere)
	float _CrouchSpeed;

	//Aiming Sensitivity
	UPROPERTY(VisibleAnywhere)
	bool isAiming;
	UPROPERTY(EditAnywhere)
	float _AimingSensitivity;

	//Player Velocity
	UPROPERTY()
	FVector _PlayerVelocity;


public:
	
	UCameraComponent* Get_Camera() const
	{
		return _Camera;
	}
};


