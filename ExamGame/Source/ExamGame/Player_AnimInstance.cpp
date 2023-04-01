// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_AnimInstance.h"
#include "Player_Character.h"

void UPlayer_AnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr) 
	{
		Pawn = TryGetPawnOwner();
		if (Pawn && Player == nullptr) {
			Player = Cast<APlayer_Character>(Pawn);
		}
	}
	AttackAnim = false;
}

void UPlayer_AnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr) 
	{
		Pawn = TryGetPawnOwner();
		if (Pawn && Player == nullptr) {
			Player = Cast<APlayer_Character>(Pawn);
		}
	}

	if (Pawn) 
	{
		FVector Speed = Pawn->GetVelocity();
		Speed.Z = 0.f;
		MovementSpeed = Speed.Size();
	}

	if(Player)
	{
		AttackAnim = Player->GetAxeActive();
	}


}

void UPlayer_AnimInstance::AttackAnimReset()
{
	if(Player)
	{
		Player->ResetAxeAttack();
	}
}
