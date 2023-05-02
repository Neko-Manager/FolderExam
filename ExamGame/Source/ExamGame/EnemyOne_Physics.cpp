// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyOne_Physics.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AEnemyOne_Physics::AEnemyOne_Physics()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initializing the Space Ship
	EnemyOne_Physics = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EnemyOne Physics"));
	SetRootComponent(EnemyOne_Physics);

	EnemyOne_Physics->SetWorldLocation(FVector3d(0, 0, -90));
	EnemyOne_Physics->SetWorldRotation(FRotator(0, -90, 0));
}

// Called when the game starts or when spawned
void AEnemyOne_Physics::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnemyOne_Physics::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

