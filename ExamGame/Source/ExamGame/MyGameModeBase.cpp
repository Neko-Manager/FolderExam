// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"


AMyGameModeBase::AMyGameModeBase(){

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



}

void AMyGameModeBase::BeginPlay()
{
	Super::BeginPlay();

}

void AMyGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


