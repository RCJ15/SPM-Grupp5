// Marcus hopefully approves of this.


#include "BoxSpawnRateManager.h"
#include "SpawnAI.h"

#include "DiffUtils.h"
#include "RenderCore.h"
#include "AI/NavigationModifier.h"
#include "Chaos/Deformable/ChaosDeformableCollisionsProxy.h"

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
	
	//UE_LOG(LogTemp, Warning, TEXT("All boxes remaining: %d"), AmountOfBoxesPerLevel);
	for (const FBoxSpawnInfo& Info : Boxes)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("BoxType: %s | RemainingBoxes: %d | CurrentSpawnRate: %.2f | CountSinceLastSpawn: %d"),
			*UEnum::GetValueAsString(Info.BoxType),
			Info.RemainingBoxes,
			Info.CurrentSpawnRate,
			Info.CountSinceLastSpawn);
	}
	for (int i = 0; i < 10; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Box %d: "), i);
		TArray<EBoxType> props = DecideProperties();
		for (EBoxType Type : props)
		{
			UE_LOG(LogTemp, Warning, TEXT("		: %d"), Type);
		}
	}
	for (const FBoxSpawnInfo& Info : Boxes)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("BoxType: %s | RemainingBoxes: %d | CurrentSpawnRate: %.2f | CountSinceLastSpawn: %d"),
			*UEnum::GetValueAsString(Info.BoxType),
			Info.RemainingBoxes,
			Info.CurrentSpawnRate,
			Info.CountSinceLastSpawn);
	}
	/*
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
	UE_LOG(LogTemp, Warning, TEXT("FlashBang boxes remaining: %d"), RemainingFlashBangBoxes);*/
}

// Called every frame
void ASpawnAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawnAI::ConvertAllPercentageToBoxes()
{
	double TotalWeight = 0;
	for (const TPair<EBoxType, double>& Pair : DangerousTypes)
	{
		TotalWeight += Pair.Value;
	}
	
	for (EBoxType BoxType : AllBoxTypes)
	{
		FBoxSpawnInfo BoxSpawnInfo = FBoxSpawnInfo(BoxType);
		int RemainingBoxes;
		double SpawnRate = 0;
		if (DangerousTypes.Contains(BoxType))
		{
			double Percentage = ConvertWeightToPercentage(DangerousTypes[BoxType], TotalWeight);
			ConvertPercentageToBox(Percentage, RemainingBoxes, GetSpawnInfo(EBoxType::Dangerous).RemainingBoxes);
			SetUpSpawnRate(SpawnRate, RemainingBoxes, GetSpawnInfo(EBoxType::Dangerous).RemainingBoxes); 
		}
		else
		{
			BoxType == EBoxType::Dangerous ? ConvertPercentageToBox(SpawnRates[BoxType], RemainingBoxes, GetSpawnInfo(EBoxType::Suspicious).RemainingBoxes) : ConvertPercentageToBox(SpawnRates[BoxType], RemainingBoxes, AmountOfBoxesPerLevel);
			BoxType == EBoxType::Dangerous ? SetUpSpawnRate(SpawnRate, RemainingBoxes, GetSpawnInfo(EBoxType::Suspicious).RemainingBoxes) : SetUpSpawnRate(SpawnRate, RemainingBoxes, AmountOfBoxesPerLevel); 
		}
		BoxSpawnInfo.RemainingBoxes = RemainingBoxes;
		BoxSpawnInfo.CurrentSpawnRate = SpawnRate;
		Boxes.Add(BoxSpawnInfo);
		if (BoxType == EBoxType::Small)
		{
			FBoxSpawnInfo LargeSpawnInfo = FBoxSpawnInfo(EBoxType::Large);
			LargeSpawnInfo.RemainingBoxes = AmountOfBoxesPerLevel-RemainingBoxes;
			SetUpSpawnRate(SpawnRate, LargeSpawnInfo.RemainingBoxes, AmountOfBoxesPerLevel);
			LargeSpawnInfo.CurrentSpawnRate = SpawnRate;
			Boxes.Add(LargeSpawnInfo);
		}
		//if (DangerousTypes.Contains(BoxType));
	}
	
	/*
	LargeBoxesPercentage = 100 - SmallBoxesPercentage;
	ConvertPercentageToBox(SmallBoxesPercentage, RemainingSmallBoxes, AmountOfBoxesPerLevel);
	
	
	
	ConvertPercentageToBox(LargeBoxesPercentage, RemainingLargeBoxes, AmountOfBoxesPerLevel);
	ConvertPercentageToBox(FragileBoxesPercentage, RemainingFragileBoxes, AmountOfBoxesPerLevel);
	ConvertPercentageToBox(SuspiciousBoxesPercentage, RemainingSuspiciousBoxes, AmountOfBoxesPerLevel);
	ConvertPercentageToBox(DangerousBoxesPercentage, RemainingDangerousBoxes, RemainingSuspiciousBoxes);
	
	ConvertWeightToBox();
	
	/*UE_LOG(LogTemp, Warning, TEXT("Percentage small box: %f"), SmallBoxesPercentage);
	UE_LOG(LogTemp, Warning, TEXT("Percentage bombs: %f"), BombBoxesWeight);*/
	/*
	ConvertPercentageToBox(BombBoxesWeight, RemainingBombBoxes, RemainingDangerousBoxes);
	ConvertPercentageToBox(ToxicWasteBoxesWeight, RemainingToxicWasteBoxes, RemainingDangerousBoxes);
	ConvertPercentageToBox(FlashBangBoxesWeight, RemainingFlashBangBoxes, RemainingDangerousBoxes);
	
	//UBoxSpawnRateManager* agjksg = GetWorld()->  GetBoxSpawnRateManager();
	
	SetUpSpawnRate(SmallBoxesSpawnRate, RemainingSmallBoxes, AmountOfBoxesPerLevel);
	SetUpSpawnRate(LargeBoxesSpawnRate, RemainingLargeBoxes, AmountOfBoxesPerLevel);
	SetUpSpawnRate(FragileBoxesSpawnRate, RemainingFragileBoxes, AmountOfBoxesPerLevel);
	SetUpSpawnRate(SuspiciousBoxesSpawnRate, RemainingSuspiciousBoxes, AmountOfBoxesPerLevel);
	SetUpSpawnRate(DangerousBoxesSpawnRate, RemainingDangerousBoxes, RemainingSuspiciousBoxes);
	SetUpSpawnRate(BombBoxesSpawnRate, RemainingBombBoxes, RemainingDangerousBoxes);
	SetUpSpawnRate(ToxicWasteBoxesSpawnRate, RemainingToxicWasteBoxes, RemainingDangerousBoxes);
	SetUpSpawnRate(FlashBangBoxesSpawnRate, RemainingFlashBangBoxes, RemainingDangerousBoxes);
	*/
}

