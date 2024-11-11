// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "AsymmetricalHuntGame/Controller/Interfaces/IAInterface.h"
#include "ThePlayerController.generated.h"

//Forward Declarations
class UInputComponent;
class ACharacter;
class ATheGameMode;

//Survivor Classes
class ASurvivor_Craig;

//Hunter Classes
class AHunter_Ghost;

UCLASS(Abstract)
class ASYMMETRICALHUNTGAME_API AThePlayerController : public APlayerController, public IIAInterface
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	//Survivor Subclasses
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASurvivor_Craig> TheSurvivorCharacter;

	//Hunter Subclasses
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AHunter_Ghost> TheHunterCharacter;

	//Character Subclass
	UPROPERTY()
	TObjectPtr<ACharacter> _PlayerCharacter;

	//GameMode Reference
	UPROPERTY()
	TObjectPtr<ATheGameMode> _TheGameMode;

public:
	
	virtual void SetupInputComponent() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=InputMapping, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* _SurvivorMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=InputMapping, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* _HunterMappingContext;
	
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
	
	//Adding controller action methods Client-Side
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
	void StopAiming(const FInputActionInstance& Instance);

	//Functions
	void PossessSurvivorCharacter();
	
	UFUNCTION(Server, Reliable)
	void S_PossessSurvivorCharacter();
	
	void PossessHunterCharacter();
	
	UFUNCTION(Server, Reliable)
	void S_PossessHunterCharacter();
};

