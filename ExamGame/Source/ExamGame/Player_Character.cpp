// Fill out your copyright notice in the Description page of Project Settings.
#include "Player_Character.h"
#include "Axe.h"
#include "PickUp.h"


//Components
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetSystemLibrary.h"

//Other
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundCue.h"
#include "PickUp.h"
#include "Interactable.h"

//Inputs
#include "EnemyOne.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"
#include "InventoryGamemode.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
APlayer_Character::APlayer_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ------------- Camera control --------------
	PlayerMesh = CreateDefaultSubobject<UMeshComponent>(TEXT("PlayerMesh"));

	//Initializing the spring arm.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetMesh(),"HeadSocket");
	SpringArm->SetRelativeLocation(FVector(0.f, -20.f, 38.f));
	SpringArm->TargetArmLength = 0.f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	// ------------- Player auto possession --------------
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// ------------- Overlap control --------------
	AxeCollisionMesh = CreateDefaultSubobject<UBoxComponent>(TEXT("AxeCollisionMesh"));
	AxeCollisionMesh->InitBoxExtent(FVector(10.f, 15.f, 10.f));
	AxeCollisionMesh->SetupAttachment(GetMesh(),FName(TEXT("RightHandSocket")));
	AxeCollisionMesh->OnComponentBeginOverlap.AddDynamic(this,&APlayer_Character::OnOverlap);
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
	TimeTick = NULL;
	TimeAddingTick = NULL;

	//PlayerReach
	Reach = 250.f;
	
	//Speed control
	Walk_Speed = 600.f;
	Sprint_Speed = 1000.f;
	Crouch_Speed = 200.f;
	Exhaust_Speed = 100.f;

	//Health and other members
	Health = 100.f;

	//Stamina control
	Live_Stamina = 100.f;
	Max_Stamina = 100.f;
	Exhaust_Timer = NULL;

	//Hunger Control
	Live_Hunger = 100.f;
	Max_Hunger = 100.f;

	//Condition time limiters
	Counter = 5.f;

	// ------------- Default floats and integers --------------
	//Booleans for sprinting and movement
	Sprinting = false;
	Exhaust = false;
	AxeActive = false;
	Crouching = false;

	//Booleans for added effects
	Eating = false;
	Starving = false;

	// ------------- Defaults for equip control --------------
	Equiped = false;
	Has_Equiped = false;
	Attacking = false;

	// ------------- Initializing collision --------------
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayer_Character::OnOverlap);

	// ------------- animation hit timers --------------
	Axe_Timer = NULL;
	Axe_ActiveFrames = 1.f;


}


// Called every frame
void APlayer_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckForInteractables();

	// ------------- DeltaTime control --------------
	TimeTick = DeltaTime;
	NegativeTimeTick = -DeltaTime;
	TimeAddingTick += DeltaTime;

	// ------------- Functions` updater --------------
	//Movement
	StaminaRecharger(TimeTick);
	Sprint();
	ExhaustChecker(Live_Stamina);
	CrouchCustom();

	HungerDecay(NegativeTimeTick);
	StarvingChecker(Live_Hunger);
	EatingChecker();
	EquipItem(TimeTick);
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
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &APlayer_Character::JumpTrigger);
		EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Triggered, this, &APlayer_Character::SprintTriggered);
		EnhancedInputComponent->BindAction(IA_Crouch, ETriggerEvent::Triggered, this, &APlayer_Character::CrouchTriggered);

		//Item related inputs
		EnhancedInputComponent->BindAction(IA_Interact, ETriggerEvent::Triggered, this, &APlayer_Character::Interact);
		EnhancedInputComponent->BindAction(IA_OpenInventory, ETriggerEvent::Triggered, this, &APlayer_Character::ToggleInventory);
		EnhancedInputComponent->BindAction(IA_DropItem, ETriggerEvent::Triggered, this, &APlayer_Character::DroppItemTrigger);
		//Combat Inputs
		EnhancedInputComponent->BindAction(IA_AxeAttack, ETriggerEvent::Triggered, this, &APlayer_Character::AxeAttackTrigger);


	}
}

