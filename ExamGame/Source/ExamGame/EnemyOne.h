#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StateControll.h"
#include "EnemyOne.generated.h"

UCLASS()
class EXAMGAME_API AEnemyOne : public ACharacter
{
	GENERATED_BODY()

private:

	// ------------ States ---------------------

	// Sets the default state to Idle, from the get AIState beneath.
	EEnemyState EnemyState = EEnemyState::EES_EnemyPatrolling;

	// Gets the AI state from the state controller
	FORCEINLINE EEnemyState GetAIState() const { return EnemyState; }

public:
	// ------------ Constructors and main functions -----------------
	AEnemyOne();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// ------------- Components ------------

	UPROPERTY(VisibleAnywhere)
		class UPawnSensingComponent* PawnSensing;

	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* Colision;

	// ------------- class Refs ------------

	UPROPERTY()
		class AAIController* EnemyController;

	// ------------ Actors -----------------

	// First patrol point
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		AActor* CombatTarget;

	// Array of patrol points
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		TArray<AActor*> PatrolTargets;

	// ------------ Variables -----------------

	// Radius of operations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		double PatrolRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		double ChaseRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		double AttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		double RetreatRadius;

	// Speeds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		float PatrolSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		float ChaseSpeed;

	// Timer Delays
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		float PatrolDelayMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		float PatrolDelayMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		float AttackDelayMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		float AttackDelayMax;

	// Other
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		int Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		bool ReadyToAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")

	float AcceptanceRadius;

		FVector StandingPosition;


	// ------------ Other ---------------------

	// Timer Handle
	FTimerHandle PatrolTimer;

	// Timer Handle
	FTimerHandle AttackTimer;

protected:

	// ------------ Functions --------------------

	UFUNCTION()
		void PawnSeen(APawn* SeenPawn);

	// Choose a target, returns and AActor
	AActor* ChoosePatrolTarget();

	bool InTargetRange(AActor* Target, float Radius);

	void CheckPatrolTarget();

	void CheckCombatTarget();

	void AttackTimerFinished();

	void PatrolTimerFinished();

	void Timer(float deltatime);

	void GetRelativePos(AActor* Target);

	void MoveToTarget(AActor* Target);

	void MoveToAttackRange(AActor* Target);

	void Die();

};
