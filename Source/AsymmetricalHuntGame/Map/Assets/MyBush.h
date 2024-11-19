// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyBush.generated.h"

UCLASS()
class ASYMMETRICALHUNTGAME_API AMyBush : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyBush();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY()
	TObjectPtr<USceneComponent> _Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> _BushMesh;

	UPROPERTY(Replicated)
	float RandomBushSize;
};
