// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Components/SphereComponent.h"
#include "ItemComponents/BaseItemComponent.h"
#include "RadioactiveComponent.generated.h"

/**
 * 
 */
UCLASS()
class SPMG5_API URadioactiveComponent : public UBaseItemComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	virtual int GetPoints() override;
	
protected:
	UFUNCTION()
	void OnOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY()
	USphereComponent* RadiationRadius;
	
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* RadiationFX;
	
	UPROPERTY()
	UNiagaraComponent* RadiationComponent;

private:
	int Points = 10;
	
};
