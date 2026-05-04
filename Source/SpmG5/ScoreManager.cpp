// Marcus hopefully approves of this.


#include "ScoreManager.h"

 int UScoreManager::GetScore()
 {
	return Score;
}

void UScoreManager::AddScore(int AddedScore)
{
 	Score += AddedScore;
 	
 	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);
}

void UScoreManager::SetScore(int NewScore)
{
 	
}

void UScoreManager::ResetScore()
{
 	
}
