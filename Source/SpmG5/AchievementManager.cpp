// Marcus hopefully approves of this.

//this class will unlock achievements
#include "AchievementManager.h"

AchievementManager::AchievementManager()
{
	LoadAchievements();
	//kolla igenom array, om !bIsUnlocked
	//subscribea olika achievement metoder på händelser
}

AchievementManager::~AchievementManager()
{
	//innan destruct spara??
	SaveAchievements();
}

//metod för att ladda in achievements från en fil, returnar success eller inte
bool AchievementManager::LoadAchievements()
{
	//från filen börja med att läsa antal achievements
	//skapa Achievements arrayen utifrån detta antalet
	//och läs sedan varje achievement del från filen
	//och skapa dessa till FAchievement
	//som man lägger in i Achievements arrayen
	return false;
}

//metod för att spara ner achievements till en fil, returnar success eller inte
bool AchievementManager::SaveAchievements()
{
	//i fil skriv ned
	//antal achievements
	//och sen lista varje achievement med dess delar
	
	return false;
}

void AchievementManager::IncreaseAchievementCount(int const AchievementIndex)
{
	FAchievement A = Achievements[AchievementIndex];
	A.CurrentEventCount++;
	if (A.CurrentEventCount >= A.ToReachEventCount)
	{
		UnlockAchievement(AchievementIndex);
	}
}

void AchievementManager::UnlockAchievement(int const AchievementIndex)
{
	Achievements[AchievementIndex].bIsUnlocked = true;
	//unsubscribe dens korresponderande metod från stuff
	//så behöver skicka med metod typ...
}

void AchievementManager::CheckFulfillAchievement0()
{
	//kolla om krav uppfylls
	
	
	//om ja
	IncreaseAchievementCount(0);
}

void AchievementManager::CheckFulfillAchievement1()
{
}

void AchievementManager::CheckFulfillAchievement2()
{
}

void AchievementManager::CheckFulfillAchievement3()
{
}


