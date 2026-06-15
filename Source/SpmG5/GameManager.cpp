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
	
	if(FirstTime || CurrentLevel != PreviousLevel)
	{
		LatentInfo.ExecutionFunction = FName("LevelLoaded");
		LatentInfo.Linkage = 0;
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(),Level, true, false, LatentInfo);
	}
	else
	{
		LatentInfo.ExecutionFunction = FName("LevelUnloaded");
		LatentInfo.Linkage = 0;
		UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(GetWorld(), Level, LatentInfo, false);
	}
	
	// Loads additive level
}

/*void UGameManager::RestartLevel()
{
	if (FirstTime) return;
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName();
	LatentInfo.Linkage = 0;
	
	UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(GetWorld(), PreviousLevel, LatentInfo, false);
	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(), CurrentLevel, true, false, LatentInfo);

	
}*/


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
	
	// Lets relevant classes know when a level has finished being loaded in
	OnLevelLoadedInternal.Broadcast();
}

void UGameManager::LevelUnloaded()
{
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("BroadcastLoaded");
	LatentInfo.Linkage = 0;
	
	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(),CurrentLevel, true, false, LatentInfo);
}

void UGameManager::BroadcastLoaded()
{
	UE_LOG(LogTemp, Warning, TEXT("clartabomb"));
	OnLevelLoadedInternal.Broadcast();
}

