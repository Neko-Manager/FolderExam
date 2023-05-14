// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayerController.h"

//classes
#include "Interactable.h"
#include "Player_Character.h"
#include "InventoryGamemode.h"

#include "Engine/DataTable.h"
#include "GameFramework/GameModeBase.h"

void AGamePlayerController::CraftItem(FInventoryItem ItemA, FInventoryItem ItemB, APlayerController* FromPlayerController)
{
    for (auto Craft : ItemB.CraftCombinations)
    {
        if (Craft.ComponentID == ItemA.ItemID)
        {
            if (Craft.bDestroyItemA)
            {
                Inventory.RemoveSingle(ItemA);
            }

            if (Craft.bDestroyItemB)
            {
                Inventory.RemoveSingle(ItemB);
            }

            AddItemToInventoryByID(Craft.ProductID);

            ReloadInventory();
        }
    }
}


void AGamePlayerController::BeginPlay()
{
    CurrentInteractable = nullptr;
    CanInteract = false;
}

void AGamePlayerController::Interact()
{
    if (CurrentInteractable)
    {
        CurrentInteractable->Interact(this);
    }
}



void AGamePlayerController::AddItemToInventoryByID(FName ID)
{

    AInventoryGamemode* GameMode = Cast< AInventoryGamemode>(GetWorld()->GetAuthGameMode());

    UDataTable* ItemTable = GameMode->GetItemDB();


    FInventoryItem* ItemToAdd = ItemTable->FindRow<FInventoryItem>(ID, "");

    if (ItemToAdd)
    {
        Inventory.Add(*ItemToAdd);
    }
}