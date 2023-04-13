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
}

void AAxe::BeginPlay()
{
	Super::BeginPlay();
	Axe = InteractableMesh;
}



void AAxe::Tick(float DeltaTime)
{
		Super::Tick(DeltaTime);
		
}

void AAxe::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Pointing to information in player class for when the axe hits the player. 
	if (OtherActor->IsA<AEnemyOne>() && Player->AxeActive == true)
	{
		Player->UseItemAtInventorySlot(0);
		EnemyOne->Health -= 5;
		Player->Live_Stamina -= 5;
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Monster Hit!!!")));
	}


}

void AAxe::AttachingAxe(int32 Index)
{
	if (Inventory[Index] != NULL && Inventory[Index]->ItemName == "Axe")
	{
		InteractableMesh->SetVisibility(true);
		InteractableMesh->SetSimulatePhysics(true);
		SetActorEnableCollision(true);
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		InteractableMesh->AttachToComponent(Player->GetMesh(), TransformRules, FName("RightHandSocket"));
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Axe attached")));
	}

}
	



