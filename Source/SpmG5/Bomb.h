// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "DangerousItem.h"
#include "Bomb.generated.h"

/**
 * 
 */
UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExampleDelegate_OnSomething);

UCLASS(BlueprintType, Blueprintable)
class SPMG5_API ABomb : public ADangerousItem
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> Shake;
	
	UPROPERTY(EditAnywhere)
	bool bDoExplode = false; //används för att testa explosion
	UFUNCTION(BlueprintCallable)
	void Explode();
	
	virtual void OnEndTimer_Implementation() override;
	
	UPROPERTY(Blueprintable, BlueprintReadWrite, BlueprintAssignable)
	FExampleDelegate_OnSomething ExampleDelegateVariable;
};
