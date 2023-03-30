// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUp.h"
#include "Stick.generated.h"

/**
 * 
 */
UCLASS()
class EXAMGAME_API AStick : public APickUp
{
	GENERATED_BODY()

	AStick();

	int32 AmountOfStick;
private:


};
