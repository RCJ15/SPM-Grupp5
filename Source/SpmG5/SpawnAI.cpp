// Marcus hopefully approves of this.

#include "SpawnAI.h"
#include "BoxSpawnRateManager.h"


void USpawnAI::SetupSpawner(int InAmountOfBoxesPerLevel, TMap<EBoxType, double> InSpawnRates, TMap<EBoxType, double> InDangerousTypes, TMap<EBoxType, double> InAddressTypes)
{
	Boxes.Empty();
	
	TotalBoxCount = InAmountOfBoxesPerLevel;
	SpawnRates = InSpawnRates;
	DangerousTypes = InDangerousTypes;
	AddressTypes = InAddressTypes;
	
	UE_LOG(LogTemp, Warning, TEXT("Total boxes: %d"), TotalBoxCount);
	
	ConvertAllPercentageToBoxes();
	
	/*UE_LOG(LogTemp, Warning, TEXT("All boxes remaining: %d"), TotalBoxCount);
	for (const FBoxSpawnInfo& Info : Boxes)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("BoxType: %s | RemainingBoxes: %d | CurrentSpawnRate: %.2f | CountSinceLastSpawn: %d"),
			*UEnum::GetValueAsString(Info.BoxType),
			Info.RemainingBoxes,
			Info.CurrentSpawnRate,
			Info.CountSinceLastSpawn);
	}
	
	for (int i = 0; i < 40; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Box %d: "), i);
		TArray<EBoxType> props = ConstructBox();
		if (TotalBoxCount > 0) TotalBoxCount--;
		
		for (EBoxType Type : props)
		{
			UE_LOG(LogTemp, Warning, TEXT("		: %d"), Type);
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
	}*/
}

TArray<EBoxType> USpawnAI::ConstructBox()
{
	// Set up spawn rates for all boxes
	for (FBoxSpawnInfo& Box : Boxes)
	{
		if (DangerousTypes.Contains(Box.BoxType))
		{
			SetUpSpawnRate(Box.CurrentSpawnRate, Box.RemainingBoxes, GetSpawnInfo(EBoxType::Dangerous).RemainingBoxes);
			continue;
		}
		if (Box.BoxType == EBoxType::Dangerous)
		{
			SetUpSpawnRate(Box.CurrentSpawnRate, Box.RemainingBoxes, GetSpawnInfo(EBoxType::Suspicious).RemainingBoxes);
			continue;
		}
		
		SetUpSpawnRate(Box.CurrentSpawnRate, Box.RemainingBoxes, TotalBoxCount);
		
		if (Box.BoxType == EBoxType::Radioactive)
		{
			UE_LOG(LogTemp, Warning, TEXT("radioactive spawn rate: %f"), Box.CurrentSpawnRate);
		}
	}
	
	TArray<EBoxType> Properties = DecideProperties();
	
	// Increase spawn rate probability for box types not chosen
	for (EBoxType BoxType : AllBoxTypes)
	{
		if (!Properties.Contains(BoxType)) GetSpawnInfo(BoxType).IncreaseCounter();
	}
	
	if (TotalBoxCount > 0)
	{
		TotalBoxCount--;
	}
	
	return Properties;
}

