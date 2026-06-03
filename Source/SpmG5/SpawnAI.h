// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SpawnAI.generated.h"

UENUM(BlueprintType)
enum class EBoxType : uint8
{
	Small,
	Large,
	Fragile,
	Suspicious,
	Dangerous,
	Bomb,
	ToxicWaste,
	FlashBang,
	Circle,
	Square,
	Triangle
};

USTRUCT(BlueprintType)
struct FBoxSpawnInfo
{
	GENERATED_BODY()
	
	FBoxSpawnInfo(){}
	
	FBoxSpawnInfo(EBoxType Type){BoxType = Type;}
	
	EBoxType BoxType;
	int RemainingBoxes;
	double CurrentSpawnRate;
	int CountSinceLastSpawn = 0;
	
	void DecrementBoxCount()
	{
		RemainingBoxes--;
		CountSinceLastSpawn = 0;
	}
	
	void IncreaseCounter()
	{
		CountSinceLastSpawn++;
	}
};

UCLASS()
class SPMG5_API USpawnAI : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetupSpawner(int InAmountOfBoxesPerLevel, TMap<EBoxType, double> InSpawnRates, TMap<EBoxType, double> InDangerousTypes, TMap<EBoxType, double> InAddressTypes);

	TArray<EBoxType> ConstructBox();

private:
	int TotalBoxCount;
	TMap<EBoxType, double> SpawnRates;
	TMap<EBoxType, double> DangerousTypes;
	TMap<EBoxType, double> AddressTypes;
	
	TArray<FBoxSpawnInfo> Boxes; //TMap<EBoxType, FBoxSpawnInfo> SpawnInfos; POTENTIELL UPGRADE ???
	TArray<EBoxType> AllBoxTypes{EBoxType::Small, EBoxType::Large, EBoxType::Fragile, EBoxType::Suspicious, EBoxType::Dangerous, EBoxType::Bomb, EBoxType::ToxicWaste, EBoxType::FlashBang, EBoxType::Circle, EBoxType::Square, EBoxType::Triangle};
	
	FBoxSpawnInfo& GetSpawnInfo(EBoxType Type);
	
	TArray<EBoxType> DecideProperties();
	
	void CheckAndAddPropertiesForWeights(TMap<EBoxType, double> AllBoxesOfType, TArray<EBoxType>& Properties);
	
	void ConvertAllPercentageToBoxes();
	double AddTotalWeightForBoxType(TMap<EBoxType, double> AllBoxesOfType);
	void CheckAndHandlePercentageForWeightedBoxes(EBoxType BoxType, TMap<EBoxType, double> AllBoxesOfType, double TotalWeight, int& RemainingBoxes, int DependencyFromAmount);
	
	void ConvertPercentageToBox(double Percentage, int& TypeOfRemainingBoxes, int DependencyFromAmount);
	double ConvertWeightToPercentage(double Weight, double TotalWeight);
	
	void EnsurePercentageIsValid();
	void EnsureValidityForBoxType(TMap<EBoxType, double> AllBoxesOfType, int DependencyFromAmount);
	void SetUpSpawnRate(double& SpawnRate, int RemainingBoxType, int DependencyFromAmount);
	
	bool RollForProperty(EBoxType BoxType);
	double GiveRandomPercentage();
	double GiveWeightedBoxesMaxPercentage(double MaxPercentage);
	
	bool GuaranteeProperty(TArray<EBoxType>& Properties, EBoxType BoxType, int DependencyFromAmount);
	void AddProperty(TArray<EBoxType>& Properties, EBoxType Type);
};
