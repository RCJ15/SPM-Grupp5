// Marcus hopefully approves of this.


#include "SpawnAI.h"

#include "RenderCore.h"

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
	
	UE_LOG(LogTemp, Warning, TEXT("All boxes remaining: %d"), AmountOfBoxesPerLevel);
	UE_LOG(LogTemp, Warning, TEXT("Small boxes remaining: %d"), RemainingSmallBoxes);
	UE_LOG(LogTemp, Warning, TEXT("Large boxes remaining: %d"), RemainingLargeBoxes);
	UE_LOG(LogTemp, Warning, TEXT("Fragile boxes remaining: %d"), RemainingFragileBoxes);
	UE_LOG(LogTemp, Warning, TEXT("Suspicious boxes remaining: %d"), RemainingSuspiciousBoxes);
	UE_LOG(LogTemp, Warning, TEXT("Dangerous boxes remaining: %d"), RemainingDangerousBoxes);
	UE_LOG(LogTemp, Warning, TEXT("Bomb boxes remaining: %d"), RemainingBombBoxes);
	UE_LOG(LogTemp, Warning, TEXT("ToxicWaste boxes remaining: %d"), RemainingToxicWasteBoxes);
	UE_LOG(LogTemp, Warning, TEXT("FlashBang boxes remaining: %d"), RemainingFlashBangBoxes);
	
	UE_LOG(LogTemp, Warning, TEXT("Small boxes spawn rate: %f"), SmallBoxesSpawnRate);
	UE_LOG(LogTemp, Warning, TEXT("Large boxes spawn rate: %f"), LargeBoxesSpawnRate);
	UE_LOG(LogTemp, Warning, TEXT("Fragile boxes spawn rate: %f"), FragileBoxesSpawnRate);
	UE_LOG(LogTemp, Warning, TEXT("Suspicious boxes spawn rate: %f"), SuspiciousBoxesSpawnRate);
	UE_LOG(LogTemp, Warning, TEXT("Dangerous boxes spawn rate: %f"), DangerousBoxesSpawnRate);
	UE_LOG(LogTemp, Warning, TEXT("Bomb boxes spawn rate: %f"), BombBoxesSpawnRate);
	UE_LOG(LogTemp, Warning, TEXT("ToxicWaste boxes spawn rate: %f"), ToxicWasteBoxesSpawnRate);
	UE_LOG(LogTemp, Warning, TEXT("FlashBang boxes spawn rate: %f"), FlashBangBoxesSpawnRate);
	
	/*UE_LOG(LogTemp, Warning, TEXT("Small boxes: %f"), SmallBoxesPercentage);
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
	UE_LOG(LogTemp, Warning, TEXT("FlashBang boxes remaining: %d"), RemainingFlashBangBoxes);*/
}

// Called every frame
void ASpawnAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawnAI::ConvertAllPercentageToBoxes()
{
	LargeBoxesPercentage = 100 - SmallBoxesPercentage;
	ConvertPercentageToBox(SmallBoxesPercentage, RemainingSmallBoxes, AmountOfBoxesPerLevel);
	ConvertPercentageToBox(LargeBoxesPercentage, RemainingLargeBoxes, AmountOfBoxesPerLevel);
	ConvertPercentageToBox(FragileBoxesPercentage, RemainingFragileBoxes, AmountOfBoxesPerLevel);
	ConvertPercentageToBox(SuspiciousBoxesPercentage, RemainingSuspiciousBoxes, AmountOfBoxesPerLevel);
	ConvertPercentageToBox(DangerousBoxesPercentage, RemainingDangerousBoxes, RemainingSuspiciousBoxes);
	
	ConvertWeightToBox();
	
	/*UE_LOG(LogTemp, Warning, TEXT("Percentage small box: %f"), SmallBoxesPercentage);
	UE_LOG(LogTemp, Warning, TEXT("Percentage bombs: %f"), BombBoxesWeight);*/
	
	ConvertPercentageToBox(BombBoxesWeight, RemainingBombBoxes, RemainingDangerousBoxes);
	ConvertPercentageToBox(ToxicWasteBoxesWeight, RemainingToxicWasteBoxes, RemainingDangerousBoxes);
	ConvertPercentageToBox(FlashBangBoxesWeight, RemainingFlashBangBoxes, RemainingDangerousBoxes);
	
	SetUpSpawnRate(SmallBoxesSpawnRate, RemainingSmallBoxes, AmountOfBoxesPerLevel);
	SetUpSpawnRate(LargeBoxesSpawnRate, RemainingLargeBoxes, AmountOfBoxesPerLevel);
	SetUpSpawnRate(FragileBoxesSpawnRate, RemainingFragileBoxes, AmountOfBoxesPerLevel);
	SetUpSpawnRate(SuspiciousBoxesSpawnRate, RemainingSuspiciousBoxes, AmountOfBoxesPerLevel);
	SetUpSpawnRate(DangerousBoxesSpawnRate, RemainingDangerousBoxes, RemainingSuspiciousBoxes);
	SetUpSpawnRate(BombBoxesSpawnRate, RemainingBombBoxes, RemainingDangerousBoxes);
	SetUpSpawnRate(ToxicWasteBoxesSpawnRate, RemainingToxicWasteBoxes, RemainingDangerousBoxes);
	SetUpSpawnRate(FlashBangBoxesSpawnRate, RemainingFlashBangBoxes, RemainingDangerousBoxes);
}

void ASpawnAI::ConvertPercentageToBox(double& Percentage, int& TypeOfRemainingBoxes, int DependencyFromAmount)
{
	double TempPercentage = Percentage;
	
	Percentage = (DependencyFromAmount * (TempPercentage / 100.f));
	
	UE_LOG(LogTemp, Warning, TEXT("			before round Percentage: %f"), Percentage);
	Percentage = FMath::RoundHalfFromZero(Percentage);
	UE_LOG(LogTemp, Warning, TEXT("			after round Percentage: %f"), Percentage);
	
	TypeOfRemainingBoxes = Percentage;
}

void ASpawnAI::ConvertWeightToBox()
{
	double TotalWeight = BombBoxesWeight + ToxicWasteBoxesWeight + FlashBangBoxesWeight;
	TArray BadBoxes = {&BombBoxesWeight, &ToxicWasteBoxesWeight, &FlashBangBoxesWeight};
	
	for (int i = 0; i < BadBoxes.Num(); i++)
	{
		double TempWeight = *BadBoxes[i];
		*BadBoxes[i] = (TempWeight/TotalWeight * 100.0);
	}
	
	/*UE_LOG(LogTemp, Warning, TEXT("After loop Percentage: %f"), BombBoxesWeight);
	UE_LOG(LogTemp, Warning, TEXT("After loop Percentage: %f"), ToxicWasteBoxesWeight);
	UE_LOG(LogTemp, Warning, TEXT("After loop Percentage: %f"), FlashBangBoxesWeight);*/
}

void ASpawnAI::SetUpSpawnRate(double& SpawnRate, int BoxType, int DependencyFromAmount)
{
	if (BoxType > 0)
	{
		SpawnRate = static_cast<double>(DependencyFromAmount) / static_cast<double>(BoxType);
	}
}