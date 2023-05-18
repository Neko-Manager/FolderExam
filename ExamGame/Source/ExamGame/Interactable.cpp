// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"

// Sets default values for the help text. This can be changed/overwritten in every class.
AInteractable::AInteractable()
{
	InteractableHelpText = FString("Press E to Interact With Item");
}

// Called when the game starts or when spawned.
void AInteractable::BeginPlay()
{
	Super::BeginPlay();
}

//default Constructor for Interact Implementation, This can be changed/overwritten in every class.
void AInteractable::Interact_Implementation()
{
	GLog->Log("Interact base class: Interact() Something Is Wrong");
}


