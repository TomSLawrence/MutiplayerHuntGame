#pragma once

#include "CoreMinimal.h"
#include "AsymmetricalHuntGame/Controller/Interfaces/IAInterface.h"
#include "GameFramework/Character.h"
#include "Survivor_Base.generated.h"

class ATheClimb;
class ATheVault;
class ATheBeacon;
class ATheFuse;
class AHunter_Base;
class AProjectile_Base;
class USphereComponent;
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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Survivor Movement
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
	virtual void IAJump_Implementation(const FInputActionInstance& Instance) override;
	UFUNCTION(NetMulticast, Reliable)
	virtual void IAInteract_Implementation(const FInputActionInstance& Instance) override;

	//Server Functions

	//Deal Damage
	UFUNCTION(Server, Reliable)
	virtual void S_BaseSurvivorDamage();
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_BaseSurvivorDamage();

	//Downing Survivor
	UFUNCTION(Server, Reliable)
	virtual void S_SurvivorDowned();
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_SurvivorDowned();
	UFUNCTION(Server, Reliable)
	virtual void S_SurvivorRevived();
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_SurvivorRevived();

	//Healing
	UFUNCTION(Server, Reliable)
	virtual void S_HealSurvivor();
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_HealSurvivor();
	UFUNCTION(Server, Reliable)
	virtual void S_HealingSurvivorAction();
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_HealingSurvivorAction();
	UFUNCTION(Server, Reliable)
	virtual void S_StopHealingSurvivor();
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_StopHealingSurvivor();

	//Searching Chests
	UFUNCTION(Server, Reliable)
	virtual void S_SearchChests();
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_SearchChests();
	UFUNCTION(Server, Reliable)
	virtual void S_SearchingChestAction();
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_SearchingChestAction();
	UFUNCTION(Server, Reliable)
	virtual void S_StopSearchingChestAction();
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_StopSearchingChestAction();
	

	//Fixing Beacons
	UFUNCTION(Server, Reliable)
	virtual void S_RepairBeacons();
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_RepairBeacons();
	UFUNCTION(Server, Reliable)
	virtual void S_RepairingBeaconAction();
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_RepairingBeaconAction();
	UFUNCTION(Server, Reliable)
	virtual void S_StopRepairingBeaconAction();
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_StopRepairingBeaconAction();


	//Movement Mechanics
	UFUNCTION(Server, Reliable)
	virtual void S_Vault();
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_Vault();
	UFUNCTION(Server, Reliable)
	virtual void S_UpdateVault();
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_UpdateVault();

	UFUNCTION(Server, Reliable)
	virtual void S_Climb();
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_Climb();
	UFUNCTION(Server, Reliable)
	virtual void S_UpdateClimb();
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_UpdateClimb();

	//Collisions
	UFUNCTION()
	virtual void OnSurvivorActionCollisionOverlap(  UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSurvivorActionCollisionEndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnSurvivorCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSurvivorCollisionEndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> _Collision;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> _SurvivorActionCollision;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool isDowned;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCharacterMovementComponent> _CharacterMovement;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool _isHoldingFuse;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Player Components
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> _Mesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> _HeadMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> _EyeMesh1;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> _EyeMesh2;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UArrowComponent> _PickupLocation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> _Camera;

	//Movement Variables
	UPROPERTY(EditAnywhere)
	float _SprintSpeed;
	UPROPERTY(EditAnywhere)
	float _WalkSpeed;
	UPROPERTY(EditAnywhere)
	float _CrouchSpeed;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	int _SurvivorHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int _SurvivorMaxHealth;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	float _HealTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ATheBeacon> _OverlappedBeacon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool _CanRepair;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool _canVault;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool _IsVaulting;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	float _CurrentVault;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	float _MaxVault;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FVector _VaultStartLocation;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FVector _VaultLocation;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FVector TargetVaultLocation;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool _canClimb;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool _IsClimbing;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	float _CurrentClimb;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	float _MaxClimb;
	

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FVector _ClimbStartLocation;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FVector _ClimbLocation;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FVector TargetClimbLocation;
	
	UPROPERTY()
	FTimerHandle FTimerHandle;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool canHeal;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool _isHoldingObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ASurvivor_Base> _OverlappedSurvivor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ATheFuse> _OverlappedFuse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ATheVault> _OverlappedVault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ATheClimb> _OverlappedClimb;

	//Player Velocity
	UPROPERTY()
	FVector _PlayerVelocity;

public:

	UCameraComponent* Get_Camera() const
	{
		return _Camera;
	}
};


