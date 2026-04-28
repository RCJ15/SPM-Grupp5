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

	int CurrentMin = 0;
	int CurrentSec = 0;

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
public:
	int ShiftLengthInSeconds = 300;

	int TimeRemaining = ShiftLengthInSeconds;

	int TimeRate = 1;

	UFUNCTION(BlueprintCallable)
	int GetCurrentMin(){return CurrentMin;}
	
	UFUNCTION(BlueprintCallable)
	int GetCurrentSec(){return CurrentSec;}
	UFUNCTION()
	void Timer();

	void CountdownShift();
};
