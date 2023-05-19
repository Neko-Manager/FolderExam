// Fill out your copyright notice in the Description page of Project Settings.
#include "Player_Character.h"
#include "Axe.h"
#include "PickUp.h"
#include "EnemyOne.h"
#include "EnemyTwo.h"

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
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Animation/AnimMontage.h"

//Inputs
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"
#include "InventoryGamemode.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayer_Character::APlayer_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ------------- Camera control --------------
	PlayerMesh = CreateDefaultSubobject<UMeshComponent>(TEXT("PlayerMesh"));

	//Initializing the spring arm.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetMesh(),FName("HeadSocketTest"));
	SpringArm->SetRelativeLocation(FVector(0.f, -0.1f, 0.1f));
	SpringArm->SetRelativeRotation(FRotator(360.f, 180.f, 180.f));
	SpringArm->SetRelativeScale3D(FVector(0.01f, 0.01f, 0.01f));
	SpringArm->TargetArmLength = 0.f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	// ------------- Player auto possession --------------
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// ------------- CollisionMesh Control --------------
	AttackCollisionMesh = CreateDefaultSubobject<UBoxComponent>(TEXT("AxeCollisionMesh"));

	AttackCollisionMesh->SetupAttachment(GetMesh(), "RightHandSocket");
	AttackCollisionMesh->InitBoxExtent(FVector(1.6f, 27.f, 0.7f));
	AttackCollisionMesh->SetRelativeLocation(FVector(6.6f, 18.8f, 15.7f));
	AttackCollisionMesh->SetRelativeRotation(FRotator(38.8f,-42.9f,-42.f));

	//---------------- Audio ----------------- 

//Loads soundque object
	static ConstructorHelpers::FObjectFinder<USoundCue> AttackSoundCueObject(TEXT("/Script/Engine.SoundCue'/Game/Audio/Player/SC_Player_Attack.SC_Player_Attack'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> TakeDamageSoundCueObject(TEXT("/Script/Engine.SoundCue'/Game/Audio/Player/SC_TakeDamage_Player.SC_TakeDamage_Player'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> JumpSoundCueObject(TEXT("/Script/Engine.SoundCue'/Game/Audio/Player/SC_Jump_Player.SC_Jump_Player'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> PickupSoundCueObject(TEXT("/Script/Engine.SoundCue'/Game/Audio/Player/SC_pickup_Player.SC_pickup_Player'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> DeathSoundCueObject(TEXT("/Script/Engine.SoundCue'/Game/Audio/Player/SC_Die.SC_Die'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> PickupNoteSoundCueObject(TEXT("/Script/Engine.SoundCue'/Game/Audio/Player/SC_PaperPickup.SC_PaperPickup'"));

	//Checks if loading worked	
	if (AttackSoundCueObject.Succeeded()) {
		AttackingSoundCue = AttackSoundCueObject.Object;
		AttackAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AttackAudioComponent"));
		AttackAudioComponent->SetupAttachment(RootComponent);
	}

	if (TakeDamageSoundCueObject.Succeeded()) {
		TakeDamageSoundCue = TakeDamageSoundCueObject.Object;
		TakeDamageAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("TakeDamageAudioComponent"));
		TakeDamageAudioComponent->SetupAttachment(RootComponent);
	}

	if (JumpSoundCueObject.Succeeded()) {
		JumpSoundCue = JumpSoundCueObject.Object;
		JumpAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("JumpSoundComponent"));
		JumpAudioComponent->SetupAttachment(RootComponent);
	}

	if (PickupSoundCueObject.Succeeded()) {
		PickupSoundCue = PickupSoundCueObject.Object;
		PickupAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PickupAudioComponent"));
		PickupAudioComponent->SetupAttachment(RootComponent);
	}
	if (DeathSoundCueObject.Succeeded()) {
		DeathSoundCue = DeathSoundCueObject.Object;
		DeathAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("DeathAudioComponent"));
		DeathAudioComponent->SetupAttachment(RootComponent);
	}

	if (PickupNoteSoundCueObject.Succeeded()) {
		PickupNoteSoundCue = AttackSoundCueObject.Object;
		PickupNoteAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PickupNoteAudioComponent"));
		PickupNoteAudioComponent->SetupAttachment(RootComponent);
	}

	// Death init
	Dead = false;
	MontagePlaying = false;
}


// Called when the game starts or when spawned
void APlayer_Character::BeginPlay()
{
	Super::BeginPlay();

	// ------------- Audio Setup --------------
	if (AttackAudioComponent && AttackingSoundCue)
		AttackAudioComponent->SetSound(AttackingSoundCue);

	if (TakeDamageAudioComponent && TakeDamageSoundCue)
		TakeDamageAudioComponent->SetSound(TakeDamageSoundCue);

	if (JumpAudioComponent && JumpSoundCue)
		JumpAudioComponent->SetSound(JumpSoundCue);

	if (PickupAudioComponent && PickupSoundCue)
		PickupAudioComponent->SetSound(PickupSoundCue);

	if (DeathAudioComponent && DeathSoundCue)
		DeathAudioComponent->SetSound(DeathSoundCue);

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
	Counter = 3.f;

	// ------------- Default floats and integers --------------
	//Booleans for sprinting and movement
	Sprinting = false;
	Exhaust = false;
	Crouching = false;

	//Booleans for added effects
	Starving = false;

	// ------------- Defaults for equip control --------------
	Equiped = false;
	Has_Equiped = false;
	Attacking = false;

	// ------------- Collision Dynamics --------------
	AttackCollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &APlayer_Character::OnOverlap);

	// ------------- Combat Booleans --------------
	AxeIsActive = false;
	StickIsActive = false;
	KnifeIsActive = false;
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
	EquipItem(TimeTick);

	if(Health <= 0 && Dead == false)
	{
		Die();
		Dead = true;
	}

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
		EnhancedInputComponent->BindAction(IA_Eating, ETriggerEvent::Triggered, this, &APlayer_Character::EatingTrigger);

		//Combat Inputs
		EnhancedInputComponent->BindAction(IA_Attack, ETriggerEvent::Triggered, this, &APlayer_Character::AttackTrigger);

	}
}

