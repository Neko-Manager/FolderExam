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

	virtual void Use_Implementation();

	void BackToInventory();

	void OnPickUp();

	UFUNCTION()
		virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);


	//Uproperties
	UPROPERTY(EditAnywhere, Category = "PickUp Properties")
		UTexture2D* PickUpThumbnail;

	UPROPERTY(EditAnywhere, Category = "PickUp Properties")
		FString ItemName;

	UPROPERTY(EditAnywhere, Category = "PickUp Properties")
		int32 Value;

	//Call When The Item Is picked Up; Clean Up After.



	
};
