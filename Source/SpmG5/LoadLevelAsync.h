// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "LoadLevelAsync.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelLoaded);
/**
 * 
 */
UCLASS()
class SPMG5_API ULoadLevelAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintAssignable)
	FOnLevelLoaded OnLevelLoaded;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
	static ULoadLevelAsync* LoadLevelAsync(UObject* WorldContextObject, TSoftObjectPtr<UWorld> Level);

	virtual void Activate() override;

private:

	UPROPERTY()
	UObject* WorldContextObject;

	UPROPERTY()
	TSoftObjectPtr<UWorld> Level;

	UFUNCTION()
	void Finish();
};
