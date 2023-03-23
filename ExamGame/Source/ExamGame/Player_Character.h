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


	// ------------------------ Character control Input Actions ----------------------------
	//Camera control
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Component")
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm Component")
		class USpringArmComponent* SpringArm;

	//Input actions
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
		class UInputAction* IA_OpenInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputsystem")
		class UInputAction* IA_Interact;

	//Movement functions
	UFUNCTION(BlueprintCallable)
		void GroundedMovement(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
		void Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
		void SprintTriggered(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
		void Sprint();

	//Movement booleans
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boolean Sprinting")
		bool Sprinting;

	//Movement members
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk Speed")
		float Walk_Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint Speed")
		float Sprint_Speed;

	// ------------------------ Over time effects ----------------------------
	//Counter
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Counter Adding")
		float CounterAdding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Counter equal")
		float CounterEqual;

	//Stamina Control
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Max stamina")
		float Max_Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Live stamina")
		float Live_Stamina;

	UFUNCTION(BlueprintCallable)
		void StaminaRecharger(float Time);

	//Exhaust Control
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exhaust Boolean")
		bool Exhaust;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exhaust Timer")
		float Exhaust_Timer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exhaust Speed")
		float Exhaust_Speed;

	UFUNCTION(BlueprintCallable)
		void ExhaustChecker(float Time);

	//Hunger Control
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hunger")
		float Hunger;

	// ------------------------ Collision Control ----------------------------
	//Collision function
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);



private:
	//player reach
	float Reach;

	// Open Inventory
	void ToggleInventory();

	// Interact with the Interactable(object), if there is one.
	void Interact();

	//check for interactable
	void CheckForInteractables();


	//Player Help Text
	FString HelpText;

	//Item the player are looking at
	AInteractable* CurrInteractable;
};
