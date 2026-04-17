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
	if (Timer <= 0)
	{
		AItem* item = Cast<AItem>(BoxToSpawn);
		
		if (item)
		{
			item->SetIsLarge(ShouldHappen(LargeBoxSpawnRate));
			item->SetIsFragile(ShouldHappen(FragileBoxSpawnRate));
            
            GetWorld()->SpawnActor<AItem>(BoxToSpawn, SpawnLocation->GetComponentLocation(), FRotator::ZeroRotator);
            Timer = SpawnRate;
		}
	}
	Timer -= DeltaTime;
}

bool ABoxSpawner::ShouldHappen(int percentage)
{
	return (FMath::RandRange(1, 100/percentage) == 1 ? true : false);
}