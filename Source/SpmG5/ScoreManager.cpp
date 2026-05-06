// Marcus hopefully approves of this.


#include "ScoreManager.h"

#include "GameManager.h"

int UScoreManager::GetScore()
 {
	return Score;
}

void UScoreManager::AddScore(int AddedScore)
{
 	Score += AddedScore;
 	
 	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);
}


void UScoreManager::ResetScore()
{
	Score = 0.f;
}

void UScoreManager::SaveScore()
{
	TSoftObjectPtr<UWorld> CurrentLevel = Cast<UGameManager>(GetWorld()->GetGameInstance())->CurrentLevel;
	
	if (Score > ScoreMap.FindOrAdd(CurrentLevel))
	{
		ScoreMap[CurrentLevel] = Score;
	}
	ResetScore();
}


