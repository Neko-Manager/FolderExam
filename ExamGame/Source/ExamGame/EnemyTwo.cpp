
//Class Include
#include "EnemyTwo.h"
#include "AIController.h"
#include "Player_Character.h"

//Niagara Include
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

//Component Include
#include "perception/PawnSensingComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"

//Other Include
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimMontage.h"

AEnemyTwo::AEnemyTwo()
{
	PrimaryActorTick.bCanEverTick = true;

	

	// Radius of operations
	ChaseRadius = 2000.f;
	AttackRadius = 100.f;
	DetectionRangeX = 1000.f;
	DetectionRangeY = 1000.f;
	DetectionRangeZ = 100.f;

	//Initilizations
	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &AEnemyTwo::OnOverlap);

	DetectionSquare = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	DetectionSquare->SetupAttachment(GetRootComponent());
	DetectionSquare->SetRelativeLocation(FVector(0.f, 0.f, DetectionRangeZ));
	DetectionSquare->InitBoxExtent(FVector(DetectionRangeX, DetectionRangeY, DetectionRangeZ));
	DetectionSquare->OnComponentBeginOverlap.AddDynamic(this, &AEnemyTwo::OnPlayerDetect);

	// Speeds
	ChaseSpeed = 300.f;

	// Timer Delay
	AttackDelayMax = 4.f;
	AttackDelayMin = 1.f;

	// Other
	Health = 20;
	Burrowed = true;
	HasDoneDamage = false;
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
	EnemyAttackState = EEnemyAttackState::EES_EnemyUnoccupied;
	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;

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

		PlayBurrow(false,true);

		//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Loose intrest")));
	}
	else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_EnemyChaseing)
	{
		// Outside attack range chase character
		EnemyState = EEnemyState::EES_EnemyChaseing;
		MoveToTarget(CombatTarget);
		AttackEnd();

		//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Chasing player")));
	}
	else if (InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_EnemyAttacking)
	{
		// inside attack range, attack character.
		EnemyState = EEnemyState::EES_EnemyAttacking;

		Attack();
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Attack")));
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

void AEnemyTwo::PlayBurrow(bool isVisible,bool isBurrowed)
{
	GetMesh()->SetVisibility(isVisible);
	Burrowed = isBurrowed;

	if (VFXBurrow) {
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, VFXBurrow, (GetActorLocation() + (GetActorForwardVector() * 2 + FVector(0.f, 0.f, -20.f))),
			GetActorRotation(), FVector(2.f), true,
			true, ENCPoolMethod::None, true);


		//PLAY BURROW ANIM MONTAGE

	}
}

void AEnemyTwo::PlayAttackMontage()
{
	//PLays the animation montage relating to connected Animation montage blue print
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance == nullptr)
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("No AnimInstance")));

	if(AttackMontage == nullptr)
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("No AttackMontage")));

	if (AnimInstance && AttackMontage)
	{
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Montage Play")));
		AnimInstance->Montage_Play(AttackMontage);
		FName SectionName = FName("Attack1");
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void AEnemyTwo::Attack()
{
	//Plays attack montage according to the state controller
	if (EnemyAttackState == EEnemyAttackState::EES_EnemyUnoccupied)
	{
		PlayAttackMontage();
		EnemyAttackState = EEnemyAttackState::EES_EnemyAttacking;
	}
}

void AEnemyTwo::AttackEnd()
{
	//Changes state when Attack over, called in Anim blueprint
	EnemyState = EEnemyState::EES_EnemyChaseing;
	EnemyAttackState = EEnemyAttackState::EES_EnemyUnoccupied;
	HasDoneDamage = false;
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

void AEnemyTwo::OnPlayerDetect(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayer_Character* Player = Cast<APlayer_Character>(OtherActor);

	if (Player && Player->ActorHasTag(FName("PlayerCharacter")) && Burrowed == true)
	{
		EnemyState = EEnemyState::EES_EnemyChaseing;
		PlayBurrow(true, false);
		CombatTarget = OtherActor;
		MoveToTarget(CombatTarget);

		//GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Yellow, FString::Printf(TEXT("Detects player")));
	}

}

void AEnemyTwo::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Detects overlap between player and enemy during attack animation for single damage done cast to player class. 

	APlayer_Character* Player = Cast<APlayer_Character>(OtherActor);

	if (Player && Player->ActorHasTag(FName("PlayerCharacter")) && EnemyAttackState == EEnemyAttackState::EES_EnemyAttacking)
	{
		if (HasDoneDamage == false)
		{
			Player->Health -= 10;
			HasDoneDamage = true;
			GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Yellow, FString::Printf(TEXT("PlayerTakesDamge")));
		}
	}

}













