// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/AudioComponent.h"
#include "Interactable.h"
#include "PickUp.h"
#include "Player_Character.generated.h"

UCLASS()
class EXAMGAME_API APlayer_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayer_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// ------------------------ Class references ----------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AAxe* Axe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AEnemyOne* EnemyOne;



	// ------------------------ Character control Input Actions ----------------------------
	//Camera control
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Component")
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm Component")
		class USpringArmComponent* SpringArm;

	//Input actions for movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputsystem")
		class UInputMappingContext* IMC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputsystem")
		class UInputAction* IA_GroundMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputsystem")
		class UInputAction* IA_Look;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputsystem")
		class UInputAction* IA_Jump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputsystem")
		class UInputAction* IA_Sprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputsystem")
		class UInputAction* IA_Crouch;

	//Input actions fro inventory and interact
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputsystem")
		class UInputAction* IA_OpenInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputsystem")
		class UInputAction* IA_Interact;

	//Input action for combat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputsystem")
		class UInputAction* IA_AxeAttack;

	//Input Triggers
	UFUNCTION(BlueprintCallable)
		void GroundedMovement(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
		void Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
		void SprintTriggered(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
		void CrouchTriggered(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
		void AxeAttackTrigger(const FInputActionValue& Value);

	//Functions for Input
	UFUNCTION(BlueprintCallable)
		void Sprint();

	UFUNCTION(BlueprintCallable)
		void CrouchCustom();


	//Movement booleans
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Boolean")
		bool Sprinting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Boolean")
		bool Crouching;

	
	//Movement Speed Control
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Speed Control")
		float Walk_Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Speed Control")
		float Sprint_Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Speed Control")
		float Crouch_Speed;

	// ------------------------ Over time effects ----------------------------
	//Time Control
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Control")
		float Counter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Control")
		float TimeTick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Control")
		float TimeAddingTick;

	//Stamina Control
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina Control")
		float Max_Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina Control")
		float Live_Stamina;

	UFUNCTION(BlueprintCallable)
		void StaminaRecharger(float Timer);

	//Exhaust Control
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exhaust Control")
		bool Exhaust;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exhaust Control")
		float Exhaust_Timer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exhaust Control")
		float Exhaust_Speed;

	UFUNCTION(BlueprintCallable)
		void ExhaustChecker(float Stamina);

	// ------------------------ Combat Control ----------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Control")
		bool AxeActive;

	UFUNCTION(BlueprintCallable)
		void ResetAxeAttack();

	
	//Hunger Control
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hunger")
		float Hunger;

	// ------------------------ Collision Control ----------------------------
	//Collision function
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);


	//Player Help Text
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	FString HelpText;

	//Adds Item to Inventory
	UFUNCTION(BlueprintPure, Category = "Inventory Functions")
		bool AddItemToInventory(APickUp* Item);

	//Get the thumbnail for a given inventory slot
	UFUNCTION(BlueprintPure, Category = "Inventory Functions")
		UTexture2D* GetThumbnailAtInventorySlot(int32 Slot);

	//Item name for a given inventory slot
	UFUNCTION(BlueprintPure, Category = "Inventory Functions")
		FString GivenItemNameAtInventorySlot(int32 Slot);

	//Use Item in a given inventory slot
	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
		void UseItemAtInventorySlot(int32 Slot);

private:
	// ------------------------ Inventory and interaction Control ----------------------------
	//player reach
	float Reach;

	// Open Inventory
	void ToggleInventory();

	// Interact with the interactable (object), if there is one
	void Interact();

	//Check for interactable
	void CheckForInteractables();

	//Item the player are looking at
	AInteractable* CurrentInteractable;

	//Player Inventory, represented as a TArray of pickup object.
	UPROPERTY(EditAnywhere)
		TArray<APickUp*> Inventory;


};