void APlayer_Character::GroundedMovement(const FInputActionValue& Value)
{
	//Checking i the controller is not a NullPointer and a Controller.
	FVector2D VectorDirection = Value.Get<FVector2D>();
	GetCharacterMovement()->MaxWalkSpeed = Walk_Speed;
	AInventoryGamemode* Gamemode = Cast<AInventoryGamemode>(GetWorld()->GetAuthGameMode());

	if (Controller && Value.IsNonZero() && Gamemode->GetHUDState() == Gamemode->HS_Ingame)
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
	AInventoryGamemode* Gamemode = Cast<AInventoryGamemode>(GetWorld()->GetAuthGameMode());

	//Checking if the controller is received.
	if (Controller && Value.IsNonZero() && Gamemode->GetHUDState() == Gamemode->HS_Ingame)
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
		Live_Stamina += Timer + 0.2f;
		/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Regaining Stamina:")));*/
	}
}

void APlayer_Character::SprintTriggered(const FInputActionValue& Value)
{
	if (Controller && Value.IsNonZero() && GetCharacterMovement()->MaxWalkSpeed >= Walk_Speed)
	{
		//Setting boolean for sprint = true
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
		Live_Stamina -= 0.5f;
		/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Losing stamina:")));*/
	}
}


// ------------- Crouch Control --------------
void APlayer_Character::CrouchTriggered(const FInputActionValue& Value)
{
	AInventoryGamemode* Gamemode = Cast<AInventoryGamemode>(GetWorld()->GetAuthGameMode());

	//Requirements for triggering crouch
	if (Controller && Value.IsNonZero() && Gamemode->GetHUDState() == Gamemode->HS_Ingame)
	{
		Crouching = true;
		CrouchCustom();
	}
	Crouching = false;

	
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


// ------------- Hunger Control --------------
void APlayer_Character::HungerDecay(float Timer)
{
	if(Sprinting == false && Exhaust == false && Live_Hunger >= NULL && Starving == false)
	{
		Live_Hunger += Timer/3.f;
	}
}

void APlayer_Character::StarvingChecker(float Hunger)
{
	//Checking what value Hunger has before eventually changing to starving state.
	if(Hunger <= NULL)
	{
		Starving = true;

		if(Starving == true)
		{
			Max_Stamina = 50.f;
			GetCharacterMovement()->MaxWalkSpeed = Walk_Speed / 2;
		}
	}
}

void APlayer_Character::EatingChecker()
{
	//Checking if eating boolean is true
	if(Eating == true)
	{
		Live_Hunger += 20;
	}
}


// ------------- Jump Control --------------
void APlayer_Character::JumpTrigger(const FInputActionValue& Value)
{
	AInventoryGamemode* Gamemode = Cast<AInventoryGamemode>(GetWorld()->GetAuthGameMode());

	if(Controller && Value.IsNonZero() && Exhaust == false && Gamemode->GetHUDState() == Gamemode->HS_Ingame && bClientWasFalling == false)
	{
		//Jump Only activates if Jump function has run through and checked for boolean bclientWasFalling
		Jump();
		Live_Stamina -= 20.f;
		bClientWasFalling = true;
	}
}


// ------------- Over time Effect Control --------------
void APlayer_Character::ExhaustChecker(float Stamina)
{
	if (Stamina <= NULL)
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Turquoise, FString::Printf(TEXT("Exhaust is TRUE")));*/

		Exhaust_Timer += TimeTick;

		if (Exhaust_Timer <= Counter)
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

// ------------- Attaching Item Control --------------
void APlayer_Character::DroppItemTrigger(const FInputActionValue& Value)
{

	AInventoryGamemode* Gamemode = Cast<AInventoryGamemode>(GetWorld()->GetAuthGameMode());
	//const int32 Index = Inventory.Find();
	int32 Index[] = { 0,1,2,3,4 };
	

	if(Gamemode->GetHUDState() == Gamemode->HS_Ingame && Value.IsNonZero() && Inventory[Index[0]] && DisabledThumbnails[Index[0]] == true)
	{
		DroppItem(Index[0]);
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Item 0 dropped")));
		return;
	}
	if (Gamemode->GetHUDState() == Gamemode->HS_Ingame && Value.IsNonZero() && Inventory[Index[1]] && DisabledThumbnails[Index[1]] == true)
	{
		DroppItem(Index[1]);
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Item 1 dropped")));
		return;
	}
	if (Gamemode->GetHUDState() == Gamemode->HS_Ingame && Value.IsNonZero() && Inventory[Index[2]] && DisabledThumbnails[Index[2]] == true)
	{
		DroppItem(Index[2]);
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Item 2 dropped")));
		return;
	}
	if (Gamemode->GetHUDState() == Gamemode->HS_Ingame && Value.IsNonZero() && Inventory[Index[3]] && DisabledThumbnails[Index[3]] == true)
	{
		DroppItem(Index[3]);
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Item 3 dropped")));
		return;
	}
	if (Gamemode->GetHUDState() == Gamemode->HS_Ingame && Value.IsNonZero() && Inventory[Index[4]] && DisabledThumbnails[Index[4]] == true)
	{
		DroppItem(Index[4]);
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Item 4 dropped")));
		return;
	}

	
}

void APlayer_Character::DroppItem(int32 Index)
{
	AInventoryGamemode* Gamemode = Cast<AInventoryGamemode>(GetWorld()->GetAuthGameMode());
	FDetachmentTransformRules TransformRules(EDetachmentRule::KeepRelative, true);

	//Checkinf i fitem is equiped and that the disableThumbnail is active for the respective slot and that the name also is the same
	if (Has_Equiped == true && ItemPickedEquiped == Inventory[Index]->ItemName && DisabledThumbnails[Index] == true)
	{
		//Setting physics attributes to true when dropping for a realistic effect. Also removes the disabledThumbnail for the respective index and sets it to nullptr. This will make it open for a new item to be picket up
		Inventory[Index]->DetachFromActor(TransformRules);
		Inventory[Index]->InteractableMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Inventory[Index]->InteractableMesh->GetForwardVector() + FVector(0.f, 100.f, 200.f);
		Has_Equiped = false;
		DisabledThumbnails[Index] = false;
		Inventory[Index] = nullptr;
	}
}

void APlayer_Character::EquipItem(int32 Index)
{
	AInventoryGamemode* Gamemode = Cast<AInventoryGamemode>(GetWorld()->GetAuthGameMode());



	if (Gamemode->GetHUDState() == Gamemode->HS_Inventory && Inventory[Index] != nullptr && Equiped == true && DisabledThumbnails[Index] != true && Has_Equiped == false) 
	{
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Inventory[Index]);

		//Attaching the item to socket
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		Inventory[Index]->AttachToComponent(GetMesh(), TransformRules, FName("RightHandSocket"));

		//Notifyingy that the item is equipped
		Has_Equiped = true;

		//Disabling the thumbnail
		DisabledThumbnails[Index] = true;

		//The Item name is the name from the index of the respective item
		ItemPickedEquiped = Inventory[Index]->ItemName;

		//Making the item visible
		Inventory[Index]->InteractableMesh->SetVisibility(true);
		Inventory[Index]->SetActorEnableCollision(true);

		//Equiped state resets the button pressed in BP
		Equiped = false;
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Item equiped:")));
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::FromInt(Index));
	}

}

