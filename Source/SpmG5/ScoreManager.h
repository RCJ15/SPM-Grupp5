// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ScoreManager.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScoreChanged);

UCLASS()
class SPMG5_API UScoreManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnScoreChanged OnScoreChanged;
	
	UFUNCTION(BlueprintCallable)
	void ConnectHUDWidget(UUserWidget* Widget) { HUDWidget = Widget; }
	
	UFUNCTION(BlueprintCallable)
	int GetScore();
	
	UFUNCTION(BlueprintCallable)
	int GetAddedScore();
	
	UFUNCTION(BlueprintCallable)
	void AddScore(int ScoreChange);
	
	UFUNCTION(BlueprintCallable)
	void ResetScore();
	
	UFUNCTION(BlueprintCallable)
	void SaveScore();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ScoreToPassTutorial = 70;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ScoreToPassLevels = 250;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<TSoftObjectPtr<UWorld>, int> ScoreMap;

private:
	UPROPERTY()
	UUserWidget* HUDWidget;
	
	int Score;
	
	int AddedScore;
};	
