// Fill out your copyright notice in the Description page of Project Settings.


#include "TheFuse.h"

#include "Components/SphereComponent.h"


// Sets default values
ATheFuse::ATheFuse()
{
	_Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	_Mesh->SetupAttachment(_Root);

	_Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	_Collision->SetupAttachment(_Mesh);
}

// Called when the game starts or when spawned
void ATheFuse::BeginPlay()
{
	Super::BeginPlay();
	
}

