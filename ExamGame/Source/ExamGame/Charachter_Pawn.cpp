// Fill out your copyright notice in the Description page of Project Settings.



//Our classes
#include "Charachter_Pawn.h"

//Components
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/AudioComponent.h"

//Other
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundCue.h"

//Inputs
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"




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

