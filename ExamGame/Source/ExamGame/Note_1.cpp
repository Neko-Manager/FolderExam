// Fill out your copyright notice in the Description page of Project Settings.


#include "Note_1.h"
#include "Player_Character.h"
#include "Kismet/GameplayStatics.h"



// Values for the note
ANote_1::ANote_1()
{
	PrimaryActorTick.bCanEverTick = true;

	AmountOfNote = 0;
	ItemName = FString("Note_1");
	Value = 100;

	HasNote = false;
}

// Help text spesificly for the note and disable physics 
void ANote_1::BeginPlay()
{
	InteractableHelpText = FString::Printf(TEXT(": Press E to Read %s"), *ItemName);
	DisableComponentsSimulatePhysics();
}

// Call pick up to get pick up info and sets the a bool to true to giving the engine the info that the Note is picked up
void ANote_1::Interact_Implementation()
{
	
		OnPickUp();
		HasNote = true;
}

//pick up info, what hapens when the player interacts with this Note
void ANote_1::OnPickUp()
{
	APlayer_Character* Character = Cast<APlayer_Character>(UGameplayStatics::GetPlayerCharacter(this, 0));

	Character->ToggleNote_1(this);

}

