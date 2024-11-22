// Fill out your copyright notice in the Description page of Project Settings.


#include "TheVault.h"

#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ATheVault::ATheVault()
{
	_Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	_Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	_Collision->SetupAttachment(_Root);
	_Collision->SetRelativeScale3D(FVector(7.0f, 3.0f, 2.0f));
}

void ATheVault::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATheVault, _EndLocationA);
	DOREPLIFETIME(ATheVault, _EndLocationB);
}

// Called when the game starts or when spawned
void ATheVault::BeginPlay()
{
	Super::BeginPlay();
	
	_EndLocationA = FVector(GetActorLocation().X + 200.0f, GetActorLocation().Y, GetActorLocation().Z);
	_EndLocationB = FVector(GetActorLocation().X - 200.0f, GetActorLocation().Y, GetActorLocation().Z);
	
}
