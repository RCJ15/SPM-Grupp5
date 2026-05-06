// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ShiftManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimeRunsOut);
/**
 * 
 */
UCLASS()
class SPMG5_API UShiftManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
private:
	FTimerHandle ShiftTimer;

	int CurrentMin = TimeRemaining / 60;
	int CurrentSec = TimeRemaining % 60;

protected:
	// Called when the game starts or when spawned

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnTimeRunsOut OnTimeRunsOut;
	
	int ShiftLengthInSeconds = 45;

	int TimeRemaining = ShiftLengthInSeconds;

	int TimeRate = 1;

	UFUNCTION(BlueprintCallable)
	int GetCurrentMin(){return CurrentMin;}
	
	UFUNCTION(BlueprintCallable)
	int GetCurrentSec(){return CurrentSec;}
	
	UFUNCTION(BlueprintCallable)
	void StartTimer();

	void CountdownShift();
	
	//UFUNCTION(BlueprintCallable, Blueprintable)
	//void TimeRunsOut();
};
