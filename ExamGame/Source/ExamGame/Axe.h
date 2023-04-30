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



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input


	//___________ Class control Axe-only ___________
	/*UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UBoxComponent* Axe;*/

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		AInteractable* AxeInteracted;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class APickUp* Item;



	//___________ Collision Control ___________



	//___________ Variables and booleans___________
	int32 AmountOfAxe;

	//___________ Functions ___________
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* BoxTraceEnd;
};
