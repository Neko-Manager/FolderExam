// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyTwoAnimInstance.h"
#include "EnemyTwo.h"

void UEnemyTwoAnimInstance::NativeInitializeAnimation()
{
	//Gets pawn and enemy refrence
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn && EnemyTwo == nullptr)
		{
			EnemyTwo = Cast<AEnemyTwo>(Pawn);
		}
	}
}

void UEnemyTwoAnimInstance::UpdateAnimationProperties()
{
	//Gets pawn and enemy refrence again
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
		if (Pawn && EnemyTwo == nullptr) {
			EnemyTwo = Cast<AEnemyTwo>(Pawn);
		}
	}

	//Calculates movementspeed for animation blueprint
	if (Pawn) {
		FVector Speed = Pawn->GetVelocity();
		Speed.Z = 0.f;
		MovementSpeed = Speed.Size();
	}
}
