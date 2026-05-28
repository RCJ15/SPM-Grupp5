#pragma once

#include "CoreMinimal.h"
#include "GameData.generated.h"


USTRUCT(BlueprintType)
struct FGameData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	TMap<FString, int32> HighScores;
	
	UPROPERTY(BlueprintReadWrite)
	float MasterVolume;
	
	UPROPERTY(BlueprintReadWrite)
	float MusicVolume;
	
	UPROPERTY(BlueprintReadWrite)
	float SFXVolume;
	
	UPROPERTY(BlueprintReadWrite)
	bool StopCameraMove;
	
};
