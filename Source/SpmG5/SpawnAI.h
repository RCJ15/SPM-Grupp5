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
	int AmountOfBoxesPerLevel = 100;
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities", meta=(UIMin = "0.0", UIMax = "100.0"))
	float SmallBoxesPercentage = 75;
	
	float LargeBoxesPercentage = 25;
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities", meta=(UIMin = "0.0", UIMax = "100.0"))
	float FragileBoxesPercentage = 15;
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities", meta=(UIMin = "0.0", UIMax = "100.0"))
	float SuspiciousBoxesPercentage = 40;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities", meta=(UIMin = "0.0", UIMax = "100.0"))
	float DangerousBoxesPercentage = 50;
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities") //, meta=(UIMin = "0.0", UIMax = "100.0"))
	float BombBoxesWeight = 1.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities") //, meta=(UIMin = "0.0", UIMax = "100.0"))
	float ToxicWasteBoxesWeight = 2.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities") //, meta=(UIMin = "0.0", UIMax = "100.0"))
	float FlashBangBoxesWeight = 3.f;
	
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
	void ConvertPercentageToBox(float& Percentage, int& TypeOfRemainingBoxes, int DependencyFromAmount);
	void ConvertWeightToBox(float& Weight, int& TypeOfRemainingBoxes, int DependencyFromAmount);
	void SetUpSpawnRate(int& SpawnRate, int BoxType);
};
