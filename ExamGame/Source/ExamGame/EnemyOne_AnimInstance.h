// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyOne_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class EXAMGAME_API UEnemyOne_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	// ------------------------ Pointer and references ----------------------------

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable)
		void UpdateAnimationProperties();

	//Animations cannot dictate verriables so make sure it is read only
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		APawn* Pawn1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		class AEnemyOne* EnemyOne;

};
