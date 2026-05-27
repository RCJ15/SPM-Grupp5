// Contains data about a song used on the Radio
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FMODEvent.h"
#include "SongAsset.generated.h"

UCLASS(BlueprintType)
class SPMG5_API USongAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Song Data")
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Song Data")
	FString Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Song Data")
	float Bpm;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Song Data")
	float BpmDelay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Song Data")
	UFMODEvent* Event;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Song Data")
	UFMODEvent* RadioHostBefore;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Song Data")
	UFMODEvent* RadioHostAfter;
};