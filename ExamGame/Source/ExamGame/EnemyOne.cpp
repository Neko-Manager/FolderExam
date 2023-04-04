
//Class Include
#include "EnemyOne.h"
#include "AIController.h"

//Component Include
#include "perception/PawnSensingComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

//Other Include
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AEnemyOne::AEnemyOne()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);

	AttackRadius = 150.f;
	ChaseRadius = 2000.f;
	PatrolRadius = 200.f;
	PatrolDelayMax = 8.f;
	PatrolDelayMin = 2.f;
	Health = 10;
	PatrolSpeed = 125.f;
	ChaseSpeed = 300.f;
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

	if(PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemyOne::PawnSeen);
	}
	if(PatrolTarget)
	{
		MoveToTarget(PatrolTarget);
	}
	
}

void AEnemyOne::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(EnemyState == EEnemyState::EES_EnemyChaseing || EnemyState == EEnemyState::EES_EnemyAttacking)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
	if(Health <= 0)
	{
		Die();
	}
}

void AEnemyOne::PawnSeen(APawn* SeenPawn)
{
	//Stops checking for pawn seen if still chasing the player
	if (EnemyState == EEnemyState::EES_EnemyChaseing) return;

	if(SeenPawn->ActorHasTag(FName("PlayerCharacter")))
	{
		//Stops the Patrolling function timer to prioritse chasing.
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
		CombatTarget = SeenPawn;

		if (EnemyState != EEnemyState::EES_EnemyAttacking)
		{
			//Sets State to chasing the player Character
			EnemyState = EEnemyState::EES_EnemyChaseing;
			MoveToTarget(CombatTarget);
			GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("PawnSeen, Chase player")));
		}
	}
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

bool AEnemyOne::InTargetRange(AActor* Target, float Radius)
{
	//Skips Function if target is a nullptr, otherwise return an input target location
	if (Target == nullptr) return false;

	const float DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	
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
	if (!InTargetRange(CombatTarget, ChaseRadius))
	{
		// When outside of chase radius go back to patrol
		CombatTarget = nullptr;
		EnemyState = EEnemyState::EES_EnemyPatrolling;
		GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
		MoveToTarget(PatrolTarget);

		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Loose intrest")));
	}
	else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_EnemyChaseing)
	{
		// Outside attack range chase character
		EnemyState = EEnemyState::EES_EnemyChaseing;
		GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
		MoveToTarget(CombatTarget);
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Chasing player")));
	}
	else if(InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_EnemyAttacking)
	{
		// inside attack range, attack character.
		EnemyState = EEnemyState::EES_EnemyAttacking;

		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Attack player")));

		//This is where attack montage is put.
	}
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

void AEnemyOne::Die()
{
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//SetLifeSpan(3.f);
}














