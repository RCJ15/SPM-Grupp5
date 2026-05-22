// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "BoxSpawnRateManager.generated.h"

/**
 * 
 */
UCLASS()
class SPMG5_API UBoxSpawnRateManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	double SmallBoxesSpawnRate;
	double LargeBoxesSpawnRate;
	double FragileBoxesSpawnRate;
	double SuspiciousBoxesSpawnRate;
	double DangerousBoxesSpawnRate;
	double BombBoxesSpawnRate;
	double ToxicWasteBoxesSpawnRate;
	double FlashBangBoxesSpawnRate;
	
public:
	void UpdateSpawnRate(double SpawnRate);
};
