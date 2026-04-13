// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxSpawner.h"

// Sets default values
ABoxSpawner::ABoxSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	RootComponent = BoxMesh;
	SpawnLocation = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnLocation"));
	SpawnLocation->SetupAttachment(RootComponent);
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

	SpawnBox(DeltaTime);

}

void ABoxSpawner::SpawnBox(float DeltaTime)
{
	if (timer <= 0)
	{
		GetWorld()->SpawnActor<AActor>(BoxToSpawn, SpawnLocation->GetComponentLocation(), FRotator::ZeroRotator);
		timer = SpawnRate;
	}
	timer -= DeltaTime;
}