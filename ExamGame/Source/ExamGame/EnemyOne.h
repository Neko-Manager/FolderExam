#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StateControll.h"
#include "EnemyOne.generated.h"

class USoundCue;

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

	// ------------- class Refs ------------

	UPROPERTY()
		class AAIController* EnemyController;

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
		bool HasDoneDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FVector StandingPosition;

	// ------------ Audio components -----------------

	UAudioComponent* AttackAudioComponent;

	UAudioComponent* TakeDamageAudioComponent;

	UAudioComponent* DeathAudioComponent;

	//Audio Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		USoundCue* AttackingSoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		USoundCue* TakeingDamageSoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		USoundCue* DeathDamageSoundCue;

	// ------------ Timer Handlers ---------------------

	// Timer Handle
	FTimerHandle PatrolTimer;

	// Timer Handle
	FTimerHandle AttackTimer;

	// Called from other class so must be public
	UFUNCTION()
	void TakeDamage();

protected:

	// ------------ Protected Functions --------------------

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	bool InTargetRange(AActor* Target, float Radius);

	AActor* ChoosePatrolTarget();

	void CheckPatrolTarget();

	void PatrolTimerFinished();

	void CheckCombatTarget();

	void AttackTimerFinished();

	void GetRelativePos(AActor* Target);

	void MoveToTarget(AActor* Target);

	void MoveToAttackRange(AActor* Target);

	void PlayAttackMontage();

	// Called in Anim Blueprint
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	void Attack();

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	void Die();

};