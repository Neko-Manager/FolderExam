// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_AnimInstance.h"
#include "Player_Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayer_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (Pawn3 == nullptr) 
	{
		Pawn3 = TryGetPawnOwner();
		if (Pawn3 && Player == nullptr) 
		{
			Player = Cast<APlayer_Character>(Pawn3);
		}
	}
}

void UPlayer_AnimInstance::UpdateAnimationProperties()
{
	if (Pawn3 == nullptr) {
		Pawn3 = TryGetPawnOwner();
		if (Pawn3 && Player == nullptr) {
			Player = Cast<APlayer_Character>(Pawn3);
		}
	}

	if (Pawn3) {
		FVector Speed = Pawn3->GetVelocity();
		Speed.Z = 0.f;
		MovementSpeed = Speed.Size();

		if(Player->GetCharacterMovement()->IsFalling())
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Falling")));
			Isfalling = true;
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("NotFalling")));
			Isfalling = false;
		}
	}
}
