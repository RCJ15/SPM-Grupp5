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
	
protected:
	UPROPERTY()
	USphereComponent* RadiationRadius;
	
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* RadiationFX;
	
	UPROPERTY()
	UNiagaraComponent* RadiationComponent;
	
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	
};
