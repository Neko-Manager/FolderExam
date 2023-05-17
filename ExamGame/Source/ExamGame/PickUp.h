// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "PickUp.generated.h"

/**
 * 
 */
UCLASS()
class EXAMGAME_API APickUp : public AInteractable
{
	GENERATED_BODY()


public:

	APickUp();


	//___________ Class control  ___________
	UPROPERTY()
		TArray<APickUp*> Inventory;

	//Functions
	virtual void BeginPlay() override;

	virtual void Interact_Implementation() override;

	void OnPickUp();


	//Uproperties
	UPROPERTY(EditAnywhere, Category = "PickUp Properties")
		UTexture2D* PickUpThumbnail;

	UPROPERTY(EditAnywhere, Category = "PickUp Properties")
		UTexture2D* DisabledPickUpThumbnail;

	UPROPERTY(EditAnywhere, Category = "PickUp Properties")
		FString ItemName;

	UPROPERTY(EditAnywhere, Category = "PickUp Properties")
		int32 Value;




	
};