void APlayer_Character::SwapItem(int32 Index)
{

	//AInventoryGamemode* Gamemode = Cast<AInventoryGamemode>(GetWorld()->GetAuthGameMode());
	//FDetachmentTransformRules TransformRules(EDetachmentRule::KeepRelative, true);

	////Checkinf i fitem is equiped and that the disableThumbnail is active for the respective slot and that the name also is the same
	//if (Gamemode->GetHUDState() == Gamemode->HS_Inventory && Has_Equiped == true && ItemPickedEquiped != Inventory[Index]->ItemName && DisabledThumbnails[Index] == true && Equiped == true && Inventory[Index] != nullptr)
	//{
	//	//Setting physics attributes to true when dropping for a realistic effect. Also removes the disabledThumbnail for the respective index and sets it to nullptr. This will make it open for a new item to be picket up
	//	DisabledThumbnails[Index] = false;
	//	Inventory[Index]->InteractableMesh->SetVisibility(true);
	//}
}



// ------------- Combat Control (Axe) --------------
void APlayer_Character::AxeAttackTrigger(const FInputActionValue& Value)
{
	AInventoryGamemode* Gamemode = Cast<AInventoryGamemode>(GetWorld()->GetAuthGameMode());

	if(Value.IsNonZero() && Live_Stamina >= NULL && Has_Equiped == true && Gamemode->GetHUDState() == Gamemode->HS_Ingame && Controller && Exhaust == false)
	{
		Attacking = true;

		if(Attacking == true)
		{
			Attack();
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Attacking activated")));
		}
		else
		{
			AxeActive = false;
		}
		Attacking = false;
		/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Attacking deactivated")));*/
	}

}

