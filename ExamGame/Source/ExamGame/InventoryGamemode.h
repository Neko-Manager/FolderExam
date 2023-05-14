// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InventoryGamemode.generated.h"

/**
 * 
 */
UCLASS()
class EXAMGAME_API AInventoryGamemode : public AGameModeBase
{

	virtual void BeginPlay() override;

	GENERATED_BODY()

public:
		AInventoryGamemode();

		enum EHUDState : uint8
	{
		HS_Ingame,
		HS_Inventory,
		HS_Shop,
	};


	//Check hud state, and apply new HUD state.

	void ApplyHUDChange();


	//  -> This gets the Function for the HUD state we are in.
	uint8 GetHUDState();

	//  -> This Set's the Function, then apply the new state, and then call ApplyHUDChange(); .
	UFUNCTION(BlueprintCallable, Category = "HUD Functions")
	void ChangeHUDState(uint8 NewState);

	//  -> Apply HUD to screen.
	bool ApplyHUD(TSubclassOf<UUserWidget> WidgetToApply, bool bShowMouseCursor, bool bEnableClickEvents);

protected:

	uint8 HUDState;

	//The HUD to show inn game
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidget", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> InGameHUDClass;

	//The HUD to show when in inventory
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidget", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> InventoryHUDClass;

	//The HUD when in a Shop
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidget", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> ShopHUDClass;


	UPROPERTY()
		class UUserWidget* CurrentWidget;
};
