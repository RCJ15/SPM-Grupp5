#include "BoxSpawner.h"

#include "Bomb.h"
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
	
	if (SpawnOnPoint)
		LoopSpawnBox(SpawnRate);
}

void ABoxSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AItem* ABoxSpawner::SpawnItem()
{
	FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, SpawnLocation->GetComponentLocation());
	
	TSubclassOf<AActor> ItemToSpawn = BoxToSpawn;
	bool IsSuspicious = ShouldHappen(SuspiciousBoxSpawnRate);
	bool IsDangerous = false;
	
	if (IsSuspicious)
	{
		IsDangerous = ShouldHappen(DangerousBoxSpawnRate);
		
		if (IsDangerous)
		{
			ItemToSpawn = BombToSpawn;
			UE_LOG(LogTemp, Warning, TEXT("Dangerous box should spawn!!!!"));
		}
	}
	
	// Sets all properties of an item before spawning it
	AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(ItemToSpawn, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	AItem* Item = Cast<AItem>(NewActor);
	
	if (Item)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Spawning Box"));
		Item->SetIsLarge(ShouldHappen(LargeBoxSpawnRate));
		Item->SetIsFragile(ShouldHappen(FragileBoxSpawnRate));
		//Item->SetIsDangerous(ShouldHappen(DangerousBoxSpawnRate));
		//Item->SetActorRotation(SpawnLocation->GetComponentRotation() + FRotator(0, FMath::RandRange(-15,15), 0));
		Item->SetIsSuspicious(IsSuspicious);
		Item->SetIsDangerous(IsDangerous);
		Item->SetAddress(SetBoxAddress());
		
		Item->SetPlaySound(PlayBoxSound);
	}

	// Actually spawn item
	UGameplayStatics::FinishSpawningActor(Item, SpawnTransform);
	return Item;
}

AItem* ABoxSpawner::SpawnBox()
{
	return SpawnItem();
}

AItem* ABoxSpawner::SpawnItem(bool IsDangerous, bool IsLarge, bool IsFragile, bool IsSuspicious, BoxAddress Address)
{
	FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, SpawnLocation->GetComponentLocation());
	
	TSubclassOf<AActor> ItemToSpawn = BoxToSpawn;
	
	if (IsSuspicious)
	{
		IsDangerous = ShouldHappen(DangerousBoxSpawnRate);
		
		if (IsDangerous)
		{
			ItemToSpawn = BombToSpawn;
			UE_LOG(LogTemp, Warning, TEXT("Dangerous box should spawn!!!!"));
		}
	}
	
	// Sets all properties of an item before spawning it
	AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(ItemToSpawn, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	AItem* Item = Cast<AItem>(NewActor);
	
	if (Item)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Spawning Box"));
		Item->SetIsLarge(IsLarge);
		Item->SetIsFragile(IsFragile);
		//Item->SetIsDangerous(ShouldHappen(DangerousBoxSpawnRate));
		//Item->SetActorRotation(SpawnLocation->GetComponentRotation() + FRotator(0, FMath::RandRange(-15,15), 0));
		Item->SetIsSuspicious(IsSuspicious);
		Item->SetIsDangerous(IsDangerous);
		Item->SetAddress(Address);
		Item->SetPlaySound(PlayBoxSound);
	}

	// Actually spawn item
	UGameplayStatics::FinishSpawningActor(Item, SpawnTransform);
	return Item;
}

void ABoxSpawner::SpawnBoxOnPoint()
{
	SpawnItem();
}

void ABoxSpawner::LoopSpawnBox(float NewSpawnRate)
{
	GetWorld()->GetTimerManager().SetTimer(
		SpawnRateTimer,
		this,
		&ABoxSpawner::SpawnBoxOnPoint,
		NewSpawnRate,
		true
		);
}

bool ABoxSpawner::ShouldHappen(int Percentage)
{
	if (Percentage <= 0)
	{
		return false;
	}
	return (FMath::RandRange(1, 100/Percentage) == 1 ? true : false);
}

// I hate this solution
BoxAddress ABoxSpawner::SetBoxAddress()
{
	int number = FMath::RandRange(0, 2);
	
	switch (number)
	{
		case 0:
			return BoxAddress::CIRCLE;
		case 1:
			return BoxAddress::SQUARE;
		// case 2:
		// 	return BoxAddress::TRIANGLE;
		default:
			return BoxAddress::CIRCLE;
	}
	
	//uint8 hks = FMath::RandHelper(BoxAddress::TOTAL_COUNT);
	// return (FMath::RandRange(1, 100/Percentage) == 1 ? true : false)
}