void APlayer_Character::TakeDamageAudio()
{
	if (TakeDamageAudioComponent && TakeDamageSoundCue)
		TakeDamageAudioComponent->Play(0.f);
}

void APlayer_Character::PlayAttackMontage()
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

void APlayer_Character::AttackEnd()
{
	//Called in Anim BP
	MontagePlaying = false;
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
	if (Controller && Value.IsNonZero() && GetVelocity().Length() >= Walk_Speed)
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
	//Function for hunger which progress towards max to zero
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

		//Reducing max stamina and speed
		if(Starving == true)
		{
			Max_Stamina = 50.f;
			Walk_Speed = 300.f;
		}
	}
	else
	{
		//Resetting
		Starving = false;
		Max_Stamina = 100.f;
		Walk_Speed = 600.f;
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
}

void APlayer_Character::EatingTrigger(const FInputActionValue& Value)
{
	//Checking each index for the eat-function
	if (Value.IsNonZero() && Inventory[LocalIndex[0]] && DisabledThumbnails[LocalIndex[0]] == true)
	{
		EatingChecker(LocalIndex[0]);
		/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Item 0 check for eat")));*/
	}
	if (Value.IsNonZero() && Inventory[LocalIndex[1]] && DisabledThumbnails[LocalIndex[1]] == true)
	{
		EatingChecker(LocalIndex[1]);
		/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Item 1 check for eat")));*/
	}
	if (Value.IsNonZero() && Inventory[LocalIndex[2]] && DisabledThumbnails[LocalIndex[2]] == true)
	{
		EatingChecker(LocalIndex[2]);
		/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Item 2 check for eat")));*/
	}
	if (Value.IsNonZero() && Inventory[LocalIndex[3]] && DisabledThumbnails[LocalIndex[3]] == true)
	{
		EatingChecker(LocalIndex[3]);
		/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Item 3 check for eat")));*/
	}
	if (Value.IsNonZero() && Inventory[LocalIndex[4]] && DisabledThumbnails[LocalIndex[4]] == true)
	{
		EatingChecker(LocalIndex[4]);
		/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Item 4 check for eat")));*/
	}

}

