// Fill out your copyright notice in the Description page of Project Settings.
#include "Stick.h"
#include "InventoryGamemode.h"
#include "Player_Character.h"
#include "Kismet/GameplayStatics.h"


//Stick Values, name and amount

AStick::AStick()
{
	PrimaryActorTick.bCanEverTick = true;

	AmountOfStick = 0;
	ItemName = FString("Stick");
	Value = 100;

}



