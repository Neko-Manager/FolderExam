// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Charachter_Pawn.generated.h"

//Forward Declare classes


UCLASS()
class EXAMGAME_API ACharachter_Pawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACharachter_Pawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ------------------------ Verriables ----------------------------
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Space ship Variables")
	float MovementSpeed;




	// ------------------------ Components ----------------------------

	//Main Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Components")
	class UStaticMeshComponent* PlayerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Components")
	class USpringArmComponent* SpringArm;

	//Input Components

	//Audio Components


	// ------------------------ Functions ----------------------------



};
