// Fill out your copyright notice in the Description page of Project Settings.

//#include "Inventory.h"
#include "InventoryGamemode.h"
//#include "InvenoryHUD.h"
#include "Player_Character.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AInventoryGamemode::BeginPlay()
{
	//When player enter game, apply HUD to the screen
	ApplyHUDChange();
}

 AInventoryGamemode::AInventoryGamemode()
{
	
	 HUDState = EHUDState::HS_Ingame;

}

 void AInventoryGamemode::ApplyHUDChange()
 {
	 //Remove Previous HUD
	 if (CurrentWidget != nullptr)
	 {
		 CurrentWidget->RemoveFromParent();
	 }
	 switch (HUDState)
	 {
	 	case EHUDState::HS_Ingame:
		 {

			 ApplyHUD(InGameHUDClass, false, false);
				break;

		 }
		case EHUDState::HS_Inventory:
		 {

			ApplyHUD(InventoryHUDClass, true, true);
				break;

		 }
		 case EHUDState::HS_Shop:
		 {

			 ApplyHUD(ShopHUDClass, true, true);
				break;
		 }
		 default:
		 {

			 ApplyHUD(InGameHUDClass, false, false);
			 break;

		 }
		

	 }



 }

 uint8 AInventoryGamemode::GetHUDState()
 {
	 return HUDState;
 }

 void AInventoryGamemode::ChangeHUDState(uint8 NewState)
 {
	 HUDState = NewState;
	 ApplyHUDChange(); 
 }

 bool AInventoryGamemode::ApplyHUD(TSubclassOf<UUserWidget> WidgetToApply, bool bShowMouseCursor, bool bEnableClickEvents)
 {

	 APlayer_Character* Player_Character = Cast<APlayer_Character>(UGameplayStatics::GetPlayerCharacter(this, 0));
	 APlayerController* Controller = GetWorld()->GetFirstPlayerController();


	 if (WidgetToApply != nullptr)
	 {
		 Controller->bShowMouseCursor = bShowMouseCursor;
		 Controller->bEnableClickEvents = bEnableClickEvents;

		 CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetToApply);


		 if (CurrentWidget != nullptr)
		 {
			 CurrentWidget->AddToViewport();
			 return true;
		 }
		 else return false;
		 
		
			
		
	 }
	 else return false;
 }

