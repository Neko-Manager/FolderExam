// Fill out your copyright notice in the Description page of Project Settings.
#include "Player_Character.h"

//Components
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/AudioComponent.h"


//Other
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundCue.h"

//Inputs
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayer_Character::APlayer_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ------------- Camera control --------------
	//Initializing the spring arm.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->SetRelativeLocation(FVector(20.f, 0.f, 90.f));
	SpringArm->TargetArmLength = 1.f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;


	// ------------- Player auto possession --------------
	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void APlayer_Character::BeginPlay()
{
	Super::BeginPlay();

	// ------------- Player control for Nullpointer --------------
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem) Subsystem->AddMappingContext(IMC, 0);
	}

	// ------------- Default floats and integers --------------
	//Counter Control
	CounterAdding = 0.f;
	CounterEqual = 0.f;

	//playerReach
	Reach = 250.f;

	//Speed control
	Walk_Speed = 600.f;
	Sprint_Speed = 1000.f;
	Exhaust_Speed = 200.f;

	//Stamina control
	Live_Stamina = 100.f;
	Max_Stamina = 100.f;

	//Exhaust Control
	Exhaust_Timer = 10.f;

	// ------------- Default floats and integers --------------
	//Booleans for sprinting
	Sprinting = false;
	Exhaust = false; 

}

// Called every frame
void APlayer_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckForInteractables();

	// ------------- Stamina Updater --------------
	//Updating Booleans
	Sprinting = false;
	CounterAdding += DeltaTime;
	CounterEqual = DeltaTime;

	// ------------- Functions` updater --------------
	StaminaRecharger(CounterEqual);
	Sprint();
	ExhaustChecker(CounterAdding);

}

// ------------- Character control --------------
void APlayer_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ------------- Input components for character mesh --------------
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_GroundMovement, ETriggerEvent::Triggered, this, &APlayer_Character::GroundedMovement);
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &APlayer_Character::Look);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Triggered, this, &APlayer_Character::SprintTriggered);
		EnhancedInputComponent->BindAction(IA_Interact, ETriggerEvent::Triggered, this, &APlayer_Character::Interact);
		EnhancedInputComponent->BindAction(IA_OpenInventory, ETriggerEvent::Triggered, this, &APlayer_Character::ToggleInventory);
	}
}

void APlayer_Character::GroundedMovement(const FInputActionValue& Value)
{
	//Checking i the controller is not a NullPointer and a Controller.
	FVector2D VectorDirection = Value.Get<FVector2D>();
	GetCharacterMovement()->MaxWalkSpeed = Walk_Speed;

	if (Controller && Value.IsNonZero())
	{
		//We want to move in the direction of the Yaw rotation(x-look-axis). Fixing rotation to Yaw.
		const FRotator ControlPlayerRotationYaw = GetControlRotation();
		const FRotator YawPlayerRotation(0.f, ControlPlayerRotationYaw.Yaw, 0.f);

		//Calculated the UnitAxis. We normalize the vector and find the normalized vector.
		const FVector PlayerDirectionYaw_Forward_Backward = FRotationMatrix(YawPlayerRotation).GetUnitAxis(EAxis::X);
		const FVector PlayerDirectionYaw_Left_Right = FRotationMatrix(YawPlayerRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(PlayerDirectionYaw_Forward_Backward, VectorDirection.Y);
		AddMovementInput(PlayerDirectionYaw_Left_Right, VectorDirection.X);
	}
}

void APlayer_Character::Look(const FInputActionValue& Value)
{
	// ------------- Mouse Direction Control for player --------------

	//Checking if the controller is received.
	if (GetController() && Value.IsNonZero())
	{
		//Creating a reference for a 2D vector.
		const FVector2D LookAxisInput = Value.Get<FVector2D>();

		//Adding values for the respective axis.
		AddControllerYawInput(LookAxisInput.X);
		AddControllerPitchInput(-LookAxisInput.Y);
	}
}


// ------------- Sprint control --------------
void APlayer_Character::StaminaRecharger(float Timer)
{
	//Executing and recharging stamina if conditions are met
	if (Sprinting == false && Live_Stamina <= Max_Stamina && Exhaust == false)
	{
		Live_Stamina += Timer + 0.5f;
		/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Regaining Stamina:")));*/
	}
}

void APlayer_Character::SprintTriggered(const FInputActionValue& Value)
{
	Sprinting = true;
	if (Value.IsNonZero() && Live_Stamina >= 0) 
	{
		Sprint();
	}
}

void APlayer_Character::Sprint()
{
	// ------------- Sprinting control with regenerating stamina --------------
	if (Sprinting == true && Exhaust == false)
	{
		GetCharacterMovement()->MaxWalkSpeed = Sprint_Speed;
		Live_Stamina -= 2.f;
		/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Losing stamina:"), Live_Stamina));*/
	}
}

void APlayer_Character::ExhaustChecker(float Timer)
{
	if (Live_Stamina <= 1.f)
	{
		
		for(Timer;Timer <= Exhaust_Timer; Timer++)
		{
			Exhaust = true;
			if(Timer <= Exhaust_Timer)
			{
				GetCharacterMovement()->MaxWalkSpeed = Exhaust_Speed;
			}
			
		}
		
	}

}

// ------------- Collision --------------
void APlayer_Character::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void APlayer_Character::ToggleInventory()
{
	//Open Inventory

}

void APlayer_Character::Interact()
{
	if (CurrInteractable != nullptr)
	{
		CurrInteractable->Interact_Implementation();
	}

}

void APlayer_Character::CheckForInteractables()
{
	//LineTrace
	FVector StartTrace = Camera->GetComponentLocation();
	FVector	EndTrace = (Camera->GetForwardVector()* Reach) + StartTrace;

	//ShowHit
	FHitResult HitResult;

	//iniz parameters- ignor the actor
	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(this);

	//Cast the Line trace
	GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_WorldDynamic, CQP);

	AInteractable* PotentialInteractable = Cast<AInteractable>(HitResult.GetActor());

	if (PotentialInteractable == NULL)
	{
		HelpText = FString("");

		CurrInteractable = nullptr;
		return;

	}
	else
	{
		CurrInteractable = PotentialInteractable;
		HelpText = PotentialInteractable->InteractableHelpText;
	}
}