TArray<EBoxType> USpawnAI::DecideProperties()
{
	TArray<EBoxType> Properties;
	if (Boxes.IsEmpty()) return Properties;
	
	// Roll for what address type
	CheckAndAddPropertiesForWeights(AddressTypes, Properties);
	
	// Roll if small or large
	if (!RollForProperty(EBoxType::Small))
	{
		if (GetSpawnInfo(EBoxType::Large).RemainingBoxes <= 0)
		{
			if (GetSpawnInfo(EBoxType::Small).RemainingBoxes > 0)
			{
				AddProperty(Properties, EBoxType::Small);
			}
			else
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
	
	// Roll if fragile
	if (!GuaranteeProperty(Properties, EBoxType::Fragile, TotalBoxCount))
		if (RollForProperty(EBoxType::Fragile)) AddProperty(Properties, EBoxType::Fragile);
	
	// Roll if radioactive
	if (!GuaranteeProperty(Properties, EBoxType::Radioactive, TotalBoxCount))
		if (RollForProperty(EBoxType::Radioactive)) AddProperty(Properties, EBoxType::Radioactive);
	
	// Roll if suspicious
	if (!GuaranteeProperty(Properties, EBoxType::Suspicious, TotalBoxCount))
	{
		if (!RollForProperty(EBoxType::Suspicious)) return Properties;
		AddProperty(Properties, EBoxType::Suspicious);
	}
	
	// Roll if dangerous
	if (!GuaranteeProperty(Properties, EBoxType::Dangerous, GetSpawnInfo(EBoxType::Suspicious).RemainingBoxes))
	{
		if (!RollForProperty(EBoxType::Dangerous)) return Properties;
		AddProperty(Properties, EBoxType::Dangerous);
	}
	
	// Roll for what dangerous type
	CheckAndAddPropertiesForWeights(DangerousTypes, Properties);
	
	return Properties;
}

void USpawnAI::CheckAndAddPropertiesForWeights(TMap<EBoxType, double> AllBoxesOfType, TArray<EBoxType>& Properties)
{
	double MaxSpawnRate = 0.0;
	
	for (FBoxSpawnInfo Info : Boxes)
	{
		if (AllBoxesOfType.Contains(Info.BoxType) && Info.RemainingBoxes > 0)
		{
			MaxSpawnRate += Info.CurrentSpawnRate;
		}
	}
	
	double Percentage = GiveWeightedBoxesMaxPercentage(MaxSpawnRate);
	double CurrentRate = 0.0;
		
	for (FBoxSpawnInfo Info : Boxes)
	{
		if (AllBoxesOfType.Contains(Info.BoxType) && Info.RemainingBoxes > 0)
		{
			// Checks which address should be added
			if (Percentage >= CurrentRate && Percentage <= Info.CurrentSpawnRate + CurrentRate)
			{
				AddProperty(Properties, Info.BoxType);
				break;
			}
			
			CurrentRate += Info.CurrentSpawnRate;
		}
	}
}

void USpawnAI::ConvertAllPercentageToBoxes()
{
	double TotalDangerousWeight = AddTotalWeightForBoxType(DangerousTypes);
	double TotalAddressWeight = AddTotalWeightForBoxType(AddressTypes);
	
	// Set up all boxes of every enum
	for (EBoxType BoxType : AllBoxTypes)
	{
		if (BoxType == EBoxType::Large) continue;
		
		FBoxSpawnInfo BoxSpawnInfo = FBoxSpawnInfo(BoxType);
		int RemainingBoxes;
		
		// brutalt lång metod.. i dont like
		
		//CheckAndHandlePercentageForWeightedBoxes(BoxType, DangerousTypes, TotalDangerousWeight, RemainingBoxes, GetSpawnInfo(EBoxType::Dangerous).RemainingBoxes);
		//CheckAndHandlePercentageForWeightedBoxes(BoxType, AddressTypes, TotalAddressWeight, RemainingBoxes, TotalBoxCount);
		
		if (DangerousTypes.Contains(BoxType))
		{
			double Percentage = ConvertWeightToPercentage(DangerousTypes[BoxType], TotalDangerousWeight);
			ConvertPercentageToBox(Percentage, RemainingBoxes, GetSpawnInfo(EBoxType::Dangerous).RemainingBoxes);
		}
		else if (AddressTypes.Contains(BoxType))
		{
			double Percentage = ConvertWeightToPercentage(AddressTypes[BoxType], TotalAddressWeight);
			ConvertPercentageToBox(Percentage, RemainingBoxes, TotalBoxCount);
		}
		else
		{
			BoxType == EBoxType::Dangerous ? ConvertPercentageToBox(SpawnRates[BoxType], RemainingBoxes, GetSpawnInfo(EBoxType::Suspicious).RemainingBoxes) : ConvertPercentageToBox(SpawnRates[BoxType], RemainingBoxes, TotalBoxCount);
		}
		
		BoxSpawnInfo.RemainingBoxes = RemainingBoxes;
		Boxes.Add(BoxSpawnInfo);
		
		if (BoxType == EBoxType::Radioactive)
		{
			UE_LOG(LogTemp, Warning, TEXT("Radioactive remaining boxes: %d"), BoxSpawnInfo.RemainingBoxes);
		}
		
		if (BoxType == EBoxType::Small)
		{
			FBoxSpawnInfo LargeSpawnInfo = FBoxSpawnInfo(EBoxType::Large);
			LargeSpawnInfo.RemainingBoxes = TotalBoxCount-RemainingBoxes;
			//SetUpSpawnRate(SpawnRate, LargeSpawnInfo.RemainingBoxes, TotalBoxCount);
			//LargeSpawnInfo.CurrentSpawnRate = SpawnRate;
			Boxes.Add(LargeSpawnInfo);
		}
	}
	
	EnsurePercentageIsValid();
}

double USpawnAI::AddTotalWeightForBoxType(TMap<EBoxType, double> AllBoxesOfType)
{
	double TotalWeight = 0.0;
	
	for (const TPair<EBoxType, double>& Pair : AllBoxesOfType)
	{
		TotalWeight += Pair.Value;
	}
	
	return TotalWeight;
}

void USpawnAI::CheckAndHandlePercentageForWeightedBoxes(EBoxType BoxType, TMap<EBoxType, double> AllBoxesOfType, double TotalWeight, int& RemainingBoxes, int DependencyFromAmount)
{
	if (AllBoxesOfType.Contains(BoxType))
	{
		double Percentage = ConvertWeightToPercentage(AllBoxesOfType[BoxType], TotalWeight);
		ConvertPercentageToBox(Percentage, RemainingBoxes, DependencyFromAmount);
	}
}

void USpawnAI::ConvertPercentageToBox(double Percentage, int& TypeOfRemainingBoxes, int DependencyFromAmount)
{
	Percentage = DependencyFromAmount * (Percentage / 100.f);
	Percentage = FMath::RoundHalfFromZero(Percentage);
	TypeOfRemainingBoxes = Percentage;
}

double USpawnAI::ConvertWeightToPercentage(double Weight, double TotalWeight)
{
	double Percentage = Weight / TotalWeight * 100.0;
	//Percentage = FMath::RoundHalfFromZero(Percentage);
	return Percentage;
}

void USpawnAI::EnsurePercentageIsValid()
{
	EnsureValidityForBoxType(DangerousTypes, GetSpawnInfo(EBoxType::Suspicious).RemainingBoxes);
	EnsureValidityForBoxType(AddressTypes, TotalBoxCount);
}

void USpawnAI::EnsureValidityForBoxType(TMap<EBoxType, double> AllBoxesOfType, int DependencyFromAmount)
{
	int TotalAmount = 0;
	TArray<FBoxSpawnInfo> RemainingBoxes;
	
	for (FBoxSpawnInfo Info : Boxes)
	{
		if (AllBoxesOfType.Contains(Info.BoxType))
		{
			TotalAmount+= Info.RemainingBoxes;
			RemainingBoxes.Add(Info);
		}
	}
	
	while (TotalAmount > DependencyFromAmount)
	{
		// does this need reference hm
		FBoxSpawnInfo BoxToRemove = RemainingBoxes[FMath::RandRange(0, RemainingBoxes.Num()-1)];
		GetSpawnInfo(BoxToRemove.BoxType).DecrementBoxCount();
		//RemainingBoxes[FMath::RandRange(0, RemainingBoxes.Num()-1)].DecrementBoxCount();
		
		TotalAmount--;
	}
}

void USpawnAI::SetUpSpawnRate(double& SpawnRate, int RemainingBoxType, int DependencyFromAmount)
{
	if (RemainingBoxType > 0)
	{
		SpawnRate = static_cast<double>(RemainingBoxType) / static_cast<double>(DependencyFromAmount);
	}
}

bool USpawnAI::RollForProperty(EBoxType BoxType)
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

double USpawnAI::GiveRandomPercentage()
{
	return FMath::RandRange(0.0, 1.0);
}

double USpawnAI::GiveWeightedBoxesMaxPercentage(double MaxPercentage)
{
	return FMath::RandRange(0.0, MaxPercentage);
}

bool USpawnAI::GuaranteeProperty(TArray<EBoxType>& Properties, EBoxType BoxType, int DependencyFromAmount)
{
	if (GetSpawnInfo(BoxType).RemainingBoxes == DependencyFromAmount && GetSpawnInfo(BoxType).RemainingBoxes > 0)
	{
		AddProperty(Properties, BoxType);
		return true;
	}
	
	/*int CurrentCounter = 1/GetSpawnInfo(BoxType).CurrentSpawnRate;
	
	if (CurrentCounter < GetSpawnInfo(BoxType).CountSinceLastSpawn && GetSpawnInfo(BoxType).RemainingBoxes > 0)
	{
		AddProperty(Properties, BoxType);
		return true;
	}*/
	
	return false;
}

FBoxSpawnInfo& USpawnAI::GetSpawnInfo(EBoxType Type)
{
	for (FBoxSpawnInfo& Box : Boxes)
	{
		if (Box.BoxType == Type) return Box;
	}
	
	return Boxes[0];
}

void USpawnAI::AddProperty(TArray<EBoxType>& Properties, EBoxType Type)
{
	Properties.Add(Type);
	GetSpawnInfo(Type).DecrementBoxCount();
}
