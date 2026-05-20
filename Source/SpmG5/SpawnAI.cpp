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
	
	UE_LOG(LogTemp, Warning, TEXT("All boxes remaining: %d"), AmountOfBoxesPerLevel);
	for (const FBoxSpawnInfo& Info : Boxes)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("BoxType: %s | RemainingBoxes: %d | CurrentSpawnRate: %.2f | CountSinceLastSpawn: %d"),
			*UEnum::GetValueAsString(Info.BoxType),
			Info.RemainingBoxes,
			Info.CurrentSpawnRate,
			Info.CountSinceLastSpawn);
	}
	
	for (int i = 0; i < 1001; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Box %d: "), i);
		TArray<EBoxType> props = DecideProperties();
		if (AmountOfBoxesPerLevel > 0) AmountOfBoxesPerLevel--;
		for (EBoxType Type : props)
		{
			UE_LOG(LogTemp, Warning, TEXT("		: %d"), Type);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("All boxes remaining: %d"), AmountOfBoxesPerLevel);

	for (const FBoxSpawnInfo& Info : Boxes)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("BoxType: %s | RemainingBoxes: %d | CurrentSpawnRate: %.2f | CountSinceLastSpawn: %d"),
			*UEnum::GetValueAsString(Info.BoxType),
			Info.RemainingBoxes,
			Info.CurrentSpawnRate,
			Info.CountSinceLastSpawn);
	}
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
	
	//Set up all boxes of every enum
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
	}
}

void ASpawnAI::ConvertPercentageToBox(double Percentage, int& TypeOfRemainingBoxes, int DependencyFromAmount)
{
	double TempPercentage = Percentage;
	
	Percentage = DependencyFromAmount * (TempPercentage / 100.f);
	
	//UE_LOG(LogTemp, Warning, TEXT("			before round Percentage: %f"), Percentage);
	Percentage = FMath::RoundHalfFromZero(Percentage);
	//UE_LOG(LogTemp, Warning, TEXT("			after round Percentage: %f"), Percentage);
	
	TypeOfRemainingBoxes = Percentage;
}

double ASpawnAI::ConvertWeightToPercentage(double Weight, double TotalWeight)
{
	return Weight / TotalWeight * 100.0;
}

void ASpawnAI::SetUpSpawnRate(double& SpawnRate, int RemainingBoxType, int DependencyFromAmount)
{
	if (RemainingBoxType > 0)
	{
		SpawnRate = static_cast<double>(RemainingBoxType) / static_cast<double>(DependencyFromAmount);
		//UBoxSpawnRateManager->SpawnRate;
	}
	/*else
	{
		SpawnRate = 0.0;
	}*/
}

TArray<EBoxType> ASpawnAI::DecideProperties()
{
	TArray<EBoxType> Properties;
	
	//RollForProperty(EBoxType::Small) ? AddProperty(Properties, EBoxType::Small) : AddProperty(Properties, EBoxType::Large);
	//RollForProperty(EBoxType::Small) ? AddProperty(Properties, EBoxType::Small) : RollForProperty(EBoxType::Large) ? AddProperty(Properties, EBoxType::Large) : return Properties) return Properties;
	//AddProperty(Properties, EBoxType::Small);
	
	//Roll if small or large
	if (!RollForProperty(EBoxType::Small))
	{
		if (GetSpawnInfo(EBoxType::Large).RemainingBoxes <= 0)
		{
			if (GetSpawnInfo(EBoxType::Small).RemainingBoxes > 0)
			{
				AddProperty(Properties, EBoxType::Small);
			}else
			{
				return Properties; // No boxes left
			}
		}
		else
		{
			AddProperty(Properties, EBoxType::Large);
		}
	}
	else
	{
		AddProperty(Properties, EBoxType::Small);
	}
	
	//Roll if fragile
	if (!GuaranteeProperty(Properties, EBoxType::Fragile, AmountOfBoxesPerLevel))
		if (RollForProperty(EBoxType::Fragile)) AddProperty(Properties, EBoxType::Fragile);
	
	//Roll if suspicious
	if (!GuaranteeProperty(Properties, EBoxType::Suspicious, AmountOfBoxesPerLevel))
	{
		if (!RollForProperty(EBoxType::Suspicious)) return Properties;
		AddProperty(Properties, EBoxType::Suspicious);
	}
	

	if (!GuaranteeProperty(Properties, EBoxType::Dangerous, GetSpawnInfo(EBoxType::Suspicious).RemainingBoxes))
	{
		if (!RollForProperty(EBoxType::Dangerous)) return Properties;
		AddProperty(Properties, EBoxType::Dangerous);
	}
	
	
	double MaxSpawnRateDangerousBoxes = 0.0;
	
	for (FBoxSpawnInfo Box : Boxes)
	{
		if (DangerousTypes.Contains(Box.BoxType) && Box.RemainingBoxes > 0)
		{
			MaxSpawnRateDangerousBoxes += Box.CurrentSpawnRate;
		}
	}
	
	double BadBoxPercentage = GiveBadBoxesMaxPercentage(MaxSpawnRateDangerousBoxes);
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
	double Chance = GiveRandomPercentage();
	double SpawnRate = SpawnInfo.CurrentSpawnRate;
	
	if (Chance <= SpawnRate)
	{
		return true;
	}
	
	return false;
}

double ASpawnAI::GiveRandomPercentage()
{
	return FMath::RandRange(0.0, 1.0);
}

double ASpawnAI::GiveBadBoxesMaxPercentage(double MaxPercentage)
{
	return FMath::RandRange(0.0, MaxPercentage);
}

bool ASpawnAI::GuaranteeProperty(TArray<EBoxType>& Properties, EBoxType BoxType, int DependencyFromAmount)
{
	if (GetSpawnInfo(BoxType).RemainingBoxes == DependencyFromAmount && GetSpawnInfo(BoxType).RemainingBoxes > 0)
	{
		AddProperty(Properties, BoxType);
		return true;
	}
	return false;
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