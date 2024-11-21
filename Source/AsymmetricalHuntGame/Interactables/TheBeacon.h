// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TheBeacon.generated.h"

class USphereComponent;

UCLASS()
class ASYMMETRICALHUNTGAME_API ATheBeacon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATheBeacon();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	float _RepairTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _MaxRepairTime;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool _HasFuse;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool _isCompleted;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> _LightMesh;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> _Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> _BaseMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> _Collision;
};
