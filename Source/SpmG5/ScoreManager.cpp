// Marcus hopefully approves of this.


#include "ScoreManager.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "GameManager.h"

int UScoreManager::GetScore()
{
	return Score;
}

int UScoreManager::GetAddedScore()
{
	return AddedScore;
}

int UScoreManager::GetComboMultiplier()
{
	return ComboMultiplier;
}

void UScoreManager::AddScore(int ScoreChange)
{
	AddedScore = ScoreChange;
	
	int NewScore = Score + ScoreChange;
 	
	if (NewScore < 0)
	{
		NewScore = 0;
		OnScoreChanged.Broadcast();
	}
	
	if (NewScore != Score)
	{
		Score = NewScore;
		OnScoreChanged.Broadcast();
		
		//Höj combo multiplier vid rätt sortering, och ta bort vid fel sortering
		if (AddedScore < 0)
		{
			ComboMultiplier = 1;
		}
		else
		{
			++ComboMultiplier;
		}
		OnComboChanged.Broadcast();
	}
	
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


