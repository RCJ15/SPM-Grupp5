// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnAI.generated.h"

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
	
	UPROPERTY(EditAnywhere)
	int AmountOfBoxesPerLevel = 150;
	
	UPROPERTY(EditAnywhere)
	int FragileBoxesPercentage = 15;
	
	UPROPERTY(EditAnywhere)
	int LargeBoxesPercentage = 10;
	
	UPROPERTY(EditAnywhere)
	int BombBoxesPercentage = 5;
	
	UPROPERTY(EditAnywhere)
	int ToxicWasteBoxesPercentage = 5;
	
	UPROPERTY(EditAnywhere)
	int FlashBangBoxesPercentage = 5;
	
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
