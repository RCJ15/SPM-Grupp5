// Contains data about a song playlist on the Radio
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SongAsset.h"
#include "SongPlaylistAsset.generated.h"

UCLASS(BlueprintType)
class SPMG5_API USongPlaylistAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USongAsset*> Songs;
};