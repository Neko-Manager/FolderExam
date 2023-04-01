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

	// ------------------------ Pointer and references ----------------------------
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties();

	UFUNCTION(BlueprintCallable)
		void AttackAnimReset();
	// ------------------------ Pointer and references ----------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		class APlayer_Character* Player;


	// ------------------------ Functions ----------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
		float MovementSpeed;

	// ------------------------ Booleans---------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Booleans")
		bool AttackAnim;
};
