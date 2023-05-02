//Class Include
#include "EnemyOne.h"
#include "EnemyOne_Physics.h"
#include "Player_Character.h"
#include "AIController.h"

//Component Include
#include "perception/PawnSensingComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

//Other Include
#include "Kismet/KismetSystemLibrary.h"
#include "Animation/AnimMontage.h"

AEnemyOne::AEnemyOne()
{
	PrimaryActorTick.bCanEverTick = true;

	// Sensing Component init
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 3000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);

	// Collision on overlap init
	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &AEnemyOne::OnOverlap);

	// Radius of operations
	PatrolRadius = 200.f;
	ChaseRadius = 2000.f;
	AttackRadius = 400.f;
	RetreatRadius = 100.f;

	// Speeds
	PatrolSpeed = 400.f;
	ChaseSpeed = 800.f;

	// Timer Delays
	PatrolDelayMax = 8.f;
	PatrolDelayMin = 2.f;

	AttackDelayMax = 4.f;
	AttackDelayMin = 1.f;

	// Other
	Health = 10;
	ReadyToAttack = false;
	HasDoneDamage = false;
	StandingPosition = FVector(0.f, 0.f, 0.f);
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

	// Init pawn sensing overlap
	if (PawnSensing)
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemyOne::PawnSeen);

	// Moves to first patrol
	if (PatrolTarget)
		MoveToTarget(PatrolTarget);
}

void AEnemyOne::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If in combat state
	if (EnemyState == EEnemyState::EES_EnemyChaseing || 
		EnemyState == EEnemyState::EES_EnemyAttacking || 
		EnemyState == EEnemyState::EES_EnemyReadyToAttack)
	{
		//Gets the targets relative position for attack location
		if (CombatTarget != nullptr)
			GetRelativePos(CombatTarget);

		//Checks combat range and updates state accordingly
		CheckCombatTarget();
	}

	// if in patrol state checks patrol ranges and updates accordingly
	else
		CheckPatrolTarget();

	// Checks if in closest range to attack then retreat
	if (InTargetRange(CombatTarget, RetreatRadius) && CombatTarget != nullptr)
	{
		GetWorldTimerManager().ClearTimer(AttackTimer);
		ReadyToAttack = false;
		Attack();
		//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Attack Then Leave")));
	}

	// If health is 0 enemy dies
	if (Health <= 0)
		Die();

}

void AEnemyOne::PawnSeen(APawn* SeenPawn)
{
	//Stops checking for pawn seen if player already has been seen by returning function
	if (EnemyState == EEnemyState::EES_EnemyChaseing || EnemyState == EEnemyState::EES_EnemyAttacking || EnemyState == EEnemyState::EES_EnemyReadyToAttack) return;

	//Activates When player in LOS of enemy
	if (SeenPawn->ActorHasTag(FName("PlayerCharacter")))
	{
		//Stops the Patrolling function timer to prioritize chasing.
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;

		//Sets State to chasing the player Character
		CombatTarget = SeenPawn;
		EnemyState = EEnemyState::EES_EnemyChaseing;
		MoveToTarget(CombatTarget);
		//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("PawnSeen, Chase player")));
	}
}

bool AEnemyOne::InTargetRange(AActor* Target, float Radius)
{
	//Skips Function if target is a nullptr, otherwise return an input target location
	if (Target == nullptr) return false;

	//Gets the distance between target and self
	const float DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();

	//returns true if >= Radius
	return DistanceToTarget <= Radius;
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

void AEnemyOne::CheckPatrolTarget()
{
	//Checks if in range of a patrol point, if yes then wait a random time and move to next.
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(PatrolDelayMin, PatrolDelayMax);
		//Sets timer in world timer manager, (timer variable, object in question, what function to call, wait time.
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemyOne::PatrolTimerFinished, WaitTime);
	}
}

void AEnemyOne::PatrolTimerFinished()
{
	//Moves to next patrol target
	MoveToTarget(PatrolTarget);
}

