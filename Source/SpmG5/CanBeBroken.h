// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CanBeBroken.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UCanBeBroken : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPMG5_API ICanBeBroken
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Break();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Repair();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Finished();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Cancelled();
};
