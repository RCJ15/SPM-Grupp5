#include "BoxSpawner.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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
	LoopSpawnBox(SpawnRate);
}

void ABoxSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoxSpawner::SpawnBox()
{
	FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, SpawnLocation->GetComponentLocation());
	AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(BoxToSpawn, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	AItem* Item = Cast<AItem>(NewActor);
	
	if (Item)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawning Box"));
		Item->SetIsLarge(ShouldHappen(LargeBoxSpawnRate));
		Item->SetIsFragile(ShouldHappen(FragileBoxSpawnRate));
		Item->SetIsDangerous(ShouldHappen(DangerousBoxSpawnRate));
	}

	UGameplayStatics::FinishSpawningActor(NewActor, SpawnTransform);
}

void ABoxSpawner::LoopSpawnBox(float NewSpawnRate)
{
	GetWorld()->GetTimerManager().SetTimer(
		SpawnRateTimer,
		this,
		&ABoxSpawner::SpawnBox,
		NewSpawnRate,
		true
		);
}

bool ABoxSpawner::ShouldHappen(int percentage)
{
	return (FMath::RandRange(1, 100/percentage) == 1 ? true : false);
}

/*void ABoxSpawner::RetRandomNumber(int32& Number)
{
	TArray<int32> Arr;
	int32 RandomInt;
	int32 ArrayGetItem;
	
	

	RandomInt = UKismetMathLibrary::RandomIntegerInRange(0, 99);
	FCustomThunkTemplates::Array_Get(Arr, RandomInt, ArrayGetItem);
	Number = ArrayGetItem;
}*/