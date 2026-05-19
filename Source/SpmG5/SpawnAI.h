// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnAI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnValueChanged);

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
	FlashBang
};

USTRUCT(BlueprintType)
struct FBoxSpawnInfo
{
	GENERATED_BODY()
	
	FBoxSpawnInfo(){};
	
	FBoxSpawnInfo(EBoxType Type){BoxType = Type;}
	
	EBoxType BoxType;
	int RemainingBoxes;
	double CurrentSpawnRate;
	int CountSinceLastSpawn = 0;
	
	void DecrementBoxCount()
	{
		RemainingBoxes--;
		CountSinceLastSpawn = 0;
	};
	
};

UCLASS()
class SPMG5_API ASpawnAI : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ASpawnAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	FOnValueChanged OnValueChanged;
	
	UPROPERTY(EditAnywhere, Category = "Box Properities")
	int AmountOfBoxesPerLevel = 100.f;
	/*
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities", meta=(UIMin = "0.0", UIMax = "100.0"))
	double SmallBoxesPercentage = 74.5;
	
	double LargeBoxesPercentage = 25;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities", meta=(UIMin = "0.0", UIMax = "100.0"))
	double FragileBoxesPercentage = 15.0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities", meta=(UIMin = "0.0", UIMax = "100.0"))
	double SuspiciousBoxesPercentage = 40.0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities", meta=(UIMin = "0.0", UIMax = "100.0"))
	double DangerousBoxesPercentage = 50.0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities|Dangerous Boxes") //, meta=(UIMin = "0.0", UIMax = "100.0"))
	double BombBoxesWeight = 1;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities|Dangerous Boxes") //, meta=(UIMin = "0.0", UIMax = "100.0"))
	double ToxicWasteBoxesWeight = 0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities|Dangerous Boxes") //, meta=(UIMin = "0.0", UIMax = "100.0"))
	double FlashBangBoxesWeight = 0;
	*/
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, EditFixedSize, Category = "Box Properities", meta=(UIMin = "0.0", UIMax = "100.0"), meta=(ReadOnlyKeys))
	TMap<EBoxType, double> SpawnRates = {{ EBoxType::Small, 75.0 }, { EBoxType::Fragile, 15.0 },{ EBoxType::Suspicious, 40.0 },{ EBoxType::Dangerous, 50.0 }};
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, EditFixedSize, Category = "Box Properities", meta=(ReadOnlyKeys))
	TMap<EBoxType, double> DangerousTypes = {{ EBoxType::Bomb, 1.0 }, { EBoxType::ToxicWaste, 0.0 }, { EBoxType::FlashBang, 0.0 } };
	
	TArray<FBoxSpawnInfo> Boxes;
	
	/*int RemainingSmallBoxes;
	int RemainingLargeBoxes;
	int RemainingFragileBoxes;
	int RemainingSuspiciousBoxes;
	int RemainingDangerousBoxes;
	int RemainingBombBoxes;
	int RemainingToxicWasteBoxes;
	int RemainingFlashBangBoxes;
	
	double SmallBoxesSpawnRate;
	double LargeBoxesSpawnRate;
	double FragileBoxesSpawnRate;
	double SuspiciousBoxesSpawnRate;
	double DangerousBoxesSpawnRate;
	double BombBoxesSpawnRate;
	double ToxicWasteBoxesSpawnRate;
	double FlashBangBoxesSpawnRate;
	
	int SmallBoxes;
	int LargeBoxes;
	int FragileBoxes;
	int SuspiciousBoxes;
	int DangerousBoxes;
	int BombBoxes;
	int ToxicWasteBoxes;
	int FlashBangBoxes;*/

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void ConvertAllPercentageToBoxes();
	void ConvertPercentageToBox(double Percentage, int& TypeOfRemainingBoxes, int DependencyFromAmount);
	double ConvertWeightToPercentage(double Weight, double TotalWeight);
	void SetUpSpawnRate(double& SpawnRate, int BoxType, int DependencyFromAmount);
	TArray<EBoxType> DecideProperties();
	bool RollForProperty(EBoxType BoxType);
	bool SetProperties();
	double GivePercentage();
	
	
private:
	TArray<EBoxType> AllBoxTypes{EBoxType::Small, EBoxType::Fragile, EBoxType::Suspicious, EBoxType::Dangerous, EBoxType::Bomb, EBoxType::ToxicWaste, EBoxType::FlashBang};
	
	FBoxSpawnInfo& GetSpawnInfo(EBoxType Type);
	
	void AddProperty(TArray<EBoxType>& Properties, EBoxType Type);
};
