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
	double SmallBoxesPercentage = 75.0;
	
	double LargeBoxesPercentage = 25.0;
	
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
	
	int RemainingSmallBoxes;
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void ConvertAllPercentageToBoxes();
	void ConvertPercentageToBox(double& Percentage, int& TypeOfRemainingBoxes, int DependencyFromAmount);
	void ConvertWeightToBox();
	void SetUpSpawnRate(double& SpawnRate, int BoxType, int DependencyFromAmount);
};
