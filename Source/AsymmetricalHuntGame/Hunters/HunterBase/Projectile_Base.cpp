// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_Base.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectile_Base::AProjectile_Base()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	_Mesh->SetupAttachment(_Root);
	_Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	_Collision->SetupAttachment(_Mesh);
	_ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	
}

// Called when the game starts or when spawned
void AProjectile_Base::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		SetReplicates(true);
	}

	_ProjectileMovement->InitialSpeed = 10000;
	_ProjectileMovement->MaxSpeed = 10000;
	
}

void AProjectile_Base::ApplyProjectileDamage()
{
	OnApplyDamage.Broadcast();
}

// Called every frame
void AProjectile_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

