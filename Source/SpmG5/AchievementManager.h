// Marcus hopefully approves of this.

#pragma once

#include <string>

#include "CoreMinimal.h"
//this class will unlock achievements
/**
 * 
 */
struct Achievement
{
	std::string name;
	//image icon
};

class SPMG5_API AchievementManager
{
public:
	AchievementManager();
	~AchievementManager();
private:
	bool UnlockAchievement(Achievement A);
	void CheckFulfillAchievement1();
	
	
	TArray<Achievement> UnlockedAchievements;
	
	//Achievements.. dessa bör väll sparas någon annan stans?
	Achievement A1 = Achievement("Throw10");
	
	//sparad info
	int SortedBoxes = 0;
};
