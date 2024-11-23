// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AsymmetricalHuntGame/Controller/Interfaces/IAInterface.h"
#include "Hunter_Base.generated.h"

class ASurvivor_Base;
class AProjectile_Base;
class AMyTree;
class UCapsuleComponent;
class UCameraComponent;
class UArrowComponent;
class UCharacterMovementComponent;

UCLASS(Abstract)
class ASYMMETRICALHUNTGAME_API AHunter_Base : public ACharacter, public IIAInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHunter_Base();
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void IACharacterMove(FVector _InputAxis);
	UFUNCTION(NetMulticast, Reliable)
	virtual void IACharacterLook(FVector _InputAxis);
	UFUNCTION(NetMulticast, Reliable)
	virtual void IAAction_Implementation(const FInputActionInstance& Instance) override;
	UFUNCTION(NetMulticast, Reliable)
	virtual void IAStopAction_Implementation(const FInputActionInstance& Instance) override;
	UFUNCTION(NetMulticast, Reliable)
	virtual void IASprint_Implementation(const FInputActionInstance& Instance) override;
	UFUNCTION(NetMulticast, Reliable)
	virtual void IAStopSprinting_Implementation(const FInputActionInstance& Instance) override;
	UFUNCTION(NetMulticast, Reliable)
	virtual void IACrouch_Implementation(const FInputActionInstance& Instance) override;
	UFUNCTION(NetMulticast, Reliable)
	virtual void IAStand_Implementation(const FInputActionInstance& Instance) override;
	UFUNCTION(NetMulticast, Reliable)
	virtual void IAMelee_Implementation(const FInputActionInstance& Instance) override;
	UFUNCTION(NetMulticast, Reliable)
	virtual void IABlock_Implementation(const FInputActionInstance& Instance) override;
	UFUNCTION(NetMulticast, Reliable)
	virtual void IAStopBlocking_Implementation(const FInputActionInstance& Instance) override;
	UFUNCTION(NetMulticast, Reliable)
	virtual void IAJump_Implementation(const FInputActionInstance& Instance) override;
	UFUNCTION(NetMulticast, Reliable)
	virtual void IAInteract_Implementation(const FInputActionInstance& Instance) override;


	//Player Components
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> _CharacterMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> _WeaponMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> _Collision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> _Camera;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCharacterMovementComponent> _CharacterMovement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UArrowComponent> _MeleeLocation;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> _HunterActionCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> _IgnoredActors;
	
	//Public variables


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHunterCollisionOverlap(  UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnHunterCollisionEndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Movement Variables
	UPROPERTY(EditAnywhere)
	float _CarryingSpeed;
	UPROPERTY(EditAnywhere)
	float _WalkSpeed;
	UPROPERTY(EditAnywhere)
	float _CrouchSpeed;
	UPROPERTY(EditAnywhere)
	float _SprintSpeed;
	UPROPERTY(EditAnywhere)
	float _BlockSpeed;

	UPROPERTY()
	FVector _StandScale;
	UPROPERTY()
	FVector _CrouchScale;

	UPROPERTY()
	FVector _RaisedWeaponLocation;
	UPROPERTY()
	FVector _LoweredWeaponLocation;

	//Aiming Sensitivity
	UPROPERTY(VisibleAnywhere)
	bool isAiming;
	UPROPERTY(EditAnywhere)
	float _AimingSensitivity;

	UPROPERTY()
	FVector _StartLocation;
	UPROPERTY()
	int _FiringDistance;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UArrowComponent> _PickupLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ASurvivor_Base> _OverlappedSurvivor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool _SurvivorInteract;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool _isHoldingSurvivor;

public:
	
	UCameraComponent* Get_Camera() const
	{
		return _Camera;
	}
};


