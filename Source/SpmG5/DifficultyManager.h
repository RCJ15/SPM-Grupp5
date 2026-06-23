// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DifficultyManager.generated.h"

/**
 * 
 */
UCLASS()
class SPMG5_API UDifficultyManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
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
