// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile_Base.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class ASurvivor_Base;

UCLASS(Abstract)
class ASYMMETRICALHUNTGAME_API AProjectile_Base : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile_Base();

	//Creating the Projectile
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> _Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> _Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> _Collision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UProjectileMovementComponent> _ProjectileMovement;


	UFUNCTION(Server, Reliable)
	 virtual void OnCollisionOverlap(  UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool _hasOverlapped;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

};
