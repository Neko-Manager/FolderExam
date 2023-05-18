// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUp.h"
#include "Note_1.generated.h"

/**
 * 
 */
UCLASS()
class EXAMGAME_API ANote_1 : public APickUp
{
	GENERATED_BODY()


	ANote_1();


	int32 AmountOfNote;

	virtual void BeginPlay() override;

	virtual void Interact_Implementation() override;

	void OnPickUp();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		bool HasNote;
};
