// Fill out your copyright notice in the Description page of Project Settings.


#include "TheBeacon.h"

#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ATheBeacon::ATheBeacon()
{
	_Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	_BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	_BaseMesh->SetupAttachment(_Root);
	_LightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightMesh"));
	_LightMesh->SetupAttachment(_Root);

	_Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	_Collision->SetupAttachment(_BaseMesh);

	SetReplicates(true);
}

void ATheBeacon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATheBeacon, _RepairTime);
	DOREPLIFETIME(ATheBeacon, _HasFuse);
	DOREPLIFETIME(ATheBeacon, _isCompleted);
}

// Called when the game starts or when spawned
void ATheBeacon::BeginPlay()
{
	Super::BeginPlay();

	_RepairTime = 0.0f;
	_MaxRepairTime = 10.0f;
	_HasFuse = false;
	_isCompleted = false;
	_LightMesh->SetVisibility(false);
	
}

