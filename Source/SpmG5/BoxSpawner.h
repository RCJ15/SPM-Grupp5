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
	void SpawnBox(float DeltaTime);
	
private:
	static int SpawnedBoxes;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float SpawnRate = 1;

	//UPROPERTY(EditAnywhere)
	//FVector SpawnLocation = FVector(50,50,120);

	float Timer = SpawnRate;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BoxMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SpawnLocation;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> BoxToSpawn;

	UPROPERTY(EditAnywhere)
	float LargeBoxSpawnRate = 5;
	
	UPROPERTY(EditAnywhere)
	float FragileBoxSpawnRate = 10;
	
	static bool ShouldHappen(int percentage);
};
