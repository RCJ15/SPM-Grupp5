// Marcus hopefully approves of this.


#include "LoadLevelAsync.h"

#include "GameManager.h"

ULoadLevelAsync* ULoadLevelAsync::LoadLevelAsync(UObject* WorldContextObject, TSoftObjectPtr<UWorld> Level)
{
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
		UGameManager* GameManager = Cast<UGameManager>(WorldContextObject->GetWorld()->GetGameInstance());
		GameManager->OnLevelLoadedInternal.AddDynamic(this, &ULoadLevelAsync::Finish);
		GameManager->LoadLevel(Level);
	}
}

void ULoadLevelAsync::Finish()
{
	UGameManager* GameManager = Cast<UGameManager>(WorldContextObject->GetWorld()->GetGameInstance());
	GameManager->OnLevelLoadedInternal.RemoveAll(this);
	OnLevelLoaded.Broadcast();
	SetReadyToDestroy();
}
