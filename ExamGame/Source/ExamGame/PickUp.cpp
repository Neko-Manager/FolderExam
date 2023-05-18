
#include "PickUp.h"
#include "Player_Character.h"
#include "InventoryGamemode.h"
#include "Kismet/GameplayStatics.h"

APickUp::APickUp()
{	//Create a mesh for the pickup, enable collision and set physics
	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickUpMesh"));
    InteractableMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	InteractableMesh->SetSimulatePhysics(true);
	
	
	//PickUp default Values
	ItemName = FString("Enter An itemName Here");
	Value = 0;
}

void APickUp::BeginPlay()
{
	// Set Help Text and give the help text an item name
	InteractableHelpText = FString::Printf(TEXT("%s: Press E to pick up"), *ItemName);

}

//Default Function that happens when the player interacts with an object. This can be override within each object/Interactable class.
void APickUp::Interact_Implementation()
{
	APlayer_Character* Character = Cast<APlayer_Character>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (Character->AddItemToInventory(this))
	{
		OnPickUp();
	}

}

//Default event when you pick up an item, This can be override within each object/Interactable class.
void APickUp::OnPickUp()
{
	InteractableMesh->SetVisibility(false);
	InteractableMesh->SetSimulatePhysics(false);
	InteractableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


