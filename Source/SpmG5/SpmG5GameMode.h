// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpmG5GameMode.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class ASpmG5GameMode : public AGameModeBase
{
	GENERATED_BODY()
private:
	FTimerHandle TimerHandle;
public:
	
	/** Constructor */
	ASpmG5GameMode();
	void Timer();
};



