// Marcus hopefully approves of this.


#include "LoadLevelAsync.h"
#include "GameManager.h"

ULoadLevelAsync* ULoadLevelAsync::LoadLevelAsync(UObject* WorldContextObject, TSoftObjectPtr<UWorld> Level)
{
	// Loads in level async so game won't stop when loading
	ULoadLevelAsync* AsyncTask = NewObject<ULoadLevelAsync>();
	AsyncTask->WorldContextObject = WorldContextObject;
	AsyncTask->Level = Level;

	return AsyncTask;
}

void ULoadLevelAsync::Activate()
{
	Super::Activate();
	
	if (WorldContextObject)
	{
		// Finds game manager in code so it doesn't have to be done in BP
		UGameManager* GameManager = Cast<UGameManager>(WorldContextObject->GetWorld()->GetGameInstance());
		
		// Find out through game manager when a new level is to be loaded
		GameManager->OnLevelLoadedInternal.AddDynamic(this, &ULoadLevelAsync::Finish);
		GameManager->LoadLevel(Level);
	}
}

void ULoadLevelAsync::Finish()
{
	UGameManager* GameManager = Cast<UGameManager>(WorldContextObject->GetWorld()->GetGameInstance());
	
	// Removes previous delegatesw if they still linger
	GameManager->OnLevelLoadedInternal.RemoveAll(this);
	OnLevelLoaded.Broadcast();
	SetReadyToDestroy();
}
