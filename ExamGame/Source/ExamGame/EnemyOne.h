// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyOne.generated.h"

UCLASS()
class EXAMGAME_API AEnemyOne : public ACharacter
{
	GENERATED_BODY()

public:
	// ------------ Constructors and main functions -----------------
	AEnemyOne();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	

	// ------------- class Refs ------------

	UPROPERTY()
		class AAIController* EnemyController;

	// ------------ Actors -----------------

	// First patrol point
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	// Array of patrol points
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	// ------------ Variables -----------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
	double CombatRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
	double PatrolRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
	float PatrolDelayMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
	float PatrolDelayMin;


	// ------------ Other ---------------------

	// Timer Handle
	FTimerHandle PatrolTimer;

protected:

	// ------------ Functions --------------------

	// Choose a target, returns and AActor
	AActor* ChoosePatrolTarget();

	bool InTargetRange(AActor* Target, double Radius);

	void CheckPatrolTarget();

	void CheckCombatTarget();

	void MoveToTarget(AActor* Target);

	void PatrolTimerFinished();











};