void APlayer_Character::ToggleDeathHUD()
{
	//Function to control HUD when player dies
	AInventoryGamemode* Gamemode = Cast<AInventoryGamemode>(GetWorld()->GetAuthGameMode());

	if (Gamemode->GetHUDState() == Gamemode->HS_Ingame)
	{
		Gamemode->ChangeHUDState(Gamemode->HS_Death);

	}
	else
	{
		Gamemode->ChangeHUDState((Gamemode->HS_Ingame));
	}
}

void APlayer_Character::EatingChecker(int32 Index)
{

	//Checking if eating boolean is true
	if(Has_Equiped == true && Health <= 100.f && Live_Hunger <= Max_Hunger)
	{
		FDetachmentTransformRules TransformRules(EDetachmentRule::KeepRelative, true);

		if (ItemPickedEquiped == "Mango")
		{
			//Hunger less and Health less
			if (Live_Hunger + 10.f <= Max_Hunger && Health + 20.f <= 100.f)
			{
				Live_Hunger += 10.f;
				Health += 20.f;
			}
			//Hunger greater and Health Less
			if (Live_Hunger + 10.f >= Max_Hunger && Health + 20.f <= 100.f)
			{
				Live_Hunger = Max_Hunger;
				Health += 20.f;
			}
			//Hunger less and Health greater
			if (Live_Hunger + 10.f <= Max_Hunger && Health + 20.f >= 100.f)
			{
				Live_Hunger += 10.f;
				Health = 100.f;
			}
			//Both Greater
			if (Live_Hunger + 10.f >= Max_Hunger && Health + 20.f >= 100.f)
			{
				Live_Hunger = Max_Hunger;
				Health = 100.f;
			}
			Inventory[Index]->SetActorEnableCollision(false);
			Inventory[Index]->DetachFromActor(TransformRules);
			Inventory[Index]->InteractableMesh->SetVisibility(false);
			Has_Equiped = false;
			DisabledThumbnails[Index] = false;
			Inventory[Index] = nullptr;
			/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Mango eaten")));*/
		}

		if (ItemPickedEquiped == "Coconut")
		{
			//Hunger less and Health less
			if(Live_Hunger + 20.f <= Max_Hunger && Health + 10.f <= 100.f)
			{
				Live_Hunger += 20.f;
				Health += 10.f;
			}
			//Hunger greater and Health Less
			if (Live_Hunger + 20.f >= Max_Hunger && Health + 10.f <= 100.f)
			{
				Live_Hunger = Max_Hunger;
				Health += 10.f;
			}
			//Hunger less and Health greater
			if (Live_Hunger + 20.f <= Max_Hunger && Health + 10.f >= 100.f)
			{
				Live_Hunger += 20.f;
				Health = 100.f;
			}
			//Both Greater
			if (Live_Hunger + 20.f >= Max_Hunger && Health + 10.f >= 100.f)
			{
				Live_Hunger = Max_Hunger;
				Health = 100.f;
			}
			Inventory[Index]->SetActorEnableCollision(false);
			Inventory[Index]->DetachFromActor(TransformRules);
			Inventory[Index]->InteractableMesh->SetVisibility(false);
			Has_Equiped = false;
			DisabledThumbnails[Index] = false;
			Inventory[Index] = nullptr;
			/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Coconut eaten")));*/
		}

		if (ItemPickedEquiped == "Bandages")
		{
			if(Health + 40.f <= 100.f)
			{
				Health += 40.f;
			}
			if(Health + 40.f >= 100.f)
			{
				Health = 100.f;
			}
			Inventory[Index]->SetActorEnableCollision(false);
			Inventory[Index]->DetachFromActor(TransformRules);
			Inventory[Index]->InteractableMesh->SetVisibility(false);
			Has_Equiped = false;
			DisabledThumbnails[Index] = false;
			Inventory[Index] = nullptr;
			/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Bandages used")));*/
		}

		
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

		if (JumpAudioComponent && JumpSoundCue)
			JumpAudioComponent->Play(0.f);

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

		//Timer for the function
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
	//Checking each index for the Dropp-function
	if(Value.IsNonZero() && Inventory[LocalIndex[0]] && DisabledThumbnails[LocalIndex[0]] == true)
	{
		DroppItem(LocalIndex[0]);
		/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Item 0 dropped")));*/
	}
	if (Value.IsNonZero() && Inventory[LocalIndex[1]] && DisabledThumbnails[LocalIndex[1]] == true)
	{
		DroppItem(LocalIndex[1]);
		/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Item 1 dropped")));*/
	}
	if (Value.IsNonZero() && Inventory[LocalIndex[2]] && DisabledThumbnails[LocalIndex[2]] == true)
	{
		DroppItem(LocalIndex[2]);
		/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Item 2 dropped")));*/
	}
	if (Value.IsNonZero() && Inventory[LocalIndex[3]] && DisabledThumbnails[LocalIndex[3]] == true)
	{
		DroppItem(LocalIndex[3]);
		/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Item 3 dropped")));*/
	}
	if (Value.IsNonZero() && Inventory[LocalIndex[4]] && DisabledThumbnails[LocalIndex[4]] == true)
	{
		DroppItem(LocalIndex[4]);
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Item 4 dropped")));
	}
}

void APlayer_Character::DroppItem(int32 Index)
{
	FDetachmentTransformRules TransformRules(EDetachmentRule::KeepRelative, true);

	//Checkinf i fitem is equiped and that the disableThumbnail is active for the respective slot and that the name also is the same
	if (Has_Equiped == true && ItemPickedEquiped == Inventory[Index]->ItemName)
	{
		//Setting physics attributes to true when dropping for a realistic effect. Also removes the disabledThumbnail for the respective index and sets it to nullptr. This will make it open for a new item to be picket up
		Inventory[Index]->DetachFromActor(TransformRules);
		Inventory[Index]->InteractableMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Inventory[Index]->InteractableMesh->SetSimulatePhysics(true);
		Inventory[Index]->SetActorLocation(FVector(GetActorLocation()));
		
		DisabledThumbnails[Index] = false;
		Has_Equiped = false;
		Inventory[Index] = nullptr;

	
	}
}

void APlayer_Character::EquipItem(int32 Index)
{
	AInventoryGamemode* Gamemode = Cast<AInventoryGamemode>(GetWorld()->GetAuthGameMode());

	if (Gamemode->GetHUDState() == Gamemode->HS_Inventory)
	{
		if(Inventory[Index] != nullptr && Equiped == true && DisabledThumbnails[Index] != true && Has_Equiped == false)
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

			//Equiped state resets the button pressed in BP
			Equiped = false;

			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Item equiped:")));
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::FromInt(Index));

		}
	}
}

