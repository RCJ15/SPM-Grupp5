// Marcus hopefully approves of this.

//this class will unlock achievements
#include "AchievementManager.h"

AchievementManager::AchievementManager()
{
	
	//subscribea olika achievement metoder på händelser
	//TryUnlockAchievement1()
}

AchievementManager::~AchievementManager()
{
	//innan destruct spara??
}

void AchievementManager::CheckFulfillAchievement1()
{
	//kolla om krav uppfylls
	SortedBoxes++;
	
	if (SortedBoxes >= 10)
	{
		UnlockAchievement(A1);
	}
}

bool AchievementManager::UnlockAchievement(Achievement A)
{
	//kolla om redan unlocked, annars unlock
	//borde finnas ett bättre sätt att göra detta som unsubscibear detta achievements metod
	//if (UnlockedAchievements.Contains(A))
		//return false;
	
	UnlockedAchievements.Add(A);
	return true;
	
}


