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
		UMeshComponent* PlayerMesh;

	// ------------ Audio components -----------------

	void TakeDamageAudio();

	class UAudioComponent* AttackAudioComponent;

	UAudioComponent* TakeDamageAudioComponent;

	UAudioComponent* JumpAudioComponent;

	UAudioComponent* PickupAudioComponent;

	UAudioComponent* DeathAudioComponent;

	UAudioComponent* PickupNoteAudioComponent;

	//Audio Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		class USoundCue* AttackingSoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		USoundCue* TakeDamageSoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		USoundCue* JumpSoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		USoundCue* PickupSoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		USoundCue* DeathSoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		USoundCue* PickupNoteSoundCue;

	// ------------------------ Do collision for items control ----------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Control")
		bool Has_Equiped;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Control")
		FString ItemPickedEquiped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Control")
		class UBoxComponent* AxeCollisionMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Control")
		class UCapsuleComponent* StickCollisonMesh;


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputsystem")
		class UInputAction* IA_DropItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputsystem")
		class UInputAction* IA_Eating;


	//Input action for combat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputsystem")
		class UInputAction* IA_Attack;

	// ------------------------ Character members hud ----------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Control")
		float Health;

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
		void JumpTrigger(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
		void AttackTrigger(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	    void ToggleInventory(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
		void DroppItemTrigger(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
		void EatingTrigger(const FInputActionValue& Value);

	// Death Control

	UFUNCTION(BlueprintCallable)
		void ToggleDeathHUD();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death Controll")
		bool Dead;


	//Movement Booleans
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
		float NegativeTimeTick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Control")
		float TimeAddingTick;

	//Stamina Control
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina Control")
		float Max_Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina Control")
		float Live_Stamina;


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

	//General Combat Control
	UFUNCTION(BlueprintCallable)
		void DamageControl();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Control")
		bool Attacking;


	//Weapon Booleans Control
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Control Axe")
		bool AxeIsActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Control Stick")
		bool StickIsActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Control Knife")
		bool KnifeIsActive;



	

	

	// ------------------------ Attaching Control ----------------------------
	UFUNCTION(BlueprintCallable)
		void EquipItem(int32 Index);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attaching Control")
		bool Equiped;

	UFUNCTION(BlueprintCallable)
		void SwapItem (int32 Index);

	UFUNCTION(BlueprintCallable)
		void DroppItem(int32 Index);


	//Hunger Control
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "hunger Control")
		float Max_Hunger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "hunger Control")
		float Live_Hunger;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hunger Control")
		bool Eating;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hunger Control")
		bool Starving;

	UFUNCTION(BlueprintCallable)
		void StarvingChecker(float HungerTick);

	UFUNCTION(BlueprintCallable,  Category = "Eating Control")
		void EatingChecker(int32 Index);


	// ------------------------ Collision Control ----------------------------
	//Collision function
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);



	// ------------------------ Interaction Control ----------------------------
	//Player Help Text
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	FString HelpText;

	//Adds Item to Inventory
	UFUNCTION(BlueprintPure, Category = "Inventory Functions")
		bool AddItemToInventory(APickUp* Item);

	//Get the thumbnail for a given inventory slot
	UFUNCTION(BlueprintPure, Category = "Inventory Functions")
		UTexture2D* GetThumbnailAtInventorySlot(int32 Slot);

	UFUNCTION(BlueprintPure, Category = "Inventory Functions")
		UTexture2D* GetDisabledThumbnailAtInventorySlot(int32 Slot);

	//Item name for a given inventory slot
	UFUNCTION(BlueprintPure, Category = "Inventory Functions")
		FString GivenItemNameAtInventorySlot(int32 Slot);

	//Notes
	UFUNCTION(BlueprintCallable, Category= "Notes")
		void ToggleNote_1(APickUp* Item);

	UFUNCTION(BlueprintCallable, Category = "Notes")
		void ToggleNote_2(APickUp* Item);

	UFUNCTION(BlueprintCallable, Category = "Notes")
		void ToggleNote_3(APickUp* Item);


	//Use Item in a given inventory slot
	/*UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
		void UseItemAtInventorySlot(int32 Slot);*/



private:
	// ------------------------ Movement Control Private ----------------------------//
	///Movement Functions
	UFUNCTION(BlueprintCallable)
		void Sprint();

	UFUNCTION(BlueprintCallable)
		void CrouchCustom();

	UFUNCTION(BlueprintCallable)
		void StaminaRecharger(float Timer);

	UFUNCTION(BlueprintCallable)
		void HungerDecay(float Timer);


	// ------------------------ Inventory and interaction Control ----------------------------
	//player reach
	float Reach;



	// Interact with the interactable (object), if there is one
	void Interact();


	//Check for interactable
	void CheckForInteractables();


	//Item the player are looking at
	AInteractable* CurrentInteractable;

public:
	//Player Inventory, represented as a TArray of pickup object.
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = ItemInArray)
		TArray<APickUp*> Inventory;

	//Array to empty slots when item has been selected
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = DisabledThumbnails)
		TArray<bool> DisabledThumbnails = { false, false, false, false, false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LocalIndexs")
		TArray<int32> LocalIndex = { 0,1,2,3,4 };


	void Die();
	

};
