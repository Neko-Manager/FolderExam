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

	// Gets the AI state from the state controller
	FORCEINLINE EEnemyState GetAIState() const { return EnemyState; }

public:
	// ------------ Constructors and main functions -----------------
	AEnemyTwo();
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

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		AActor* CombatTarget;

	//Sets PysicsAsset as blueprint value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		TSubclassOf<AActor> BP_EnemyTwoPysics;

	// ------------ Variables -----------------

	// Radius of operations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		double ChaseRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation")
		double AttackRadius;

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

	// ------------ Other ---------------------

protected:

	// ------------ Functions --------------------

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	void IsInRange();

	bool InTargetRange(AActor* Target, float Radius);

	void CheckCombatTarget();

	void MoveToTarget(AActor* Target);

	void StayAtPosition(FVector Location);

	void Die();
};