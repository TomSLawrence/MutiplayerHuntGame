// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AsymmetricalHuntGame/Controller/Interfaces/IAInterface.h"
#include "Hunter_Base.generated.h"

//Forward Declarations
class ASurvivor_Base;
class AProjectile_Base;
class ATheVault;
class ATheClimb;
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

	//Basic Character Functions
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
	virtual void IAStopMelee_Implementation(const FInputActionInstance& Instance) override;
	UFUNCTION(NetMulticast, Reliable)
	virtual void IABlock_Implementation(const FInputActionInstance& Instance) override;
	UFUNCTION(NetMulticast, Reliable)
	virtual void IAStopBlocking_Implementation(const FInputActionInstance& Instance) override;
	UFUNCTION(NetMulticast, Reliable)
	virtual void IAJump_Implementation(const FInputActionInstance& Instance) override;
	UFUNCTION(NetMulticast, Reliable)
	virtual void IAInteract_Implementation(const FInputActionInstance& Instance) override;

	//Attack Functions
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_HunterLunge();
	UFUNCTION()
	virtual void Multi_UpdateHunterLunge();
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_MissedLunge();
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_HitLunge();
	UFUNCTION()
	virtual void Multi_LungeCooldown();

	
	//Movement Mechanics

	//Vaulting
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_Vault();
	UFUNCTION()
	virtual void Multi_UpdateVault();

	//Climbing
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_Climb();
	UFUNCTION()
	virtual void Multi_UpdateClimb();

	//Sliding
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_Slide();

	//Standing Up
	UFUNCTION()
	virtual void CharacterStand();

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

	//Interaction Collision (Different from player collider)
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> _HunterActionCollision;

	//Ignored Actors for Attacks
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> _IgnoredActors;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Hunter Actions Collider
	UFUNCTION()
	virtual void OnHunterActionCollisionOverlap(  UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnHunterActionCollisionEndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//General Hunter Collision
	UFUNCTION()
	virtual void OnHunterCollisionOverlap(  UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnHunterCollisionEndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Player Speed Variables
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

	//Sprinting Variables
	UPROPERTY(EditAnywhere)
	bool _IsSprinting;
	UPROPERTY(EditAnywhere)
	bool _CanSprint;

	//Attacking Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _LungePower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool _IsSwinging;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool _CanSwing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool _HitPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _CurrentSwing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _MaxSwing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _CurrentCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _MaxCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _CooldownSpeed;

	//Vaulting Variables
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool _canVault;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool _IsVaulting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _CurrentVault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _MaxVault;

	//Vaulting Location Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector _VaultStartLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector _VaultLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetVaultLocation;

	//Climbing Variables
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool _IsClimbing;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool _CanClimb;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _CurrentClimb;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _MaxClimb;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _TraceDistance;
	

	//Climbing Location Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector _ClimbStartLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector _ClimbLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetClimbLocation;

	//Sliding Variables
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool _canSlide;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool _IsSliding;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _CurrentSlide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _MaxSlide;

	//Timer Handles
	UPROPERTY() 
	FTimerHandle FActionTimerHandle;
	UPROPERTY()
	FTimerHandle FAttackHandle;
	UPROPERTY()
	FTimerHandle FSlideTimerHandle;

	//Player Climbing Collision Check
	FCollisionQueryParams _CollisionParams;

	//Weapon Locations - *Not Implemented yet*
	UPROPERTY()
	FVector _RaisedWeaponLocation;
	UPROPERTY()
	FVector _LoweredWeaponLocation;

	//Lunge Start Location
	UPROPERTY()
	FVector _StartLocation;

	//Item Pickup attach location
	UPROPERTY(EditAnywhere)
	TObjectPtr<UArrowComponent> _PickupLocation;

	//Actors that collide with the Hunter
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ASurvivor_Base> _OverlappedSurvivor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ATheVault> _OverlappedVault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ATheClimb> _OverlappedClimb;

	//Holding Survivor Check
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool _isHoldingSurvivor;

public:
	
	UCameraComponent* Get_Camera() const
	{
		return _Camera;
	}
};


