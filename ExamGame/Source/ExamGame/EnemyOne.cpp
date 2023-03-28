
#include "EnemyOne.h"
#include "AIController.h"

AEnemyOne::AEnemyOne()
{
	PrimaryActorTick.bCanEverTick = true;

	CombatRadius = 500.f;
	PatrolRadius = 200.f;
	PatrolDelayMax = 8.f;
	PatrolDelayMin = 2.f;
}

void AEnemyOne::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyOne::BeginPlay()
{
	Super::BeginPlay();

	// Gets the AI Controller
	EnemyController = Cast<AAIController>(GetController());

	MoveToTarget(PatrolTarget);

}

void AEnemyOne::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckPatrolTarget();
}

AActor* AEnemyOne::ChoosePatrolTarget()
{
	// Finds all patrol targets that are valid (not currently at)
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}

	// Setts a random destination based on the array of valid targets
	const int32 NumPatroltargets = ValidTargets.Num();
	if (NumPatroltargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatroltargets - 1);
		return ValidTargets[TargetSelection];
	}

	return nullptr;
}

bool AEnemyOne::InTargetRange(AActor* Target, double Radius)
{
	//Skips Function if target is a nullptr, otherwise return an input target location
	if (Target == nullptr) return false;

	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();

	return DistanceToTarget <= Radius;
}

void AEnemyOne::CheckPatrolTarget()
{
	//Checks if in range of a patrol point, if yes then wait a random time and activate the timer function.
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(PatrolDelayMin, PatrolDelayMax);
		//Sets timer in world timer manager, (timer variabel, object in question, what function to call, wait time.
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemyOne::PatrolTimerFinished, WaitTime);
	}
}

void AEnemyOne::CheckCombatTarget()
{
}

void AEnemyOne::MoveToTarget(AActor* Target)
{
	// Moves to the return value of target given by in target range
	if (EnemyController == nullptr || Target == nullptr) return;

	//Sets the move request
	FAIMoveRequest MoveRequest;

	//Sets move to goal location
	MoveRequest.SetGoalActor(Target);

	//How far away it stops from goal location
	MoveRequest.SetAcceptanceRadius(15.f);

	//Actually moves the player
	EnemyController->MoveTo(MoveRequest);
}

void AEnemyOne::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);

}














