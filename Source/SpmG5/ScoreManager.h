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
	
	
private:
	int Score;
	
public:
	UFUNCTION(BlueprintCallable)
	int GetScore();
	
	UFUNCTION(BlueprintCallable)
	void AddScore(int AddedScore);
	
	UFUNCTION(BlueprintCallable)
	void SetScore(int NewScore);
	
	UFUNCTION(BlueprintCallable)
	void ResetScore();
	
};	
