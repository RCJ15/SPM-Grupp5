// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxSpawner.generated.h"

UCLASS()
class SPMG5_API ABoxSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoxSpawner();
	void SpawnBox(float DeltaTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float SpawnRate = 1;

	float timer = SpawnRate;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BoxMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SpawnLocation;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> BoxToSpawn;


};
