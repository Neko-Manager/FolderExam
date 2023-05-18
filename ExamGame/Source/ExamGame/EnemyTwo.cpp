
//Class Include
#include "EnemyTwo.h"
#include "AIController.h"
#include "Player_Character.h"

//Niagara Include
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

//Component Include
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

//Other Include
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimMontage.h"

AEnemyTwo::AEnemyTwo()
{
	PrimaryActorTick.bCanEverTick = true;

	// Radius of operations
	ChaseRadius = 2000.f;
	AttackRadius = 150.f;
	DetectionRangeX = 1000.f;
	DetectionRangeY = 1000.f;
	DetectionRangeZ = 100.f;

	AtttackColliderSquare = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackColider"));
	AtttackColliderSquare->SetupAttachment(GetMesh(),"Tail_06th");

	DetectionSquare = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	DetectionSquare->SetupAttachment(GetRootComponent());
	DetectionSquare->SetRelativeLocation(FVector(0.f, 0.f, DetectionRangeZ));
	DetectionSquare->InitBoxExtent(FVector(DetectionRangeX, DetectionRangeY, DetectionRangeZ));

	// Speeds
	ChaseSpeed = 300.f;

	// Timer Delay
	AttackDelayMax = 4.f;
	AttackDelayMin = 1.f;

	// Other
	Health = 20;
	Burrowed = true;
	HasDoneDamage = false;
	MontageHasPlayed = false;
	AttackHits = false;

	//---------------- Audio ----------------- 

//Loads soundque object
	static ConstructorHelpers::FObjectFinder<USoundCue> BurrowCueObject(TEXT("/Script/Engine.SoundCue'/Game/Audio/EnemyTwo/SC_Digging.SC_Digging'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> AttackSoundCueObject(TEXT("/Script/Engine.SoundCue'/Game/Audio/EnemyTwo/SC_Attack.SC_Attack'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> TakeDamageSoundCueObject(TEXT("/Script/Engine.SoundCue'/Game/Audio/EnemyTwo/SC_TakeDamage.SC_TakeDamage'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> DeathSoundCueObject(TEXT("/Script/Engine.SoundCue'/Game/Audio/EnemyTwo/SC_DeathSound.SC_DeathSound'"));

	//Checks if loading worked	
	if (BurrowCueObject.Succeeded()) {
		DiggingSoundCue = BurrowCueObject.Object;
		BurrowAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BurrowAudioComponent"));
		BurrowAudioComponent->SetupAttachment(RootComponent);
	}

	if (AttackSoundCueObject.Succeeded()) {
		AttackingSoundCue = AttackSoundCueObject.Object;
		AttackAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AttackAudioComponent"));
		AttackAudioComponent->SetupAttachment(RootComponent);
	}

	if (TakeDamageSoundCueObject.Succeeded()) {
		TakeingDamageSoundCue = TakeDamageSoundCueObject.Object;
		TakeDamageAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("TakeDamageAudioComponent"));
		TakeDamageAudioComponent->SetupAttachment(RootComponent);
	}

	if (DeathSoundCueObject.Succeeded()) {
		DeathDamageSoundCue = DeathSoundCueObject.Object;
		DeathAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("DeathAudioComponent"));
		DeathAudioComponent->SetupAttachment(RootComponent);
	}
}

void AEnemyTwo::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyTwo::BeginPlay()
{
	Super::BeginPlay();

	// ------------- Audio Setup --------------

	if (BurrowAudioComponent && DiggingSoundCue)
		BurrowAudioComponent->SetSound(DiggingSoundCue);

	if (AttackAudioComponent && AttackingSoundCue)
		AttackAudioComponent->SetSound(AttackingSoundCue);

	if (TakeDamageAudioComponent && TakeingDamageSoundCue)
		TakeDamageAudioComponent->SetSound(TakeingDamageSoundCue);

	if (DeathAudioComponent && DeathDamageSoundCue)
		DeathAudioComponent->SetSound(DeathDamageSoundCue);

	GetMesh()->SetVisibility(false);

	// Gets the AI Controller
	EnemyController = Cast<AAIController>(GetController());

	EnemyState = EEnemyState::EES_EnemyIdle;
	EnemyAttackState = EEnemyAttackState::EES_EnemyUnoccupied;
	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;

	//Initilizations
	AtttackColliderSquare->OnComponentBeginOverlap.AddDynamic(this, &AEnemyTwo::OnOverlap);
	DetectionSquare->OnComponentBeginOverlap.AddDynamic(this, &AEnemyTwo::OnPlayerDetect);
}

void AEnemyTwo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyState != EEnemyState::EES_EnemyIdle)
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
	if (!InTargetRange(CombatTarget, ChaseRadius) && EnemyAttackState == EEnemyAttackState::EES_EnemyUnoccupied)
	{
		// When outside of chase radius Stay where it is
		CombatTarget = nullptr;
		EnemyState = EEnemyState::EES_EnemyIdle;
		StayAtPosition(GetActorLocation());
		PlayBurrow(false,true);

		//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Loose intrest")));
	}
	if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState == EEnemyState::EES_EnemyChaseing && EnemyAttackState == EEnemyAttackState::EES_EnemyUnoccupied)
	{
		// Outside attack range, chase character
		MoveToTarget(CombatTarget);

		//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Chasing player")));
	}
	if (InTargetRange(CombatTarget, AttackRadius) && EnemyAttackState == EEnemyAttackState::EES_EnemyUnoccupied)
	{
		// Inside attack range, attack character.
		Attack();

		//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Attack")));
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
	MoveRequest.SetAcceptanceRadius(40.f);

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

	if (BurrowAudioComponent && DiggingSoundCue)
		BurrowAudioComponent->Play(0.f);

	if (VFXBurrow) {
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, VFXBurrow, (GetActorLocation() + (GetActorForwardVector() * 2 + FVector(0.f, 0.f, -50.f))),
			GetActorRotation(), FVector(2.f), true,
			true, ENCPoolMethod::None, true);

	}
}

