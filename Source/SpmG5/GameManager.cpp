// Marcus hopefully approves of this.


#include "GameManager.h"

#include "Kismet/GameplayStatics.h"


void UGameManager::LoadLevel(TSoftObjectPtr<UWorld> Level)
{
	if (!FirstTime)
	{
		PreviousLevel = CurrentLevel;
	}
	CurrentLevel = Level;
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("LevelLoaded");
	LatentInfo.Linkage = 0;
	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(),Level, true, false, LatentInfo);
}


void UGameManager::LevelLoaded()
{
	if (FirstTime)
	{
		FirstTime = false;
	}else
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(GetWorld(), PreviousLevel, LatentInfo, false);
	}
	OnLevelLoadedInternal.Broadcast();
}


