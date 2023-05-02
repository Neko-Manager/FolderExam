// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUp.h"
#include "Axe.generated.h"

/**
 * 
 */
UCLASS()
class EXAMGAME_API AAxe : public APickUp
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAxe();

	/*UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UBoxComponent* AxeCollisionMesh;*/


public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		AInteractable* AxeInteracted;

	//UFUNCTION()
	//void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	//	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	//	bool bFromSweep, const FHitResult& SweepResult);

};
