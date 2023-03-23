
<<<<<<< Updated upstream

#include "PickUp.h"
#include "Player_Character.h"
=======
#include "PickUp.h"
#include "Player_Character.h"

>>>>>>> Stashed changes
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
	OnPickUp();
}

void APickUp::Use_Implementation()
{
	GLog->Log("you Should Not See This");

}

void APickUp::OnPickUp()
{
	InteractableMesh->SetVisibility(false);
	InteractableMesh->SetSimulatePhysics(false);
	InteractableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


}


