// Fill out your copyright notice in the Description page of Project Settings.


#include "Note_1.h"
#include "Player_Character.h"
#include "Kismet/GameplayStatics.h"




ANote_1::ANote_1()
{
	PrimaryActorTick.bCanEverTick = true;

	AmountOfNote = 0;
	ItemName = FString("Note_1");
	Value = 100;
}

void ANote_1::BeginPlay()
{
	InteractableHelpText = FString::Printf(TEXT(": Press E to Read %s"), *ItemName);
}

void ANote_1::Interact_Implementation()
{
	
		OnPickUp();
}

void ANote_1::OnPickUp()
{
	APlayer_Character* Character = Cast<APlayer_Character>(UGameplayStatics::GetPlayerCharacter(this, 0));

	Character->ToggleNote_1(this);

}

