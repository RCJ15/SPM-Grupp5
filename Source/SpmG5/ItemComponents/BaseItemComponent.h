// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseItemComponent.generated.h"

class AItem;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPMG5_API UBaseItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	virtual void BeginPlay() override;
	virtual void OnItemDestroy(){}
	virtual int GetPoints();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void BreakCaller(AActor* Actor);
	
protected:
	UPROPERTY(BlueprintReadOnly)
	AItem* Owner;
};