// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include  "Player_Character.h"


//Generators
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EXAMGAME_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//classes
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class APlayer_Character* ThisPlayer;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		class AInteractable* CurrentInteractable;

	//Functions
	UFUNCTION(BlueprintCallable)
		void Interact();

	UFUNCTION(BlueprintImplementableEvent)
		void ReloadInventory();

	UFUNCTION(BlueprintCallable, Category = "Utils")
		void AddItemToInventoryByID(FName ID);

	UFUNCTION(BlueprintCallable, Category = "Utils")
		void CraftItem(FInventoryItem ItemA, FInventoryItem ItemB, APlayerController* FromPlayerController);

	//Bolleans
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact Control")
		bool CanInteract;


	//This is the Interactable the player is looking at, if player is not looking at an interactable this is equal to a nullptr.
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		TArray<FInventoryItem> Inventory;


};
