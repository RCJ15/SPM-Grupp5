// Marcus hopefully approves of this.


#include "GameManager.h"
#include "Kismet/GameplayStatics.h"


void UGameManager::LoadLevel(TSoftObjectPtr<UWorld> Level)
{
	// Checks if there is a previous level to despawn
	if (!FirstTime)
	{
		PreviousLevel = CurrentLevel;
	}
	
	CurrentLevel = Level;
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("LevelLoaded");
	LatentInfo.Linkage = 0;
	
	// Loads additive level
	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(),Level, true, false, LatentInfo);
}

void UGameManager::RestartLevel()
{
	if (FirstTime) return;
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("UGameplayStatics::LoadStreamLevelBySoftObjectPtr(");
	LatentInfo.Linkage = 0;
	
	UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(GetWorld(), CurrentLevel, LatentInfo, false);

	
}


bool UGameManager::GetLevelStarted()
{
	return LevelStarted;
}

TSoftObjectPtr<UWorld> UGameManager::GetCurrentLevel()
{
	return CurrentLevel;
}

TSoftObjectPtr<UWorld> UGameManager::GetPreviousLevel()
{
	return PreviousLevel;
}

void UGameManager::LevelLoaded()
{
	if (FirstTime)
	{
		FirstTime = false;
	}
	else
	{
		FLatentActionInfo LatentInfo;
		
		// Deloads previous level
		if (PreviousLevel != CurrentLevel)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *PreviousLevel.ToString());
			UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(GetWorld(), PreviousLevel, LatentInfo, false);
		}
	}
	
	// Let's relevant classes know when a level has finished being loaded in
	OnLevelLoadedInternal.Broadcast();
}


