// Fill out your copyright notice in the Description page of Project Settings.


#include "TheClimb.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ATheClimb::ATheClimb()
{
	_Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	_Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	_Collision->SetupAttachment(_Root);
	_Collision->SetRelativeScale3D(FVector(3.0f, 2.0f, 2.0f));
}

void ATheClimb::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATheClimb, _EndLocationA);
}

// Called when the game starts or when spawned
void ATheClimb::BeginPlay()
{
	Super::BeginPlay();

	_EndLocationA = FVector(GetActorLocation().X , GetActorLocation().Y, GetActorLocation().Z + 50.0f);
	
}

