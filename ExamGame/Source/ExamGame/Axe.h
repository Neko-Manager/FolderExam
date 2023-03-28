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

	//___________ Mesh Control ___________
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Weapons)
		UStaticMeshComponent* Axe;

	//___________ Collision Control ___________
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Weapons)
		class UBoxComponent* HitBox;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	//___________ Collision Control ___________
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Weapons)
	//	class AEnemy_One* Enemy_One;
};
