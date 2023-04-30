// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"




UCLASS()
class EXAMGAME_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintNativeEvent)
		void Interact();

		virtual void Interact_Implementation();


	//Sett Up Mesh

		UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Interactable Properties")
			class UStaticMeshComponent* InteractableMesh;

	//TEXT

		UPROPERTY(EditAnywhere, Category = "Interactable Properties")
			FString InteractableHelpText;
};