void APlayer_Character::SwapItem(int32 Index)
{

	AInventoryGamemode* Gamemode = Cast<AInventoryGamemode>(GetWorld()->GetAuthGameMode());


	//Checkinf i fitem is equiped and that the disableThumbnail is active for the respective slot and that the name also is the same
	if (Gamemode->GetHUDState() == Gamemode->HS_Inventory/*&& Inventory[Index] != nullptr && Equiped == true*/)
	{
		//Checking if item is the same as item held
		if (Has_Equiped == true && Inventory[Index]->ItemName != ItemPickedEquiped && Equiped == true)
		{

			if (DisabledThumbnails[Index] == false && Inventory[Index]->ItemName == ItemPickedEquiped)
			{
				Has_Equiped = true;
			}

			else if (DisabledThumbnails[Index] == false && Inventory[Index]->ItemName != ItemPickedEquiped)
			{
				
				Has_Equiped = false;
				EquipItem(Index);
			}
			Equiped = false;
		}

		else if (Has_Equiped == true && Inventory[Index]->ItemName != ItemPickedEquiped && Equiped == true)
		{
			if (DisabledThumbnails[Index] == true && Inventory[Index]->ItemName != ItemPickedEquiped)
			{
				DisabledThumbnails[Index] = false;
				Inventory[Index]->InteractableMesh->SetVisibility(false);
				Inventory[Index]->InteractableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				Has_Equiped = false;
			}
		
		}

		/*if (Inventory[Index]->ItemName != ItemPickedEquiped && Equiped == true && Has_Equiped == true)
		{
			Has_Equiped = false;

			if(Inventory[Index]->ItemName != ItemPickedEquiped && DisabledThumbnails[Index] == true)
			{
				DisabledThumbnails[Index] = false;
				Inventory[Index]->InteractableMesh->SetVisibility(false);
				Inventory[Index]->InteractableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				Has_Equiped = false;
			}
			EquipItem(Index);
			
		}*/
	}
}



