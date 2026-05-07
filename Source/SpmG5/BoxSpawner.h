#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.h"
#include "BoxSpawner.generated.h"

UCLASS()
class SPMG5_API ABoxSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ABoxSpawner();
	
private:
	static int SpawnedBoxes;

protected:
	virtual void BeginPlay() override;

	AItem* SpawnItem();
	
	UPROPERTY(EditAnywhere)
	float SpawnRate = 1;
	
	FTimerHandle SpawnRateTimer;

	//UPROPERTY(EditAnywhere)
	//FVector SpawnLocation = FVector(50,50,120);

public:	
	virtual void Tick(float DeltaTime) override;

	//UFUNCTION(BlueprintImplementableEvent)
	AItem* SpawnBox();
	void SpawnBoxOnPoint();
	
	void LoopSpawnBox(float NewSpawnRate);
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BoxMesh;

	UPROPERTY(EditAnywhere)
	bool SpawnOnPoint;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SpawnLocation;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> BoxToSpawn;
	
	// UPROPERTY(EditAnywhere)
	// AActor* ItemToSpawn;

	UPROPERTY(EditAnywhere)
	float LargeBoxSpawnRate = 5;
	
	UPROPERTY(EditAnywhere)
	float FragileBoxSpawnRate = 10;
	
	UPROPERTY(EditAnywhere)
	float SuspiciousBoxSpawnRate = 10;
	
	static bool ShouldHappen(int percentage);
	
	BoxAddress SetBoxAddress();
};
