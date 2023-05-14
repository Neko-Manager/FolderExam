
#include "PickUp.h"
#include "Kismet/GameplayStatics.h"

APickUp::APickUp()
{
	//InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickUpMesh"));
	//InteractableMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndProbe);

	//ItemName = FString("Enter An itemName Here");
	//Value = 0;

	PickUpMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickUpMesh");

	ItemID = FName("Enter Object ID");
}

//void APickUp::BeginPlay()
//{
//	InteractableHelpText = FString::Printf(TEXT("%s: Press E to pick up"), *ItemName);
//}

//void APickUp::Interact_Implementation()
//{
//	APlayer_Character* Character = Cast<APlayer_Character>(UGameplayStatics::GetPlayerCharacter(this, 0));
//
//	//Into Inventory
//	if (Character->AddItemToInventory(this))
//	{
//		OnPickUp();
//	}
//
//}

//void APickUp::OnPickUp()
//{
//	InteractableMesh->SetVisibility(false);
//	InteractableMesh->SetSimulatePhysics(false);
//	SetActorEnableCollision(false);
//}
//
//

