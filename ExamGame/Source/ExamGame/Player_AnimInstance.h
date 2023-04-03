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
		

public: 

	// ------------------------ Pointer and references ----------------------------

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable)
		void UpdateAnimationProperties();

	UFUNCTION(BlueprintCallable)
		void ResetAttack();

	//Animations cannot dictate verriables so make sure it is read only
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		float MovementSpeed;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	//	float IsAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		class APlayer_Character* Player;





	/*UFUNCTION(BlueprintCallable)
	void NativeUpdateAnimation(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void CombatAnimReset();*/
	// ------------------------ Pointer and references ----------------------------

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	//	class APawn* Pawn;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	//	class APlayer_Character* Player;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	//	class ACharacter* Character;


	// ------------------------ Functions ----------------------------
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	//	float MovementSpeed;

	// ------------------------ Booleans---------------------------
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Booleans")
	//	bool AxeAnim;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Booleans")
	//	bool WalkAnim;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Booleans")
	//	bool SprintAnim;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Booleans")
	//	bool FallingIdleAnim;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Booleans")
	//	bool FallingEndAnim;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Booleans")
	//	bool ExhaustAnim;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Booleans")
	//	bool CrouchAnim;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Booleans")
	//	bool JumpAnim;
};
