// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe.h"
#include "EnemyOne.h"
#include "InventoryGamemode.h"
#include "Player_Character.h"
#include "Components/BoxComponent.h"



AAxe::AAxe()
{

	AmountOfAxe = 0;
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	/*PrimaryActorTick.bCanEverTick = true;

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	HitBox->InitBoxExtent(FVector(5.f, 20.f, 10.f));
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AAxe::OnOverlap);
	HitBox->SetupAttachment(InteractableMesh);*/
}

void AAxe::BeginPlay()
{
}



void AAxe::Tick(float DeltaTime)
{
		Super::Tick(DeltaTime);
}

void AAxe::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
//	if(OtherActor->IsA<AEnemyOne>())
//	{
//
//	    /*EnemyOne->Health -= 10;*/
//		Player->Live_Stamina -= 5;
//	}
//
//	navnPåEnemy->Health --;
//	PlayerCharacter->LiveStamina -= 0.2;

	}

	



