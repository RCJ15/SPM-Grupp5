// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

class ASpmG5Character;
// This class does not need to be modified.
UINTERFACE(Blueprintable, MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPMG5_API IInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnInteract(ASpmG5Character* InteractingPlayer);
	
	//virtual void OnInteract_Implementation(ASpmG5Character* InteractingPlayer);
};