// ------------- Combat Control (Axe) --------------
void APlayer_Character::AttackTrigger(const FInputActionValue& Value)
{
	AInventoryGamemode* Gamemode = Cast<AInventoryGamemode>(GetWorld()->GetAuthGameMode());

	//Requirements for the attack bool to be true
	if(Gamemode->GetHUDState() == Gamemode->HS_Ingame && Value.IsNonZero() && Live_Stamina >= NULL && Has_Equiped == true && Exhaust == false)
	{
		Attacking = true;
		/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString::Printf(TEXT("Attacking = true")));*/


		//PLaying attack montage
		if(MontagePlaying == false)
		{
			PlayAttackMontage();
			MontagePlaying = true;
			if (AttackAudioComponent && AttackingSoundCue)
				AttackAudioComponent->Play(0.f);
		}
		
		//Activating damage-function
		if (Attacking == true)
		{
			DamageControl();
			/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("AttackTrigger activated")));*/
		}
		
		Attacking = false;
		/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString::Printf(TEXT("Attacking = false")));*/
	}
	else
	{
		Attacking = false;
		AxeIsActive = false;
	}
	 
}

void APlayer_Character::DamageControl()
{
	AInventoryGamemode* Gamemode = Cast<AInventoryGamemode>(GetWorld()->GetAuthGameMode());

	
	if (Gamemode->GetHUDState() == Gamemode->HS_Ingame)
	{
		//Weapons with respective damage and stamina drain
		if(Attacking == true && ItemPickedEquiped == "Axe")
		{
			AxeIsActive = true;
			Live_Stamina -= 7.f;
			/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Attacking = true")));*/
		}
		if (Attacking == true && ItemPickedEquiped == "Stick")
		{
			StickIsActive = true;
			Live_Stamina -= 3.f;
			/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Attacking = true")));*/
		}
		if (Attacking == true && ItemPickedEquiped == "Knife")
		{
			KnifeIsActive= true;
			Live_Stamina -= 5.f;
			/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Attacking = true")));*/
		}

	}
	else
	{
		AxeIsActive = false;
		StickIsActive = false;
		KnifeIsActive = false;
	}
}


// ------------- Collision --------------
void APlayer_Character::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemyOne* EnemyOne = Cast<AEnemyOne>(OtherActor);
	AEnemyTwo* EnemyTwo = Cast<AEnemyTwo>(OtherActor);

	//Overlap and damage control for when enemy is hit
	if(Has_Equiped == true)
	{
		if(AxeIsActive == true && ItemPickedEquiped == "Axe" && EnemyOne)
		{
			EnemyOne->Health -= 3;
			EnemyOne->TakeDamage();
			AxeIsActive = false;
		/*	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Attacking = false")));*/
		}

		if (AxeIsActive == true && ItemPickedEquiped == "Axe" && EnemyTwo)
		{
			EnemyTwo->Health -= 3;
			EnemyTwo->TakeDamageAudio();
			AxeIsActive = false;
			/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Attacking = false")));*/
		}

		if (StickIsActive == true && ItemPickedEquiped == "Stick" && EnemyOne)
		{
			EnemyOne->Health -= 3;
			EnemyOne->TakeDamage();
			AxeIsActive = false;
			/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Attacking = false")));*/
		}

		if (StickIsActive == true && ItemPickedEquiped == "Stick" && EnemyTwo)
		{
			EnemyTwo->Health -= 3;
			EnemyTwo->TakeDamageAudio();
			AxeIsActive = false;
			/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Attacking = false")));*/
		}

		if (AxeIsActive == true && ItemPickedEquiped == "Knife" && EnemyOne)
		{
			EnemyOne->Health -= 2;
			EnemyOne->TakeDamage();
			StickIsActive = false;
			/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Attacking = false")));*/
		}

		if (AxeIsActive == true && ItemPickedEquiped == "Knife" && EnemyTwo)
		{
			EnemyTwo->Health -= 2;
			EnemyTwo->TakeDamageAudio();
			StickIsActive = false;
			/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Attacking = false")));*/
		}
		else
		{
			AxeIsActive = false;
			StickIsActive = false;
		}
	}
}



