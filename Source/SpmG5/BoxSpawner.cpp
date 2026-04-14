#include "BoxSpawner.h"

ABoxSpawner::ABoxSpawner()
{
 	PrimaryActorTick.bCanEverTick = true;
	
	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	SetRootComponent(BoxMesh);

	SpawnLocation = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnLocation"));
	SpawnLocation->SetupAttachment(RootComponent);

	//SpawnLocation += GetActorLocation();

}

void ABoxSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

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