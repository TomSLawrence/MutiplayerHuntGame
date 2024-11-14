// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AsymmetricalHuntGame/Hunters/HunterBase/Hunter_Base.h"
#include "Hunter_Ghost.generated.h"

class AProjectile_Ghost;
class UArrowComponent;
class UCameraComponent;

UCLASS()
class ASYMMETRICALHUNTGAME_API AHunter_Ghost : public AHunter_Base
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHunter_Ghost();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AProjectile_Ghost> _Projectile;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UArrowComponent> _ProjectileSpawn;

	UPROPERTY()
	TObjectPtr<UCameraComponent> _playerCamera;

public:

	//Player Movement/Looking
	UFUNCTION(Server, Reliable)
	virtual void IAMove_Implementation(float _MoveX, float _MoveY) override;
	UFUNCTION(Server, Reliable)
	virtual void IALook_Implementation(const FInputActionInstance& Instance) override;
	UFUNCTION(Server, Reliable)
	virtual void IAAction_Implementation(const FInputActionInstance& Instance) override;
	UFUNCTION(Server, Reliable)
	virtual void IASprint_Implementation(const FInputActionInstance& Instance) override;
	UFUNCTION(Server, Reliable)
	virtual void IAStopSprinting_Implementation(const FInputActionInstance& Instance) override;
	UFUNCTION(Server, Reliable)
	virtual void IACrouch_Implementation(const FInputActionInstance& Instance) override;
	UFUNCTION(Server, Reliable)
	virtual void IAStand_Implementation(const FInputActionInstance& Instance) override;
	UFUNCTION(Server, Reliable)
	virtual void IAJump_Implementation(const FInputActionInstance& Instance) override;
	UFUNCTION(Server, Reliable)
	virtual void IAShoot_Implementation(const FInputActionInstance& Instance) override;
	UFUNCTION(Server, Reliable)
	virtual void IAAim_Implementation(const FInputActionInstance& Instance) override;

};

