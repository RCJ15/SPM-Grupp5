// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ScoreManager.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScoreChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComboChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComboBreak);

UCLASS()
class SPMG5_API UScoreManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseComboMultiplier = true;
	
	UPROPERTY(BlueprintAssignable)
	FOnScoreChanged OnScoreChanged;
	UPROPERTY(BlueprintAssignable)
	FOnComboChanged OnComboChanged;
	UPROPERTY(BlueprintAssignable)
	FOnComboBreak OnComboBreak;
	
	UFUNCTION(BlueprintCallable)
	int GetScore();
	
	UFUNCTION(BlueprintCallable)
	int GetAddedScore();
	
	UFUNCTION(BlueprintCallable)
	int GetComboMultiplier();
	UFUNCTION(BlueprintCallable)
	float GetComboTimeRate();
	
	UFUNCTION(BlueprintCallable)
	void StartTimer();
	
	
	UFUNCTION(BlueprintCallable)
	int GetCorrectlySortedBoxes();
	
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

	UScoreManager* GetScoreManager() { return this; }
	
private:
	
	int Score;
	
	int AddedScore;
	
	int ComboMultiplier = 1;
	float ComboTimeRemaining = 0;
	int TimeRate = 1;
	float ComboTimerMAX = 5;
	FTimerHandle ComboTimer;
	void CountdownCombo();
	void ChangeCombo(int Change);
	
	
	int CorrectlySortedBoxes = 0;
};	
