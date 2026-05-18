// Marcus hopefully approves of this.


#include "SpawnAI.h"

// Sets default values
ASpawnAI::ASpawnAI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpawnAI::BeginPlay()
{
	Super::BeginPlay();
	
	ConvertAllPercentageToBoxes();
	UE_LOG(LogTemp, Warning, TEXT("Small boxes: %f"), SmallBoxesPercentage);
	UE_LOG(LogTemp, Warning, TEXT("Small boxes remaining: %d"), RemainingSmallBoxes);
	UE_LOG(LogTemp, Warning, TEXT("Large boxes: %f"), LargeBoxesPercentage);
	UE_LOG(LogTemp, Warning, TEXT("Large boxes remaining: %d"), RemainingLargeBoxes);
	UE_LOG(LogTemp, Warning, TEXT("Fragile boxes: %f"), FragileBoxesPercentage);
	UE_LOG(LogTemp, Warning, TEXT("Fragile boxes remaining: %d"), RemainingFragileBoxes);
	UE_LOG(LogTemp, Warning, TEXT("Suspicious boxes: %f"), SuspiciousBoxesPercentage);
	UE_LOG(LogTemp, Warning, TEXT("Suspicious boxes remaining: %d"), RemainingSuspiciousBoxes);
	UE_LOG(LogTemp, Warning, TEXT("Dangerous boxes: %f"), DangerousBoxesPercentage);
	UE_LOG(LogTemp, Warning, TEXT("Dangerous boxes remaining: %d"), RemainingDangerousBoxes);
	UE_LOG(LogTemp, Warning, TEXT("Bomb boxes: %f"), BombBoxesWeight);
	UE_LOG(LogTemp, Warning, TEXT("Bomb boxes remaining: %d"), RemainingBombBoxes);
	UE_LOG(LogTemp, Warning, TEXT("ToxicWaste boxes: %f"), ToxicWasteBoxesWeight);
	UE_LOG(LogTemp, Warning, TEXT("ToxicWaste boxes remaining: %d"), RemainingToxicWasteBoxes);
	UE_LOG(LogTemp, Warning, TEXT("FlashBang boxes: %f"), FlashBangBoxesWeight);
	UE_LOG(LogTemp, Warning, TEXT("FlashBang boxes remaining: %d"), RemainingFlashBangBoxes);
}

// Called every frame
void ASpawnAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawnAI::ConvertAllPercentageToBoxes()
{
	ConvertPercentageToBox(SmallBoxesPercentage, RemainingSmallBoxes, AmountOfBoxesPerLevel);
	LargeBoxesPercentage = 100 - SmallBoxesPercentage;
	ConvertPercentageToBox(LargeBoxesPercentage, RemainingLargeBoxes, AmountOfBoxesPerLevel);
	
	ConvertPercentageToBox(FragileBoxesPercentage, RemainingFragileBoxes, AmountOfBoxesPerLevel);
	
	ConvertPercentageToBox(SuspiciousBoxesPercentage, RemainingSuspiciousBoxes, AmountOfBoxesPerLevel);
	ConvertPercentageToBox(DangerousBoxesPercentage, RemainingDangerousBoxes, RemainingSuspiciousBoxes);
	
	ConvertWeightToBox(BombBoxesWeight, RemainingBombBoxes, RemainingDangerousBoxes);
	ConvertWeightToBox(ToxicWasteBoxesWeight, RemainingToxicWasteBoxes, RemainingDangerousBoxes);
	ConvertWeightToBox(FlashBangBoxesWeight, RemainingFlashBangBoxes, RemainingDangerousBoxes);
	
	SetUpSpawnRate(FragileBoxesSpawnRate, RemainingFragileBoxes);
	SetUpSpawnRate(LargeBoxesSpawnRate, RemainingLargeBoxes);
	SetUpSpawnRate(SuspiciousBoxesSpawnRate, RemainingSuspiciousBoxes);
	SetUpSpawnRate(FragileBoxesSpawnRate, RemainingFragileBoxes);
	SetUpSpawnRate(BombBoxesSpawnRate, RemainingBombBoxes);
	SetUpSpawnRate(FragileBoxesSpawnRate, RemainingFragileBoxes);
	SetUpSpawnRate(FragileBoxesSpawnRate, RemainingFragileBoxes);
}

void ASpawnAI::ConvertPercentageToBox(float& Percentage, int& TypeOfRemainingBoxes, int DependencyFromAmount)
{
	float TempPercentage = Percentage;
	Percentage = DependencyFromAmount * (TempPercentage / 100);
	TypeOfRemainingBoxes = Percentage;
}

void ASpawnAI::ConvertWeightToBox(float& Weight, int& TypeOfRemainingBoxes, int DependencyFromAmount)
{
	float TotalWeight = BombBoxesWeight + ToxicWasteBoxesWeight + FlashBangBoxesWeight;
	TArray BadBoxes = {BombBoxesWeight, ToxicWasteBoxesWeight, FlashBangBoxesWeight};
	
	for (int i = 0; i < BadBoxes.Num(); i++)
	{
		Weight = BadBoxes[i]/TotalWeight * 100;
	}
	
	ConvertPercentageToBox(Weight, TypeOfRemainingBoxes, DependencyFromAmount);
}

void ASpawnAI::SetUpSpawnRate(int& SpawnRate, int BoxType)
{
	
}
