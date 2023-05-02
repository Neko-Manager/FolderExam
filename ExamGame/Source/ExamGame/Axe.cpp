// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe.h"
#include "EnemyOne.h"
#include "Player_Character.h"
#include "Kismet/GameplayStatics.h"


AAxe::AAxe()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemName = FString("Axe");
	Value = 100;
	//AxeCollisionMesh = CreateDefaultSubobject<UBoxComponent>(TEXT("AxeCollisionMesh"));
	//AxeCollisionMesh->SetupAttachment(InteractableMesh);
	//AxeCollisionMesh->InitBoxExtent(FVector(10.f, 10.f, 10.f));
	//AxeCollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &AAxe::OnOverlap);
}


//
//void AAxe::OnOverlap(UPrimitiveComponent* OverlappedComponent,
//	AActor* OtherActor, UPrimitiveComponent* OtherComponent,
//	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//
//	APlayer_Character* ThePlayer = Cast<APlayer_Character>(OtherActor);
//
//
//	if (ThePlayer->Has_Equiped == true)
//	{
//		AEnemyOne* ThisEnemy = Cast<AEnemyOne>(OtherActor);
//
//		if (ThePlayer->AxeActive == true && ThisEnemy && ThePlayer->ItemPickedEquiped == ItemName)
//		{
//			ThisEnemy->Health -= 10.f;
//			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnHitWorks"));
//		}
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AxeCollison Works"));
//	}
//}












	



