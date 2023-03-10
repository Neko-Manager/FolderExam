// Fill out your copyright notice in the Description page of Project Settings.



//Classes
#include "Charachter_Pawn.h"

//Components
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/AudioComponent.h"

//Other
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundCue.h"

//Inputs
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"

// Sets default values
ACharachter_Pawn::ACharachter_Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ------------- Initialization --------------

	//Initializing the Playe
	PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	SetRootComponent(PlayerMesh);

	//Initializing the spring arm.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(PlayerMesh);
	SpringArm->TargetArmLength = 1000.f;
	SpringArm->SetRelativeLocation(FVector3d(0.f, 0.f, 50.f));
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 1.f;

	//Initializing the camera.
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	//Setting default values for variables.
	MovementSpeed = 100.f;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ACharachter_Pawn::BeginPlay()
{
	Super::BeginPlay();
	
	//Controller imputs --------------
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
			Subsystem->AddMappingContext(IMC, 0);
	}

}

// Called every frame
void ACharachter_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharachter_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ------------- Input components for Spaceship actions --------------
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(Movement_AI, ETriggerEvent::Triggered, this, &ACharachter_Pawn::Movement);
		EnhancedInputComponent->BindAction(Look_AI, ETriggerEvent::Triggered, this, &ACharachter_Pawn::Look);
	}

}

void ACharachter_Pawn::Movement(const FInputActionValue& Value)
{
	//Checking i the controller is not a NullPointer and a Controller.
	if (Controller && Value.IsNonZero())
	{
		//Creating a general const variable for the type of vector we want to recieve/work with.
		const FVector3d VectorMove = Value.Get<FVector3d>();

		//creating a new variable for each of the directions we want the movement to have access to. 
		const FVector3d Up_Down = GetActorUpVector();
		const FVector3d Forward_Backward = GetActorForwardVector();
		const FVector3d Right_Left = GetActorRightVector();

		//Adding the movement by defining what to update the received input, and where  to update it. (New vector, Old vector)
		AddMovementInput(Up_Down, VectorMove.Z);
		AddMovementInput(Forward_Backward, VectorMove.Y);
		AddMovementInput(Right_Left, VectorMove.X);
	}
}

void ACharachter_Pawn::Look(const FInputActionValue& Value)
{
	// ------------- Mouse Direction Control for player Ship --------------

	//Checking if the controller is received.
	if (GetController())
	{
		//Getting the general vector as in the MOvement function.
		const FVector2D LookAxisInput = Value.Get<FVector2D>();

		//Same principle as in Movement, but with 2 vectors.
		AddControllerYawInput(LookAxisInput.X);
		AddControllerPitchInput(LookAxisInput.Y);
	}
}

