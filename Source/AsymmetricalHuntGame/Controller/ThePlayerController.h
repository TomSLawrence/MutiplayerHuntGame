// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "AsymmetricalHuntGame/Controller/Interfaces/IAInterface.h"
#include "ThePlayerController.generated.h"

class UInputComponent;
class ACharacter;

//Survivor Classes
class ASurvivor_Craig;

UCLASS(Abstract)
class ASYMMETRICALHUNTGAME_API AThePlayerController : public APlayerController, public IIAInterface
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	void PossessSurvivorCharacter();
	void PossessHunterCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASurvivor_Craig> ThePlayerCharacter;
	UPROPERTY()
	TObjectPtr<ACharacter> _PlayerCharacter;

public:

	virtual void SetupInputComponent() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=InputMapping, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* _InputMappingContext;
	
	//Player Mechanics
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=InputMapping, meta=(AllowPrivateAccess = "true"))
	UInputAction* _Move;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=InputMapping, meta=(AllowPrivateAccess = "true"))
	UInputAction* _Sprint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=InputMapping, meta=(AllowPrivateAccess = "true"))
	UInputAction* _Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=InputMapping, meta=(AllowPrivateAccess = "true"))
	UInputAction* _Jump;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=InputMapping, meta=(AllowPrivateAccess = "true"))
	UInputAction* _Look;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=InputMapping, meta=(AllowPrivateAccess = "true"))
	UInputAction* _Crouch;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=InputMapping, meta=(AllowPrivateAccess = "true"))
	UInputAction* _Aim;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=InputMapping, meta=(AllowPrivateAccess = "true"))
	UInputAction* _Shoot;

	//Adding controller action methods
	void MoveInput(const FInputActionInstance& Instance);
	void LookInput(const FInputActionInstance& Instance);
	void ActionInput(const FInputActionInstance& Instance);
	void SprintInput(const FInputActionInstance& Instance);
	void StopSprintingInput(const FInputActionInstance& Instance);
	void CrouchInput(const FInputActionInstance& Instance);
	void StandInput(const FInputActionInstance& Instance);
	void JumpInput(const FInputActionInstance& Instance);
	void ShootInput(const FInputActionInstance& Instance);
	void AimInput(const FInputActionInstance& Instance);
};

