// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxSpawner.h"

// Sets default values
ABoxSpawner::ABoxSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ABoxSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoxSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpawnBox();

}

void ABoxSpawner::SpawnBox()
{
	GetWorld()->SpawnActor<AActor>(BoxToSpawn, SpawnLocation, FRotator::ZeroRotator);
}