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

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EEnemyAttackState EnemyAttackState = EEnemyAttackState::EES_EnemyUnoccupied;

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

	//UPROPERTY()
	//	class EPlayer_Character* Player{ nullptr };

	// ------------ Actors -----------------

	// First patrol point
	UPROPERTY(EditInstanceOnly, Category = "AI")
		AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI")
		AActor* CombatTarget;

	// Array of patrol points
	UPROPERTY(EditInstanceOnly, Category = "AI")
		TArray<AActor*> PatrolTargets;

	//Sets PysicsAsset as blueprint value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		TSubclassOf<AActor> BP_EnemyOnePysics;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		TSubclassOf<AActor> BP_Player;

	// ------------ Variables -----------------

	// Radius of operations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		double PatrolRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		double ChaseRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		double AttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		double RetreatRadius;

	// Speeds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float PatrolSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float ChaseSpeed;

	// Timer Delays
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float PatrolDelayMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float PatrolDelayMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float AttackDelayMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float AttackDelayMax;

	// Animation Montages

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UAnimMontage* AttackMontage;

	// Other
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		int Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		bool ReadyToAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
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

	AActor* ChoosePatrolTarget();

	bool InTargetRange(AActor* Target, float Radius);

	void CheckPatrolTarget();

	void CheckCombatTarget();

	void AttackTimerFinished();

	void PatrolTimerFinished();

	void GetRelativePos(AActor* Target);

	void MoveToTarget(AActor* Target);

	void MoveToAttackRange(AActor* Target);

	void PlayAttackMontage();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	void Attack();

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	void Die();

};