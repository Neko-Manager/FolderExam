// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTwo_Physics.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AEnemyTwo_Physics::AEnemyTwo_Physics()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initializing the Space Ship
	EnemyTwo_Physics = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EnemyOne Physics"));
	SetRootComponent(EnemyTwo_Physics);

	EnemyTwo_Physics->SetWorldLocation(FVector3d(0, 0, -90));
	EnemyTwo_Physics->SetWorldRotation(FRotator(0, -90, 0));
}

// Called when the game starts or when spawned
void AEnemyTwo_Physics::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnemyTwo_Physics::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

