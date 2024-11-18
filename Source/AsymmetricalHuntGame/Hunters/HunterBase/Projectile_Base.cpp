// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_Base.h"

#include "AsymmetricalHuntGame/Survivors/SurvivorBase/Survivor_Base.h"
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
	
	_ProjectileMovement->InitialSpeed = 15000;
	_ProjectileMovement->MaxSpeed = 15000;
}


// Called when the game starts or when spawned
void AProjectile_Base::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		SetReplicates(true);
		_Collision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile_Base::OnCollisionOverlap);
	}

	
}

void AProjectile_Base::OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASurvivor_Base* _HitSurvivor = Cast<ASurvivor_Base>(OtherActor);
	S_OnCollision(_HitSurvivor);
	
}

void AProjectile_Base::S_OnCollision_Implementation(ASurvivor_Base* _HitSurvivor)
{
	_HitSurvivor->S_BaseSurvivorDamage();
	Destroy();
}


