// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_Ghost.h"


// Sets default values
AProjectile_Ghost::AProjectile_Ghost()
{
	
}

// Called when the game starts or when spawned
void AProjectile_Ghost::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectile_Ghost::OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnCollisionOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

