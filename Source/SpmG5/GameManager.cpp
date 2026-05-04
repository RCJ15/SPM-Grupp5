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
		UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(GetWorld(), PreviousLevel, LatentInfo, false);
	}
	
	// Let's relevant classes know when a level has finished being loaded in
	OnLevelLoadedInternal.Broadcast();
}


