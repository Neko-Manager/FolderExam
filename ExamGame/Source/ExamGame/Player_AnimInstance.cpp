// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_AnimInstance.h"
#include "Player_Character.h"

void UPlayer_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (Pawn == nullptr) 
	{
		Pawn = TryGetPawnOwner();
		if (Pawn && Player == nullptr) 
		{
			Player = Cast<APlayer_Character>(Pawn);
		}
	}
}

void UPlayer_AnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
		if (Pawn && Player == nullptr) {
			Player = Cast<APlayer_Character>(Pawn);
		}
	}

	if (Pawn) {
		FVector Speed = Pawn->GetVelocity();
		Speed.Z = 0.f;
		MovementSpeed = Speed.Size();
	}
}
