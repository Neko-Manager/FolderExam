// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe.h"
#include "EnemyOne.h"
#include "InventoryGamemode.h"
#include "Player_Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"


AAxe::AAxe()
{
	AmountOfAxe = 0;
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemName = FString("Axe");
	Value = 100;

	//Axe = CreateDefaultSubobject<UBoxComponent>(TEXT("axecollisionmesh"));
	//SetRootComponent(InteractableMesh);
	//Axe->InitBoxExtent(FVector(9.6f, 17.4f, 1.46f));
	//Axe->SetRelativeLocation(FVector(-10.f, 0.f, 0.f));
	//Axe->SetupAttachment(InteractableMesh);
	//Axe->OnComponentBeginOverlap.AddUniqueDynamic(this, &AAxe::OnOverlap);
}

void AAxe::BeginPlay()
{
	Super::BeginPlay();

	//Inherited properties modified
	
}

void AAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAxe::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayer_Character* Character = Cast<APlayer_Character>(UGameplayStatics::GetPlayerCharacter(this, 0));
	AEnemyOne* EnemyOne = Cast<AEnemyOne>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (OtherActor->IsA<AEnemyOne>() && Character->AxeActive == true)
	{
		EnemyOne->Health -= 10.f;
	}
}








	



