#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxSpawner.generated.h"

UCLASS()
class SPMG5_API ABoxSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ABoxSpawner();
	void SpawnBox(float DeltaTime);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float SpawnRate = 1;

	//UPROPERTY(EditAnywhere)
	//FVector SpawnLocation = FVector(50,50,120);

	float timer = SpawnRate;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BoxMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SpawnLocation;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> BoxToSpawn;


};
