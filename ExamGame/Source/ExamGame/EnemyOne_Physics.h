// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyOne_Physics.generated.h"

UCLASS()
class EXAMGAME_API AEnemyOne_Physics : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyOne_Physics();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Sets PysicsAsset as blueprint value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		TSubclassOf<AActor> BP_EnemyOne;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		USkeletalMeshComponent* EnemyOne_Physics;


};
