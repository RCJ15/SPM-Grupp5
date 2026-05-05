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
	int AmountOfBoxesPerLevel = 150;
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities", meta=(UIMin = "0.0", UIMax = "100.0"))
	int SmallBoxesPercentage = 75;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities", meta=(UIMin = "0.0", UIMax = "100.0"))
	int LargeBoxesPercentage = 25;
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities", meta=(UIMin = "0.0", UIMax = "100.0"))
	int FragileBoxesPercentage = 15;
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities", meta=(UIMin = "0.0", UIMax = "100.0"))
	int SuspiciousBoxesPercentage = 30;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities", meta=(UIMin = "0.0", UIMax = "100.0"))
	int DangerousBoxesPercentage = 50;
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities", meta=(UIMin = "0.0", UIMax = "100.0"))
	int BombBoxesPercentage = 34;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities", meta=(UIMin = "0.0", UIMax = "100.0"))
	int ToxicWasteBoxesPercentage = 33;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Box Properities", meta=(UIMin = "0.0", UIMax = "100.0"))
	int FlashBangBoxesPercentage = 33;
	
	int RemainingFragileBoxes;
	int RemainingLargeBoxes;
	int RemainingBombBoxes;
	int RemainingToxicWasteBoxes;
	int RemainingFlashBangBoxes;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void ConvertAllPercentageToBoxes();
	void ConvertPercentageToBox(int& Percentage, int& TypeOfRemainingBoxes);
};
