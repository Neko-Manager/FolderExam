// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyOne_AnimInstance.h"
#include "EnemyOne.h"

void UEnemyOne_AnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn && EnemyOne == nullptr)
		{
			EnemyOne = Cast<AEnemyOne>(Pawn);
		}
	}
}

void UEnemyOne_AnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
		if (Pawn && EnemyOne == nullptr) {
			EnemyOne = Cast<AEnemyOne>(Pawn);
		}
	}

	if (Pawn) {
		FVector Speed = Pawn->GetVelocity();
		Speed.Z = 0.f;
		MovementSpeed = Speed.Size();
	}
}
