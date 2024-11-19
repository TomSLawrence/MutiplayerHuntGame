// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyTree.generated.h"

class UCapsuleComponent;

UCLASS()
class ASYMMETRICALHUNTGAME_API AMyTree : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyTree();

	UPROPERTY()
	TObjectPtr<USceneComponent> _Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> _TrunkMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> _LeavesMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCapsuleComponent> _Collision;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
