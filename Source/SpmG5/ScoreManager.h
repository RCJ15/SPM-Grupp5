// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ScoreManager.generated.h"

/**
 * 
 */
UCLASS()
class SPMG5_API UScoreManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	int GetScore();
	
	UFUNCTION(BlueprintCallable)
	void AddScore(int AddedScore);
	
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
	int Score;
};	
