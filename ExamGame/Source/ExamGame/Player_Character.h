// Fill out your copyright notice in the Description page of Project Settings.

//Other
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/AudioComponent.h"
#include "Engine/DataTable.h"

//classes
#include "PickUp.h"

//Class generator

#include "Player_Character.generated.h"
//_______________________________________________________________________________
USTRUCT(BlueprintType)
struct FCraftingInfo : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ComponentID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ProductID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDestroyItemA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDestroyItemB;
};

//_______________________________________________________________________________
USTRUCT(BlueprintType)
struct FInventoryItem : public FTableRowBase
{

	GENERATED_BODY()

public:

	FInventoryItem()
	{
		Name = FText::FromString("Item");
		Action = FText::FromString("Use");
		Description = FText::FromString("Please enter a description for this item");
		Value = 10;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class APickUp> ItemPickup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FCraftingInfo> CraftCombinations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanBeUsed;

	bool operator==(const FInventoryItem& Item) const
	{
		if (ItemID == Item.ItemID)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};
//_______________________________________________________________________________

UCLASS(config = Game)

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AEnemyOne* EnemyOne;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AAxe* Axe;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AGamePlayerController* FromPlayerController;*/


	// ------------------------ Do collision for items control ----------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Control")
		bool Has_Equiped;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Control")
		FName ItemPickedEquiped;


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
		class UInputAction* IA_DropItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputsystem")
		class UInputAction* IA_Eating;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputsystem")
		class UInputAction* IA_Interaction;



	//Input action for combat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputsystem")
		class UInputAction* IA_AxeAttack;

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
		void AxeAttackTrigger(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	    void ToggleInventory(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
		void DroppItemTrigger(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
		void EatingTrigger(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
		void InteractTrigger();


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

	//Tracing control
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tracing Control")
		float Reach;


	// ------------------------ Combat Control ----------------------------

	//Axe
	UFUNCTION(BlueprintCallable)
		void Attack();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Control Axe")
		bool Attacking;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Control Axe")
		bool AxeActive;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Control Axe")
		float Axe_Timer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Control Axe")
		float Axe_ActiveFrames;

	

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hunger Control")
		float Max_Hunger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hunger Control")
		float Live_Hunger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hunger Control")
		bool Eating;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hunger Control")
		bool Starving;

	UFUNCTION(BlueprintCallable)
		void StarvingChecker(float HungerTick);

	UFUNCTION(BlueprintCallable)
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

protected:
	//Check for interactable
	void CheckForInteractables();

	


};
