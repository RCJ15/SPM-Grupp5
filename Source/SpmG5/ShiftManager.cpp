// Marcus hopefully approves of this.


#include "ShiftManager.h"

void UShiftManager::Timer(float Time)
{
	GetWorld()->GetTimerManager().SetTimer(
		ShiftTimer, this, &UShiftManager::CountdownShift, TimeRate, true, -9);
}

void UShiftManager::CountdownShift()
{
	TimeRemaining -= TimeRate;

	//Update timer on screen
	
	if (TimeRemaining <= 0)
	{
		GetWorld()->GetTimerManager().PauseTimer(ShiftTimer);
	}

	CurrentMin = TimeRemaining / 60;
	CurrentSec = TimeRemaining % 60;
	//FString s =  "" + (TimeRemaining % 60)
}


