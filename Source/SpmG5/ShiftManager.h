// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ShiftManager.generated.h"

/**
 * 
 */
UCLASS()
class SPMG5_API UShiftManager : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	FTimerHandle ShiftTimer;

	int CurrentMin;
	int CurrentSec;
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ShiftLengthInSeconds = 60;

	int TimeRemaining = ShiftLengthInSeconds;

	int TimeRate = 1;

	int GetCurrentMin(){return CurrentMin;}
	int GetCurrentSec(){return CurrentSec;}

	void Timer(float Time);

	void CountdownShift();
};
