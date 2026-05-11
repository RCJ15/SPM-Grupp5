// Marcus hopefully approves of this.


#include "ShiftManager.h"
#include "GameManager.h"

void UShiftManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	UGameManager* GameManager = Cast<UGameManager>(GetWorld()->GetGameInstance());

	/*if (GameManager)
	{
		if (GameManager->GetLevelStarted())
		{
			GameManager->OnLevelLoadedInternal.AddDynamic(this, &UShiftManager::StartTimer);
		}
	}*/
	//Timer();
	//OnTimeRunsOut.AddDynamic(this, &UShiftManager::TimeRunsOut);
}

void UShiftManager::SetShiftTime(int seconds)
{
	if (seconds <= 0)
	{
		seconds = BaseShiftLengthInSeconds;
	}
	ShiftLengthInSeconds = seconds;
}

void UShiftManager::StartTimer()
{
	OnTimeRunsOut.RemoveAll(this);
	GetWorld()->GetTimerManager().ClearTimer(ShiftTimer);
	TimeRemaining = ShiftLengthInSeconds;
	CurrentMin = ShiftLengthInSeconds / 60;
	CurrentSec = ShiftLengthInSeconds % 60;
	
	GetWorld()->GetTimerManager().SetTimer(
		ShiftTimer, this, &UShiftManager::CountdownShift, TimeRate, true);
}

void UShiftManager::CountdownShift()
{
	TimeRemaining -= TimeRate;

	// Update timer on screen
	
	if (TimeRemaining <= 0)
	{
		//GetWorld()->GetTimerManager().PauseTimer(ShiftTimer);
		GetWorld()->GetTimerManager().ClearTimer(ShiftTimer);
		OnTimeRunsOut.Broadcast();
	}

	CurrentMin = TimeRemaining / 60;
	CurrentSec = TimeRemaining % 60;
}

/*void UShiftManager::TimeRunsOut()
{
	
}*/
