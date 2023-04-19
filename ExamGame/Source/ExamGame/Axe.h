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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Weapons)
		class UBoxComponent* AxeCollisionMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Weapons)
		AInteractable* Weapon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class AEnemyOne* EnemyOne;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class APlayer_Character* Player;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class ACharacter* Character;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class APickUp* Item;

	//Player Inventory, represented as a TArray of pickup object.
	UPROPERTY(EditAnywhere)
		TArray<APickUp*> Inventory;



	//___________ Collision Control ___________
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);



	//___________ Variables ___________
	int32 AmountOfAxe;

<<<<<<< HEAD

	//___________ Functions ___________
	virtual void Use_Implementation() override;
=======
	////___________ Functions ___________
	virtual void Use_Implementation() override;
	
>>>>>>> MainBranch

private:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* BoxTraceEnd;
};
