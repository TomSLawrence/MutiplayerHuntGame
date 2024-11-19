// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBush.h"

#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AMyBush::AMyBush()
{
	_Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	_BushMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrunkMesh"));
	_BushMesh->SetupAttachment(_Root);

	SetReplicates(true);
}

void AMyBush::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyBush, RandomBushSize);
}

// Called when the game starts or when spawned
void AMyBush::BeginPlay()
{
	Super::BeginPlay();

	RandomBushSize = UKismetMathLibrary::RandomIntegerInRange(0.5, 1);
	_BushMesh->SetRelativeScale3D(FVector(RandomBushSize,RandomBushSize, RandomBushSize));
	
}

