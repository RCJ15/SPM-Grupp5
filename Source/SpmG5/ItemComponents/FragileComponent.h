// Marcus hopefully approves of this.

#pragma once

#include "NiagaraSystem.h"
#include "CoreMinimal.h"
#include "Item.h"
#include "ItemComponents/BaseItemComponent.h"
#include "FragileComponent.generated.h"

/**
 * 
 */



UCLASS()
class SPMG5_API UFragileComponent : public UBaseItemComponent
{
	GENERATED_BODY()
	
public:
	virtual int GetPoints() override {return Points;}
	virtual void OnItemDestroy() override;
	
protected:
	virtual void BeginPlay() override;
		
private:
	bool ShouldBreak();
	UFUNCTION()
	void OnHit(AActor* OtherActor, FVector NormalImpulse);
		
	int Points = 5;
	float MaxSpeed = 400;
	bool ShouldBreakOnImpact = false;
	
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* BreakEffect;
	
};
