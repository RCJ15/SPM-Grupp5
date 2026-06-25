// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ConveyorBeltUpgraded.h"
#include "DifficultyManager.generated.h"

/**
 * 
 */
UCLASS()
class SPMG5_API UDifficultyManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
	
public:
	void UpdateDifficulty();
	
	UPROPERTY()
	TArray<AConveyorBeltUpgraded*> ConveyorBelts;
private:
	void CheckPerformance();
	void SetBeltSpeed(float BeltSpeed);
	void SetSpawnerSpeed(float SpawnerSpeed);
	void SetDangerousRate(float DangerousRate);
	void BreakBelt();
	void SetBlowRate();
	void SetPlayerSpeed(float PlayerSpeed);
	void SetMultiplier(float Multiplier);
	
};
