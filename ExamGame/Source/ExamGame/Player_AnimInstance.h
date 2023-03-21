// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Player_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class EXAMGAME_API UPlayer_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

		virtual void NativeInitializeAnimation() override;

public: 

	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties();

	UPROPERTY(EditAnywhere, BlueprrintReadOnly, Catagory = "Animation")
		float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprrintReadOnly, Catagory = "Animation")
		class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprrintReadOnly, Catagory = "Animation")
		class APlayer_Character* Player;
	
};
