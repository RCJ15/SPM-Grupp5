// Marcus hopefully approves of this.


#include "DangerousItem.h"


void ADangerousItem::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(
	BadTimer,
	this,
	&ADangerousItem::OnEndTimer,
	Timer,
	false
	);
	if (ShakeWhenTimerLow)
	{
		FTimerHandle ShakeWhenLowTimer;
		GetWorld()->GetTimerManager().SetTimer(
		ShakeWhenLowTimer,
		this,
		&ADangerousItem::StartShake,
		Timer- StartShakeAt,
		false
		);
	}

}

void ADangerousItem::OnEndTimer_Implementation()
{
}

void ADangerousItem::StartShake()
{
	FTimerHandle ShakeWhenLowTimer;
	GetWorld()->GetTimerManager().SetTimer(
	ShakeWhenLowTimer,
	this,
	&ADangerousItem::Shake,
	ShakeRate,
	true
	);
}

void ADangerousItem::Shake()
{
	BaseMesh->SetRelativeLocation(FVector(FMath::RandRange(-ShakeIntensity,ShakeIntensity), FMath::RandRange(-ShakeIntensity,ShakeIntensity), 0.0f));
}
