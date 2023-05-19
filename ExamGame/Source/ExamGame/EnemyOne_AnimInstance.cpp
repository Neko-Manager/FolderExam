// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyOne_AnimInstance.h"
#include "EnemyOne.h"


void UEnemyOne_AnimInstance::NativeInitializeAnimation()
{
	//Gets pawn and enemy refrence
	if (Pawn1 == nullptr)
	{
		Pawn1 = TryGetPawnOwner();
		if (Pawn1 && EnemyOne == nullptr)
		{
			EnemyOne = Cast<AEnemyOne>(Pawn1);
		}
	}
}

void UEnemyOne_AnimInstance::UpdateAnimationProperties()
{
	//Double check casting
	if (Pawn1 == nullptr) 
	{
		Pawn1 = TryGetPawnOwner();

		if (Pawn1 && EnemyOne == nullptr) 
		{
			EnemyOne = Cast<AEnemyOne>(Pawn1);
		}
	}

	// Calculalte movementspeed for blend animation
	if (Pawn1) {
		FVector Speed = Pawn1->GetVelocity();
		Speed.Z = 0.f;
		MovementSpeed = Speed.Size();
	}
}
