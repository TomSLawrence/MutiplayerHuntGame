// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AsymmetricalHuntGame/Hunters/HunterBase/Projectile_Base.h"
#include "Projectile_Ghost.generated.h"

UCLASS()
class ASYMMETRICALHUNTGAME_API AProjectile_Ghost : public AProjectile_Base
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile_Ghost();

	virtual void OnCollisionOverlap(  UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void S_OnCollision(ASurvivor_Base* _HitSurvivor) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
};