void ASpawnAI::ConvertPercentageToBox(double Percentage, int& TypeOfRemainingBoxes, int DependencyFromAmount)
{
	double TempPercentage = Percentage;
	
	Percentage = (DependencyFromAmount * (TempPercentage / 100.f));
	
	//UE_LOG(LogTemp, Warning, TEXT("			before round Percentage: %f"), Percentage);
	Percentage = FMath::RoundHalfFromZero(Percentage);
	//UE_LOG(LogTemp, Warning, TEXT("			after round Percentage: %f"), Percentage);
	
	TypeOfRemainingBoxes = Percentage;
}

double ASpawnAI::ConvertWeightToPercentage(double Weight, double TotalWeight)
{
	return Weight / TotalWeight * 100.0;
}

void ASpawnAI::SetUpSpawnRate(double& SpawnRate, int BoxType, int DependencyFromAmount)
{
	if (BoxType > 0)
	{
		SpawnRate = static_cast<double>(BoxType) / static_cast<double>(DependencyFromAmount);
		//UBoxSpawnRateManager->SpawnRate;
	}
}

TArray<EBoxType> ASpawnAI::DecideProperties()
{
	TArray<EBoxType> Properties;
	
	RollForProperty(EBoxType::Small) ? AddProperty(Properties, EBoxType::Small) : AddProperty(Properties, EBoxType::Large);
	if (RollForProperty(EBoxType::Fragile)) AddProperty(Properties, EBoxType::Fragile);
	
	if (!RollForProperty(EBoxType::Suspicious)) return Properties;
	AddProperty(Properties, EBoxType::Suspicious);
		
	if (!RollForProperty(EBoxType::Dangerous)) return Properties; 
	AddProperty(Properties, EBoxType::Dangerous);
	
	double BadBoxPercentage = GivePercentage();
	double CurrentRate = 0.0;
	
	for (FBoxSpawnInfo Box : Boxes)
	{
		if (DangerousTypes.Contains(Box.BoxType) && Box.RemainingBoxes > 0)
		{
			// Checks which bad box should be added
			if (BadBoxPercentage >= CurrentRate && BadBoxPercentage <= Box.CurrentSpawnRate + CurrentRate)
			{
				AddProperty(Properties, Box.BoxType);
				break;
			}
			
			CurrentRate += Box.CurrentSpawnRate;
		}
	}
	return Properties;
}

bool ASpawnAI::RollForProperty(EBoxType BoxType)
{
	FBoxSpawnInfo SpawnInfo = GetSpawnInfo(BoxType);
	if (SpawnInfo.RemainingBoxes <= 0) return false;
	double Chance = GivePercentage();
	double SpawnRate = SpawnInfo.CurrentSpawnRate;
	
	if (Chance <= SpawnRate)
	{
		return true;
	}
	
	return false;
}

bool ASpawnAI::SetProperties()
{
	
	
	return true;
}

double ASpawnAI::GivePercentage()
{
	return FMath::RandRange(0.0, 1.0);
}

FBoxSpawnInfo& ASpawnAI::GetSpawnInfo(EBoxType Type)
{
	for (FBoxSpawnInfo& Box : Boxes)
	{
		if (Box.BoxType == Type) return Box;
	}
	return Boxes[0];
}

void ASpawnAI::AddProperty(TArray<EBoxType>& Properties, EBoxType Type)
{
	Properties.Add(Type);
	GetSpawnInfo(Type).DecrementBoxCount();

}


