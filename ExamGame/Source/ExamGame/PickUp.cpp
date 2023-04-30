
#include "PickUp.h"
#include "Player_Character.h"
#include "InventoryGamemode.h"
#include "Kismet/GameplayStatics.h"

APickUp::APickUp()
{
	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickUpMesh"));
	InteractableMesh->SetSimulatePhysics(true);

	ItemName = FString("Enter An itemName Here");
	Value = 0;
}

void APickUp::BeginPlay()
{
	InteractableHelpText = FString::Printf(TEXT("%s: Press E to pick up"), *ItemName);
}

void APickUp::Interact_Implementation()
{
	APlayer_Character* Character = Cast<APlayer_Character>(UGameplayStatics::GetPlayerCharacter(this, 0));

	//Into Inventory
	if (Character->AddItemToInventory(this))
	{
		OnPickUp();
	}

}

void APickUp::Use_Implementation()
{
	GLog->Log("you Should Not See This");
}

void APickUp::BackToInventory()
{
	AInventoryGamemode* Gamemode = Cast<AInventoryGamemode>(GetWorld()->GetAuthGameMode());

	APlayer_Character* Character = Cast<APlayer_Character>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (Gamemode->GetHUDState() == Gamemode->HS_Inventory && this != nullptr && Character->Equiped0 == true && Inventory[0] != nullptr)
	{
		OnPickUp();
	}

}

void APickUp::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void APickUp::OnPickUp()
{
	InteractableMesh->SetVisibility(false);
	InteractableMesh->SetSimulatePhysics(false);
	SetActorEnableCollision(false);
}



