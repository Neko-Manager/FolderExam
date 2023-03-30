// Fill out your copyright notice in the Description page of Project Settings.
#include "Player_Character.h"
#include "Axe.h"


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
#include "InventoryGamemode.generated.h"
#include "PickUp.h"
#include "Interactable.h"

//Inputs
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"

//Test Mesh
class AAxe;

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

	Inventory.SetNum(5);
	CurrentInteractable = nullptr;

	// Adds a charachter tag to the player Character for AI Detection.
	Tags.Add(FName("PlayerCharacter"));

	// ------------- Player control for Nullpointer --------------
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem) Subsystem->AddMappingContext(IMC, 0);
	}

	// ------------- Default floats and integers --------------
	//DeltaTime Control
	Exhaust_Timer = NULL;
	TimeTick = NULL;
	TimeAddingTick = NULL;

	//playerReach
	Reach = 250.f;
	
	//Speed control
	Walk_Speed = 600.f;
	Sprint_Speed = 1000.f;
	Crouch_Speed = 200.f;
	Exhaust_Speed = 100.f;

	//Stamina control
	Live_Stamina = 100.f;
	Max_Stamina = 100.f;

	//Condition time limiters
	Counter = 5.f;

	// ------------- Default floats and integers --------------
	//Booleans for sprinting
	Sprinting = false;
	Exhaust = false;
	Crouching = false;
}

// Called every frame
void APlayer_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckForInteractables();

	// ------------- DeltaTime control --------------
	TimeTick = DeltaTime;
	TimeAddingTick += DeltaTime;

	
	// ------------- Functions` updater --------------
	//Movement
	StaminaRecharger(TimeTick);
	Sprint();
	ExhaustChecker(Live_Stamina);
	CrouchCustom();
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
		EnhancedInputComponent->BindAction(IA_Crouch, ETriggerEvent::Triggered, this, &APlayer_Character::CrouchTriggered);
		EnhancedInputComponent->BindAction(IA_Interact, ETriggerEvent::Triggered, this, &APlayer_Character::Interact);
		EnhancedInputComponent->BindAction(IA_OpenInventory, ETriggerEvent::Triggered, this, &APlayer_Character::ToggleInventory);


		//Combat Inputs
		EnhancedInputComponent->BindAction(IA_AxeCut, ETriggerEvent::Triggered, this, &APlayer_Character::AxeCutTrigger);
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
	if (Controller && Value.IsNonZero())
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
	if (Controller && Value.IsNonZero())
	{
		Sprinting = true;
		if(Live_Stamina >= NULL &&  Exhaust == false)
		{
			Sprint();
		/*	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Sprinting == true:")));*/
		}
	}
	Sprinting = false;
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("Sprinting == false:")));
}

void APlayer_Character::Sprint()
{
	// ------------- Sprinting control with regenerating stamina --------------
	if (Sprinting == true)
	{
		GetCharacterMovement()->MaxWalkSpeed = Sprint_Speed;
		Live_Stamina -= 1.f;
		/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Losing stamina:")));*/
	}
}

void APlayer_Character::CrouchTriggered(const FInputActionValue& Value)
{
	if (Controller && Value.IsNonZero() && GetCapsuleComponent() != nullptr)
	{
		Crouching = true;
		CrouchCustom();
	}
	Crouching = false;
	
}

void APlayer_Character::AxeCutTrigger(const FInputActionValue& Value)
{
	if (Controller && Value.IsNonZero() && Live_Stamina >= NULL)
	{
		for (float ActiveFrame = NULL; ActiveFrame <= 5.f; ActiveFrame++)
		{
			AxeCut();
		}
	}
}

void APlayer_Character::CrouchCustom()
{
	// ------------- Sprinting control with regenerating stamina --------------
	if (Crouching == true)
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(66.f);
		GetCharacterMovement()->MaxWalkSpeed = Crouch_Speed;
	
	/*	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Crouch == true:")));*/
		
	}
	else
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(88.f);
		GetCharacterMovement()->MaxWalkSpeed = Crouch_Speed;
	/*	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("Crouch == false:")));*/
	}
}

void APlayer_Character::ExhaustChecker(float Stamina)
{
	if (Stamina <= NULL)
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Turquoise, FString::Printf(TEXT("Exhaust is TRUE")));*/

		Exhaust_Timer += TimeTick;

		if(Exhaust_Timer <= Counter)
		{
			Exhaust = true;
			GetCharacterMovement()->MaxWalkSpeed = Exhaust_Speed;
			/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("Walking Speed Active:")));*/
		}
		else
		{
			Exhaust = false;
		}
	}
	else
	{
		
		Exhaust = false;
		Exhaust_Timer = NULL;
		/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, FString::Printf(TEXT("Exhaust is FALSE")));*/
	}
}

void APlayer_Character::AxeCut()
{
	Axe->HitBox;
}

// ------------- Collision --------------
void APlayer_Character::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

bool APlayer_Character::AddItemToInventory(APickUp* Item)
{
	if (Item !=  NULL)
	{
		const int32 AvailableSlot = Inventory.Find(nullptr); // find first slot with a nullptr in it

		if (AvailableSlot != INDEX_NONE)
		{
			Inventory[AvailableSlot] = Item;
			return true;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You cant carry any more items!"));
			return false;
		}
	}
	else return false;
}

UTexture2D* APlayer_Character::GetThumbnailAtInventorySlot(int32 Slot)
{
	if (Inventory[Slot] != NULL)
	{
		return Inventory[Slot]->PickUpThumbnail;
	}
	else return nullptr;
	
}

FString APlayer_Character::GivenItemNameAtInventorySlot(int32 Slot)
{
	if (Inventory[Slot] != NULL)
	{
		return Inventory[Slot]->ItemName;
	}
	return FString("None");
}

void APlayer_Character::UseItemAtInventorySlot(int32 Slot)
{
	if(Inventory[Slot] != NULL)
	{
		Inventory[Slot]->Use_Implementation();
		Inventory[Slot] = NULL; //deleat item from inventory when used;
	}
}



void APlayer_Character::ToggleInventory()
{
	//Open Inventory

}

void APlayer_Character::Interact()
{
	if (CurrentInteractable != nullptr)
	{
		CurrentInteractable->Interact_Implementation();
	}

}

void APlayer_Character::CheckForInteractables()
{
	//LineTrace. Trace:A method for reaching out and getting feedback on objects that is represented as physical objects.
	FVector StartTrace = Camera->GetComponentLocation();
	FVector	EndTrace = (Camera->GetForwardVector()* Reach) + StartTrace;

	//ShowHit
	FHitResult HitResult;

	//Initliaize parameters and ignoring the actor, so the visual interaction text does not appear constantly
	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(this);

	//Getting the Line trace by set world
	GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_WorldDynamic, CQP);

	//Cast the Line trace
	AInteractable* PotentialInteractable = Cast<AInteractable>(HitResult.GetActor());

	//Condition for Visual text to execute
	if (PotentialInteractable == NULL)
	{
		HelpText = FString("");

		CurrentInteractable = nullptr;
		return;

	}
	else
	{
		CurrentInteractable = PotentialInteractable;
		HelpText = PotentialInteractable->InteractableHelpText;
	}
}