void APlayer_Character::Attack()
{
	AInventoryGamemode* Gamemode = Cast<AInventoryGamemode>(GetWorld()->GetAuthGameMode());


	if (ItemPickedEquiped == "Axe" && Attacking == true) 
	{
		AxeActive = true;
		Live_Stamina -= 10.f;
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("true")));
	}
	
	


}


// ------------- Collision --------------
void APlayer_Character::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Has_Equiped == true)
	{
		AEnemyOne* ThisEnemy = Cast<AEnemyOne>(OtherActor);
	
		if (AxeActive == true && ThisEnemy && ItemPickedEquiped == "Axe")
		{
			ThisEnemy->Health -= 10.f;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnHitWorks"));
		}
	/*	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AxeCollison Works"));*/
	}
}



// ------------- Item Control --------------
bool APlayer_Character::AddItemToInventory(APickUp* Item)
{
	if (Item !=  NULL)
	{
		const int32 AvailableSlot1 = DisabledThumbnails.Find(true);
		const int32 AvailableSlot2 = Inventory.Find(nullptr); // find first slot with a nullptr in it
		int32 AvailableSlot = 0;

		if(AvailableSlot1 <= AvailableSlot2 && AvailableSlot1 != -1 && Equiped == true)
		{
			AvailableSlot = AvailableSlot1;
			DisabledThumbnails[AvailableSlot] = false;
		}
		else
		{
			AvailableSlot = AvailableSlot2;
		}

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

//void APlayer_Character::UseItemAtInventorySlot(int32 Slot)
//{
//	if(Inventory[Slot] != NULL)
//	{
//		Inventory[Slot]->Use_Implementation();
//		Inventory[Slot] = NULL; //deleat item from inventory when used;
//	}
//}



// ------------- Interaction Control --------------
void APlayer_Character::ToggleInventory(const FInputActionValue& Value)
{
	//Open Inventory
	AInventoryGamemode* Gamemode = Cast<AInventoryGamemode>(GetWorld()->GetAuthGameMode());

	if (Controller && Value.IsNonZero() && Gamemode->GetHUDState() == Gamemode->HS_Ingame)
	{
		Gamemode->ChangeHUDState(Gamemode->HS_Inventory);
	}
	else
	{
		Gamemode->ChangeHUDState((Gamemode->HS_Ingame));
	}
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




