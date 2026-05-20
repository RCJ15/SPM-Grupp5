// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "GameManager.generated.h"


class UWidget;
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
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PauseGame();
	
	UPROPERTY()
	FOnLevelLoadedInternal OnLevelLoadedInternal;
	
	bool GetLevelStarted();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSoftObjectPtr<UWorld> GetCurrentLevel();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSoftObjectPtr<UWorld> GetPreviousLevel();
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UUserWidget* HUDWidget;
	
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* StartCountdownWidget;
	
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* MenuWidget;
	
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* InstructionWidget;
	
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* ControllerWidget;
	
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* WinOrLoseWidget;
	
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* PauseWidget;
	
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* SettingsWidget;
	
protected:
	UPROPERTY(BlueprintReadWrite)
	bool LevelStarted;

private:
	UFUNCTION()
	void LevelLoaded();

	bool FirstTime = true;
};
