// Marcus hopefully approves of this.

#pragma once

#include <string>

#include "CoreMinimal.h"
//this class will unlock achievements
//USTRUCT(Blueprintable)
struct FAchievement : public FTableRowBase
{
	//GENERATED_BODY()
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Index; // index i arrayen/tabellen, kanske inte neccessary
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	std::string Name;
	//image icon
	std::string Description; //beskrivning som kan komma upp när man visar detta possibly
	bool bIsUnlocked = false;
	int ToReachEventCount = 1;//håller reda på hur många gånger något ska ha hänt för att unlocka detta achievement
	int CurrentEventCount = 0; //håller reda på hur många gånger detta faktiskt har hänt
	
	//Mesh CosmeticToUnlock //alternativt string till dens path
};

class SPMG5_API AchievementManager
{
public:
	AchievementManager();
	~AchievementManager();
private:
	TArray<FAchievement> Achievements;
	
	bool LoadAchievements();
	bool SaveAchievements();
	
	void IncreaseAchievementCount(int AchievementIndex);
	void UnlockAchievement(int AchievementIndex);
	//En sådan här metod för varje achievement som kollar om allt uppfylls:
	void CheckFulfillAchievement0();
	void CheckFulfillAchievement1();
	void CheckFulfillAchievement2();
	void CheckFulfillAchievement3();
};