void AEnemyOne::CheckCombatTarget()
{
	//Checks; if outside chase Radius go back to patrol
	if (!InTargetRange(CombatTarget, ChaseRadius))
	{
		CombatTarget = nullptr;
		EnemyState = EEnemyState::EES_EnemyPatrolling;
		GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
		MoveToTarget(PatrolTarget);

		//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Loose intrest")));
	}
	//Checks; if outside attack range chase player to attack range
	else if (InTargetRange(CombatTarget, ChaseRadius) && EnemyState != EEnemyState::EES_EnemyAttacking)
	{
		EnemyState = EEnemyState::EES_EnemyChaseing;
		GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
		MoveToAttackRange(CombatTarget);

		//GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, FString::Printf(TEXT("Chasing player")));
	}
	//Checks; if inside attack range, start attack timer
	if (InTargetRange(CombatTarget, AttackRadius) && ReadyToAttack == false)
	{
		const float WaitTime = FMath::RandRange(AttackDelayMin, AttackDelayMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemyOne::AttackTimerFinished, WaitTime);
		ReadyToAttack = true;

		//GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, FString::Printf(TEXT("Attack Timer Started")));
	}
}

void AEnemyOne::AttackTimerFinished()
{
	// When random attack timer is up attack target
	EnemyState = EEnemyState::EES_EnemyAttacking;
	EnemyAttackState = EEnemyAttackState::EES_EnemyUnoccupied;
	HasDoneDamage = false;
	MoveToTarget(CombatTarget);
	//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Attacking")));
}


void AEnemyOne::GetRelativePos(AActor* Target)
{
	//Skips Function if target is a nullptr, otherwise return target location
	if (Target != nullptr)
	{
		//Gets the enemies closest location for positioning
		// Firs Quadrant
		if (GetActorLocation().X >= Target->GetActorLocation().X && GetActorLocation().Y >= Target->GetActorLocation().Y) {
			StandingPosition = FVector(180, 180, 0);
		}
		// Second Quadrant
		else if (GetActorLocation().X <= Target->GetActorLocation().X && GetActorLocation().Y >= Target->GetActorLocation().Y) {
			StandingPosition = FVector(-180, 180, 0);
		}
		// Third Quadrant
		else if (GetActorLocation().X <= Target->GetActorLocation().X && GetActorLocation().Y <= Target->GetActorLocation().Y) {
			StandingPosition = FVector(-180, -180, 0);
		}
		// Fourth Quadrant
		else if (GetActorLocation().X >= Target->GetActorLocation().X && GetActorLocation().Y <= Target->GetActorLocation().Y) {
			StandingPosition = FVector(180, -180, 0);
		}
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
	MoveRequest.SetAcceptanceRadius(25.f);

	//Actually moves the player
	EnemyController->MoveTo(MoveRequest);
}

void AEnemyOne::MoveToAttackRange(AActor* Target)
{
	// Skips function if there is no enemy controller or target
	if (EnemyController == nullptr || Target == nullptr) return;

	//Sets the move request
	FAIMoveRequest MoveRequest;

	//Sets The Target location based on enemy position and target position
	MoveRequest.SetGoalLocation((Target->GetActorLocation() + StandingPosition));
	//Acceptable precision of range
	MoveRequest.SetAcceptanceRadius(10.f);

	//Sets player as rotation focus and moves according to move request
	EnemyController->SetFocus(Target);
	EnemyController->MoveTo(MoveRequest);
}

void AEnemyOne::PlayAttackMontage()
{
	//PLays the animation montage relating to connected Animation montage blue print
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Montage Play")));
		AnimInstance->Montage_Play(AttackMontage);
		FName SectionName = FName("Attack1");
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void AEnemyOne::AttackEnd()
{
	//Changes state when Attack over, called in Enemy Blueprint
	EnemyState = EEnemyState::EES_EnemyChaseing;
}

void AEnemyOne::Attack()
{
	//Plays attack montage according to the state controller
	if(EnemyAttackState == EEnemyAttackState::EES_EnemyUnoccupied)
	{
		PlayAttackMontage();
		EnemyAttackState = EEnemyAttackState::EES_EnemyAttacking;
	}
}

void AEnemyOne::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Detects overlap between player and enemy during attack animation for single damage done cast to player class. 

	APlayer_Character* Player = Cast<APlayer_Character>(OtherActor);

	if (Player && Player->ActorHasTag(FName("PlayerCharacter")) && EnemyAttackState == EEnemyAttackState::EES_EnemyAttacking)
	{
		if (HasDoneDamage == false)
		{
			Player->Health -= 10;
			HasDoneDamage = true;
			//GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Yellow, FString::Printf(TEXT("PlayerTakesDamge")));
		}
	}
}

void AEnemyOne::Die()
{
	//Spawns Physics Actor on enemy position at time of death, BP specified physics
	GetWorld()->SpawnActor<AActor>(BP_EnemyOnePysics, GetActorTransform());

	//Removes enemy character components
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	this->Destroy();

	//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Died")));
}
