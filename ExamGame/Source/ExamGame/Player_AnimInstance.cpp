// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_AnimInstance.h"
#include "Player_Character.h"

void UPlayer_AnimInstance::NativeInitializeAnimation()
{
	//Super::NativeInitializeAnimation();

	//// makes sure its not a null pointer before using it in code.
	//if (Pawn == nullptr) {
	//	Pawn = TryGetPawnOwner();
	//}
	////IsAttack = false;


	//if(Player == nullptr)
	//{
	//	Player = Cast<APlayer_Character>(Pawn);
	//}

	

	//Player = Cast<APlayer_Character>(Pawn);
	
	if (Pawn == nullptr) 
	{
		Pawn = TryGetPawnOwner();
		if (Pawn && Player == nullptr) 
		{
			Player = Cast<APlayer_Character>(Pawn);
		}
	}
	//AxeAnim = false;
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
	//if (Player) {
	//	IsAttack = Player->GetIsAttack();
	//}

}

void UPlayer_AnimInstance::ResetAttack()
{
	//if (Player) {
	//	Player->resetAttack();
	//}
}

//void UPlayer_AnimInstance::NativeUpdateAnimation(float DeltaTime)
//{
//	if (Pawn == nullptr) 
//	{
//		Pawn = TryGetPawnOwner();
//		if (Pawn && Player == nullptr) 
//		{
//			Player = Cast<APlayer_Character>(Pawn);
//		}
//	}
//
//	if (Pawn) 
//	{
//		FVector Speed = Pawn->GetVelocity();
//		Speed.Z = 0.f;
//		MovementSpeed = Speed.Size();
//	}
//
//	if(Player)
//	{
//		//Movement animations
//	/*	WalkAnim = Player->;*/
//		SprintAnim = Player->Sprinting;
//		ExhaustAnim = Player->Exhaust;
//		FallingIdleAnim = Player->bWasJumping;
//		FallingEndAnim = Player->bClientWasFalling;
//
//
//	    //Other Movement animations
//		CrouchAnim = Player->Crouching == true;
//		JumpAnim = Character->bWasJumping == true;
//
//		//CombatAnimations
//		AxeAnim = Player->AxeActive == true;
//	}
//
//
//}
//
//void UPlayer_AnimInstance::CombatAnimReset()
//{
//	if(Player)
//	{
//		Player->ResetAxeAttack();
//	}
//}
