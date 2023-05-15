#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StateControll.h"
#include "EnemyTwo.generated.h"

UCLASS()
class EXAMGAME_API AEnemyTwo : public ACharacter
{
	GENERATED_BODY()

private:

	// ------------ States ---------------------

	// Sets the default state to Idle, from the get AIState beneath.
	EEnemyState EnemyState = EEnemyState::EES_EnemyPatrolling;
	EEnemyAttackState EnemyAttackState = EEnemyAttackState::EES_EnemyUnoccupied;

	// Gets the AI state from the state controller
	FORCEINLINE EEnemyState GetAIState() const { return EnemyState; }

public:
	// ------------ Constructors and main functions -----------------
	AEnemyTwo();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// ------------- Components -----------

	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* Colision;

	// ------------- class Refs ------------

	UPROPERTY()
		class AAIController* EnemyController;

	// ------------ Actors -----------------

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		AActor* CombatTarget;

	//Sets PysicsAsset as blueprint value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		TSubclassOf<AActor> BP_EnemyTwoPysics;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		class UBoxComponent* DetectionSquare;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		class UNiagaraSystem* VFXBurrow;

	// Animation Montages
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UAnimMontage* AttackMontage;

	// ------------ Variables -----------------

	// Radius of operations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		double ChaseRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		double AttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		double DetectionRangeX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		double DetectionRangeY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		double DetectionRangeZ;

	// Speeds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		float ChaseSpeed;

	// Timer Delays

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		float AttackDelayMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		float AttackDelayMax;

	// Other
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		int Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		bool Burrowed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		bool HasDoneDamage;

	// ------------ Other ---------------------

protected:

	// ------------ Functions --------------------

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	bool InTargetRange(AActor* Target, float Radius);

	void CheckCombatTarget();

	void MoveToTarget(AActor* Target);

	void StayAtPosition(FVector Location);

	void PlayBurrow(bool isVisible, bool isBurrowed);

	void PlayAttackMontage();

	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	void Die();

	UFUNCTION()
		void OnPlayerDetect(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);
};