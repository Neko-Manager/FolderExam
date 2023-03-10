// Fill out your copyright notice in the Description page of Project Settings.


#include "Charachter_Pawn.h"

// Sets default values
ACharachter_Pawn::ACharachter_Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharachter_Pawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharachter_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharachter_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

