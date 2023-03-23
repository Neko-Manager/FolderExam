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

	virtual void BeginPlay() override;

	virtual void Interact_Implementation() override;

	UFUNCTION(BlueprintNativeEvent)
	void Use();
	virtual void Use_Implementation();

	UPROPERTY(EditAnywhere, Category = "PickUp Properties")
		UTexture2D* PickUpTexture;

	UPROPERTY(EditAnywhere, Category = "PickUp Properties")
		FString ItemName;

	UPROPERTY(EditAnywhere, Category = "PickUp Properties")
		int32 Value;

	//Call When The Item Is picked Up; Clean Up After.

	void OnPickUp();
	
};