void AEnemyTwo::PlayAttackMontage()
{
	//PLays the animation montage relating to connected Animation montage blue print

		// Gets the Animation instance for montage animations
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if(AnimInstance == nullptr)
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("No AnimInstance")));

	if(AttackMontage == nullptr)
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("No AttackMontage")));

	if (AnimInstance && AttackMontage)
	{
		//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Montage Play")));
		AnimInstance->Montage_Play(AttackMontage);
		FName SectionName = FName("Attack1");
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void AEnemyTwo::Attack()
{

	//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Attack called")));
	EnemyAttackState = EEnemyAttackState::EES_EnemyAttacking;
	PlayAttackMontage();
	HasDoneDamage = false;

	if (AttackAudioComponent && AttackingSoundCue)
		AttackAudioComponent->Play(0.f);

}

void AEnemyTwo::AttackEnd()
{
	//Changes state when Attack over, called in Anim blueprint
	EnemyState = EEnemyState::EES_EnemyChaseing;
	EnemyAttackState = EEnemyAttackState::EES_EnemyUnoccupied;
	

}

void AEnemyTwo::TakeDamageAudio()
{
	if (TakeDamageAudioComponent && TakeingDamageSoundCue)
		TakeDamageAudioComponent->Play(0.f);
}

void AEnemyTwo::Die()
{
	//Spawns The pysics asset when it has died
	GetWorld()->SpawnActor<AActor>(BP_EnemyTwoPysics,GetActorTransform());

	if (DeathAudioComponent && DeathDamageSoundCue)
		DeathAudioComponent->Play(0.f);

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

	if (Player && Player->ActorHasTag(FName("PlayerCharacter")) && Burrowed == true && EnemyAttackState == EEnemyAttackState::EES_EnemyUnoccupied)
	{
		EnemyState = EEnemyState::EES_EnemyChaseing;
		EnemyAttackState = EEnemyAttackState::EES_EnemyUnoccupied;
		PlayBurrow(true, false);
		CombatTarget = OtherActor;
		MoveToTarget(CombatTarget);
		//GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Yellow, FString::Printf(TEXT("Detects player")));
	}
}

void AEnemyTwo::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Detects overlap between player and enemy during attack animation for single damage done cast to player class. 

	//GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Yellow, FString::Printf(TEXT("Collision")));

	APlayer_Character* Player1 = Cast<APlayer_Character>(OtherActor);
	
	//if(Player1==nullptr)
		//GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Yellow, FString::Printf(TEXT("Cant get player cast")));

	if (Player1 && Player1->ActorHasTag(FName("PlayerCharacter")) && EnemyAttackState == EEnemyAttackState::EES_EnemyAttacking)
	{
		//GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Yellow, FString::Printf(TEXT("Got player cast")));
		if (HasDoneDamage == false)
		{
			Player1->Health -= 20;
			Player1->Live_Hunger -= 10;
			if (Player1->Dead == false)
				Player1->TakeDamageAudio();
			HasDoneDamage = true;
			//GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Yellow, FString::Printf(TEXT("PlayerTakesDamge")));
		}
	}
}













