// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyTwoAnimInstance.h"
#include "EnemyTwo.h"

void UEnemyTwoAnimInstance::NativeInitializeAnimation()
{
	//Gets pawn and enemy refrence
	if (Pawn2 == nullptr)
	{
		Pawn2 = TryGetPawnOwner();
		if (Pawn2 && EnemyTwo == nullptr)
		{
			EnemyTwo = Cast<AEnemyTwo>(Pawn2);
		}
	}
}

void UEnemyTwoAnimInstance::UpdateAnimationProperties()
{
	//Gets pawn and enemy refrence again
	if (Pawn2 == nullptr) 
	{
		Pawn2 = TryGetPawnOwner();

		if (Pawn2 && EnemyTwo == nullptr) 
		{
			EnemyTwo = Cast<AEnemyTwo>(Pawn2);
		}
	}

	//Calculates movementspeed for animation blueprint
	if (Pawn2) {
		FVector Speed = Pawn2->GetVelocity();
		Speed.Z = 0.f;
		MovementSpeed = Speed.Size();
	}
}
