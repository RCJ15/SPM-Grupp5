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
	
	int GetScore();
	
	void AddScore(int AddedScore);
	
	void SetScore(int NewScore);
	
	void ResetScore();
	
};	
