// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTree.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AMyTree::AMyTree()
{
	_Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	_TrunkMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrunkMesh"));
	_TrunkMesh->SetupAttachment(_Root);
	_LeavesMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeavesMesh"));
	_LeavesMesh->SetupAttachment(_Root);


	_Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	_Collision->SetupAttachment(_Root);

	SetReplicates(true);
}

// Called when the game starts or when spawned
void AMyTree::BeginPlay()
{
	Super::BeginPlay();
	
	float RandomTreeSize = UKismetMathLibrary::RandomIntegerInRange(7, 12);
	
	_TrunkMesh->SetRelativeScale3D(FVector(1,1, 4));
	_LeavesMesh->SetRelativeScale3D(FVector(RandomTreeSize, RandomTreeSize, RandomTreeSize));
}
