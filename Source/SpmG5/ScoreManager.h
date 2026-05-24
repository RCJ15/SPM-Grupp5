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

UCLASS()
class SPMG5_API UScoreManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnScoreChanged OnScoreChanged;
	UPROPERTY(BlueprintAssignable)
	FOnComboChanged OnComboChanged;
	
	UFUNCTION(BlueprintCallable)
	int GetScore();
	
	UFUNCTION(BlueprintCallable)
	int GetAddedScore();
	
	UFUNCTION(BlueprintCallable)
	int GetComboMultiplier();
	
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
};	
