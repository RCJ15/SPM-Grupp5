// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "FMODAudioComponent.h"
#include "NiagaraSystem.h"
#include "ItemComponents/BaseItemComponent.h"
#include "ExplosiveComponent.generated.h"

/**
 * 
 */
UCLASS()
class SPMG5_API UExplosiveComponent : public UBaseItemComponent
{
	GENERATED_BODY()

public:	
	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void Explode();
	
	UFUNCTION(BlueprintCallable)
	void StartCountDownTimer();
		
private:
	UPROPERTY(EditAnywhere)
	float Lifetime = 10.0f;
	
	UPROPERTY(EditAnywhere)
	FTimerHandle BadBoxTimer;
	
	UPROPERTY(EditAnywhere)
	bool ShakeWhenTimerLow;
	
	UPROPERTY(EditAnywhere)
	float StartShakeAt = 10.f;
	
	UPROPERTY(EditAnywhere)
	float ShakeRate = 0.1f;
	
	UPROPERTY(EditAnywhere)
	float ShakeIntensity = 5.f;
	
	UFUNCTION()
	void StartShake();
	
	void Shake();
	
	/* VFX */
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* ExplosionParticles;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> CameraShake;
	
	/* SFX */
	float FuseSFXTimer = 0;
	
	UPROPERTY(EditAnywhere, Category="Audio")
	UFMODEvent* FuseSFX;
	
	UPROPERTY(EditAnywhere, Category="Audio")
	UFMODEvent* ExplodeSFX;
	
	UPROPERTY()
	UFMODAudioComponent* FuseSFXInstance;
};
