// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "DangerousItem.generated.h"

/**
 * 
 */
UCLASS()
class SPMG5_API ADangerousItem : public AItem
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Timer = 30.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle BadTimer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShakeWhenTimerLow;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartShakeAt = 10.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShakeRate = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShakeIntensity = 5.f;


public:
	UFUNCTION(BlueprintNativeEvent)
	void OnEndTimer();
	
	virtual void OnEndTimer_Implementation();
	
private:
	UFUNCTION()
	void StartShake();
	
	void Shake();
};


