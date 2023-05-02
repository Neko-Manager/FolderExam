// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyTwoAnimInstance.h"
#include "EnemyTwo.h"

void UEnemyTwoAnimInstance::NativeInitializeAnimation()
{
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
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
		if (Pawn && EnemyTwo == nullptr) {
			EnemyTwo = Cast<AEnemyTwo>(Pawn);
		}
	}

	if (Pawn) {
		FVector Speed = Pawn->GetVelocity();
		Speed.Z = 0.f;
		MovementSpeed = Speed.Size();
	}
}
