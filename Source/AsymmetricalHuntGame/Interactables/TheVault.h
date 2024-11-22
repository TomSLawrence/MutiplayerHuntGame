// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TheVault.generated.h"

class UBoxComponent;

UCLASS()
class ASYMMETRICALHUNTGAME_API ATheVault : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATheVault();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FVector _EndLocationA;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FVector _EndLocationB;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> _Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> _Collision;
};
