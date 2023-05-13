
//Class Include
#include "EnemyTwo.h"
#include "AIController.h"
#include "Player_Character.h"

//Component Include
#include "perception/PawnSensingComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"

//Other Include
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AEnemyTwo::AEnemyTwo()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 1000.f;
	PawnSensing->SetPeripheralVisionAngle(360.f);


	// Radius of operations
	ChaseRadius = 2000.f;
	AttackRadius = 100.f;
	DetectionRangeX = 1000;
	DetectionRangeY = 1000;
	DetectionRangeZ = 150;

	DetectionSquare = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	DetectionSquare->SetupAttachment(GetRootComponent());
	DetectionSquare->InitBoxExtent(FVector(DetectionRangeX, DetectionRangeY, DetectionRangeZ));
	DetectionSquare->OnComponentBeginOverlap.AddDynamic(this, &AEnemyTwo::OnSquareDetect);

	// Speeds
	ChaseSpeed = 300.f;

	// Timer Delay
	AttackDelayMax = 4.f;
	AttackDelayMin = 1.f;

	// Other
	Health = 20;

	Burrowed = true;

}

void AEnemyTwo::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyTwo::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->SetVisibility(false);

	// Gets the AI Controller
	EnemyController = Cast<AAIController>(GetController());

	EnemyState = EEnemyState::EES_EnemyIdle;
	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemyTwo::PawnSeen);
	}
}

void AEnemyTwo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyState == EEnemyState::EES_EnemyChaseing)
	{
		CheckCombatTarget();
	}
	
	if (Health <= 0)
	{
		Die();
	}
}

void AEnemyTwo::PawnSeen(APawn* SeenPawn)
{
	////Stops checking for pawn seen if still chasing the player
	
	
	//if (EnemyState == EEnemyState::EES_EnemyChaseing) return;

	//if (SeenPawn->ActorHasTag(FName("PlayerCharacter")) && Burrowed == false)
	//{
	//	CombatTarget = SeenPawn;

	//	//Sets State to chasing the player Character
	//	EnemyState = EEnemyState::EES_EnemyChaseing;
	//	MoveToTarget(CombatTarget);
	//	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("PawnSeen, Chase player")));
	//}
}

bool AEnemyTwo::InTargetRange(AActor* Target, float Radius)
{
	//Skips Function if target is a nullptr, otherwise return an input target location
	if (Target == nullptr) return false;

	const float DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();

	return DistanceToTarget <= Radius;
}

void AEnemyTwo::CheckCombatTarget()
{
	if (!InTargetRange(CombatTarget, ChaseRadius))
	{
		// When outside of chase radius Stay where it is
		CombatTarget = nullptr;
		EnemyState = EEnemyState::EES_EnemyIdle;
		StayAtPosition(GetActorLocation());

		GetMesh()->SetVisibility(false);
		Burrowed = true;

		//PLAY BURROW ANIM MONTAGE

		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Loose intrest")));
	}
	else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_EnemyChaseing)
	{
		// Outside attack range chase character
		EnemyState = EEnemyState::EES_EnemyChaseing;
		MoveToTarget(CombatTarget);


		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Chasing player")));
	}
	else if (InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_EnemyAttacking)
	{
		// inside attack range, attack character.
		EnemyState = EEnemyState::EES_EnemyAttacking;

		Die();

		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Attack player")));

		//This is where attack montage is put.
	}
}

void AEnemyTwo::MoveToTarget(AActor* Target)
{
	// Moves to the return value of target given by in target range
	if (EnemyController == nullptr || Target == nullptr) return;

	//Sets the move request
	FAIMoveRequest MoveRequest;

	//Sets move to goal location
	MoveRequest.SetGoalActor(Target);

	//How far away it stops from goal location
	MoveRequest.SetAcceptanceRadius(15.f);

	//Set focus
	EnemyController->SetFocus(Target);

	//Actually moves the player
	EnemyController->MoveTo(MoveRequest);
}

void AEnemyTwo::StayAtPosition(FVector Location)
{
	// Moves to the return value of target given by in target range
	if (EnemyController == nullptr) return;

	//Sets the move request
	FAIMoveRequest MoveRequest;

	//Sets move to goal location
	MoveRequest.SetGoalLocation(Location);

	//How far away it stops from goal location
	MoveRequest.SetAcceptanceRadius(15.f);

	//Actually moves the player
	EnemyController->MoveTo(MoveRequest);

}

void AEnemyTwo::OnSquareDetect(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayer_Character* Player = Cast<APlayer_Character>(OtherActor);

	if (Player && Player->ActorHasTag(FName("PlayerCharacter")) && Burrowed == true)
	{
		EnemyState = EEnemyState::EES_EnemyChaseing;
		GetMesh()->SetVisibility(true);
		Burrowed = false;
		CombatTarget = OtherActor;
		MoveToTarget(CombatTarget);

		// PLAY UNBURROW ANIM MONTAGE

		GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Yellow, FString::Printf(TEXT("Sees player")));
	}

}

void AEnemyTwo::Die()
{
	//Spawns The pysics asset when it has died
	GetWorld()->SpawnActor<AActor>(BP_EnemyTwoPysics, GetActorTransform());

	//Removes Original mesh
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	this->Destroy();
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Died")));
}