// ------------- Item Control --------------
bool APlayer_Character::AddItemToInventory(APickUp* Item)
{
	if (Item !=  NULL)
	{
		//Searching for values and booleans in the item array
		const int32 AvailableSlot1 = DisabledThumbnails.Find(true);
		const int32 AvailableSlot2 = Inventory.Find(nullptr); // find first slot with a nullptr in it
		int32 AvailableSlot = 0;

		//Checking for when to use disabledthumbnail and which to spot to put the item into
		if(AvailableSlot1 == AvailableSlot2 && AvailableSlot1 != INDEX_NONE && Equiped == true)
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
			if (PickupAudioComponent && PickupSoundCue)
				PickupAudioComponent->Play(0.f);
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

UTexture2D* APlayer_Character::GetDisabledThumbnailAtInventorySlot(int32 Slot)
{
	//Getting the respectiv disabledThumbnail to respective slot
	if (Inventory[Slot] != NULL && Has_Equiped == true && DisabledThumbnails[Slot] == true)
	{
		return Inventory[Slot]->DisabledPickUpThumbnail;
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


void APlayer_Character::ToggleNote_1(APickUp* Item)
{

	AInventoryGamemode* Gamemode = Cast<AInventoryGamemode>(GetWorld()->GetAuthGameMode());

	if (Gamemode->GetHUDState() == Gamemode->HS_Ingame)
	{
		Gamemode->ChangeHUDState(Gamemode->HS_Note_1);
		if (PickupNoteAudioComponent && PickupNoteSoundCue)
			PickupNoteAudioComponent->Play(0.f);
		
	}
	else if(Gamemode->GetHUDState() == Gamemode->HS_Note_1){

		Gamemode->ChangeHUDState(Gamemode->HS_WinGame);

	}
	else
	{
		Gamemode->ChangeHUDState((Gamemode->HS_Ingame));
		
	}
}

void APlayer_Character::ToggleNote_2(APickUp* Item)
{
	AInventoryGamemode* Gamemode = Cast<AInventoryGamemode>(GetWorld()->GetAuthGameMode());

	if (Gamemode->GetHUDState() == Gamemode->HS_Ingame)
	{
		Gamemode->ChangeHUDState(Gamemode->HS_Note_2);
		if (PickupNoteAudioComponent && PickupNoteSoundCue)
			PickupNoteAudioComponent->Play(0.f);
	}
	else
	{
		Gamemode->ChangeHUDState((Gamemode->HS_Ingame));
	}
}

void APlayer_Character::ToggleNote_3(APickUp* Item)
{
	AInventoryGamemode* Gamemode = Cast<AInventoryGamemode>(GetWorld()->GetAuthGameMode());

	if (Gamemode->GetHUDState() == Gamemode->HS_Ingame)
	{
		Gamemode->ChangeHUDState(Gamemode->HS_Note_3);
		if (PickupNoteAudioComponent && PickupNoteSoundCue)
			PickupNoteAudioComponent->Play(0.f);
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
	CQP.AddIgnoredActor(Inventory[0]);
	CQP.AddIgnoredActor(Inventory[1]);
	CQP.AddIgnoredActor(Inventory[3]);
	CQP.AddIgnoredActor(Inventory[4]);

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

void APlayer_Character::Die()
{

	ToggleDeathHUD();

	if (DeathAudioComponent && DeathSoundCue)
		DeathAudioComponent->Play(0.f);

}




