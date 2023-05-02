

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyTwo_Physics.generated.h"

UCLASS()
class EXAMGAME_API AEnemyTwo_Physics : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyTwo_Physics();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Sets PysicsAsset as blueprint value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		TSubclassOf<AActor> BP_EnemyTwo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		USkeletalMeshComponent* EnemyTwo_Physics;


};
