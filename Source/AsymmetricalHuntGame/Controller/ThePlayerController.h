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

UCLASS()
class ASYMMETRICALHUNTGAME_API AThePlayerController : public APlayerController, public IIAInterface
{
	GENERATED_BODY()

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<ACharacter> _PlayerCharacterPC;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=InputMapping, meta=(AllowPrivateAccess = "true"))
	UInputAction* _Interact;
	
	//Adding controller action methods Client-Side
	void MoveInput(const FInputActionInstance& Instance);
	void LookInput(const FInputActionInstance& Instance);
	void ActionInput(const FInputActionInstance& Instance);
	void StopActionInput(const FInputActionInstance& Instance);
	void SprintInput(const FInputActionInstance& Instance);
	void StopSprintingInput(const FInputActionInstance& Instance);
	void CrouchInput(const FInputActionInstance& Instance);
	void StandInput(const FInputActionInstance& Instance);
	void JumpInput(const FInputActionInstance& Instance);
	void ShootInput(const FInputActionInstance& Instance);
	void AimInput(const FInputActionInstance& Instance);
	void StopAiming(const FInputActionInstance& Instance);
	void Interact(const FInputActionInstance& Instance);

	UFUNCTION(Server, Reliable, WithValidation)
	void S_MoveInput(const FVector _PlayerInput);
	UFUNCTION(Server, Reliable, WithValidation)
	void S_LookInput(const FVector _PlayerInput);
	UFUNCTION(Server, Reliable, WithValidation)
	void S_ActionInput(const FInputActionInstance& Instance);
	UFUNCTION(Server, Reliable, WithValidation)
	void S_StopActionInput(const FInputActionInstance& Instance);
	UFUNCTION(Server, Reliable, WithValidation)
	void S_SprintInput(const FInputActionInstance& Instance);
	UFUNCTION(Server, Reliable, WithValidation)
	void S_StopSprintingInput(const FInputActionInstance& Instance);
	UFUNCTION(Server, Reliable, WithValidation)
	void S_CrouchInput(const FInputActionInstance& Instance);
	UFUNCTION(Server, Reliable, WithValidation)
	void S_StandInput(const FInputActionInstance& Instance);
	UFUNCTION(Server, Reliable, WithValidation)
	void S_JumpInput(const FInputActionInstance& Instance);
	UFUNCTION(Server, Reliable, WithValidation)
	void S_ShootInput(const FInputActionInstance& Instance);
	UFUNCTION(Server, Reliable, WithValidation)
	void S_AimInput(const FInputActionInstance& Instance);
	UFUNCTION(Server, Reliable, WithValidation)
	void S_StopAiming(const FInputActionInstance& Instance);
	UFUNCTION(Server, Reliable, WithValidation)
	void S_Interact(const FInputActionInstance& Instance);

	UFUNCTION(BlueprintCallable)
	void PC_SpawnCharacters(ATheGameMode* _GameModeRef);
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnCharacters();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isHunter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isSurvivor;

	
};

