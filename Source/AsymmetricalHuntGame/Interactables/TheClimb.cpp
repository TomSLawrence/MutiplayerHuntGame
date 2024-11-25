// Fill out your copyright notice in the Description page of Project Settings.


#include "TheClimb.h"
#include "Components/BoxComponent.h"

// Sets default values
ATheClimb::ATheClimb()
{
	_Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	_Mesh->SetupAttachment(_Root);
	
	_Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	_Collision->SetupAttachment(_Root);
}

// Called when the game starts or when spawned
void ATheClimb::BeginPlay()
{
	Super::BeginPlay();
	
}

