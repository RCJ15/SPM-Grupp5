// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "GameManager.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelLoadedInternal);
/**
 * 
 */
UCLASS()
class SPMG5_API UGameManager : public UGameInstance
{
	GENERATED_BODY()
	
public:
	void LoadLevel(TSoftObjectPtr<UWorld> Level);

	FName PendingLevelName;
	
	TSoftObjectPtr<UWorld> PreviousLevel;

	TSoftObjectPtr<UWorld> CurrentLevel;
	
	UPROPERTY()
	FOnLevelLoadedInternal OnLevelLoadedInternal;
	
	bool GetLevelStarted();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSoftObjectPtr<UWorld> GetCurrentLevel();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSoftObjectPtr<UWorld> GetPreviousLevel();
	
protected:
	UPROPERTY(BlueprintReadWrite)
	bool LevelStarted;

private:
	UFUNCTION()
	void LevelLoaded();

	bool FirstTime = true;
};
