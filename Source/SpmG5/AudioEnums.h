#pragma once

#include "CoreMinimal.h"
#include "AudioEnums.generated.h"
 
// https://dev.epicgames.com/community/learning/tutorials/rMD2/how-to-create-custom-enums-and-structs-in-unreal-engine

UENUM(BlueprintType)
enum class EAudioGroundType : uint8
{
	Option_A UMETA(DisplayName = "None"),
	Option_B UMETA(DisplayName = "Wood"),
	Option_C UMETA(DisplayName = "Metal")
};

UENUM(BlueprintType)
enum class EAudioItemType : uint8
{
	Option_A UMETA(DisplayName = "None"),
	Option_B UMETA(DisplayName = "Box"),
	Option_C UMETA(DisplayName = "Glass")
};