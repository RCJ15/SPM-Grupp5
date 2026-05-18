// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnAI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnValueChanged);

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
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities", meta=(UIMin = "0.0", UIMax = "100.0"))
	double SmallBoxesPercentage = 75.5f;
	
	double LargeBoxesPercentage = 24.5f;
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities", meta=(UIMin = "0.0", UIMax = "100.0"))
	double FragileBoxesPercentage = 15.f;
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities", meta=(UIMin = "0.0", UIMax = "100.0"))
	double SuspiciousBoxesPercentage = 40.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities", meta=(UIMin = "0.0", UIMax = "100.0"))
	double DangerousBoxesPercentage = 50.f;
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities|Dangerous Boxes") //, meta=(UIMin = "0.0", UIMax = "100.0"))
	double BombBoxesWeight = 1.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities|Dangerous Boxes") //, meta=(UIMin = "0.0", UIMax = "100.0"))
	double ToxicWasteBoxesWeight = 2.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities|Dangerous Boxes") //, meta=(UIMin = "0.0", UIMax = "100.0"))
	double FlashBangBoxesWeight = 7.f;
	
	int RemainingSmallBoxes;
	int RemainingLargeBoxes;
	int RemainingFragileBoxes;
	int RemainingSuspiciousBoxes;
	int RemainingDangerousBoxes;
	int RemainingBombBoxes;
	int RemainingToxicWasteBoxes;
	int RemainingFlashBangBoxes;
	
	int SmallBoxesSpawnRate;
	int LargeBoxesSpawnRate;
	int FragileBoxesSpawnRate;
	int SuspiciousBoxesSpawnRate;
	int DangerousBoxesSpawnRate;
	int BombBoxesSpawnRate;
	int ToxicWasteBoxesSpawnRate;
	int FlashBangBoxesSpawnRate;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void ConvertAllPercentageToBoxes();
	void ConvertPercentageToBox(double& Percentage, int& TypeOfRemainingBoxes, int DependencyFromAmount);
	void ConvertWeightToBox(double& Weight, int& TypeOfRemainingBoxes, int DependencyFromAmount);
	void SetUpSpawnRate(int& SpawnRate, int BoxType);
	int RoundToClosestInt(float Value);
};
