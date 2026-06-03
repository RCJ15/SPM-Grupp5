// Marcus hopefully approves of this.


#include "DangerousItem.h"

void ADangerousItem::BeginPlay()
{
	Super::BeginPlay();
	
	if (ShakeWhenTimerLow)
	{
		FTimerHandle ShakeWhenLowCountdownTimer;
		
		// Decides after how long a box should start shaking
		GetWorld()->GetTimerManager().SetTimer(
			ShakeWhenLowCountdownTimer,
			this,
			&ADangerousItem::StartShake,
			Lifetime - StartShakeAt,
			false
		);
	}
}

void ADangerousItem::StartCountDownTimer()
{
	// Decides what should happen when box's lifetime runs out
	GetWorld()->GetTimerManager().SetTimer(
		BadBoxTimer,
		this,
		&ADangerousItem::OnEndTimer,
		Lifetime,
		false
	);
}

void ADangerousItem::OnEndTimer_Implementation()
{
	// Should be implemented by subclasses
}

void ADangerousItem::StartShake()
{
	FTimerHandle ShakeWhenLowTimer;
	
	// Decides how often a box should shake
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
	//BaseMesh->SetRelativeLocation(FVector(FMath::RandRange(-ShakeIntensity,ShakeIntensity), FMath::RandRange(-ShakeIntensity,ShakeIntensity), 0.0f));
	FVector OldLoc = GetActorLocation();
	SetActorLocation(FVector(OldLoc.X + FMath::RandRange(-ShakeIntensity,ShakeIntensity), OldLoc.Y + FMath::RandRange(-ShakeIntensity,ShakeIntensity), OldLoc.Z));
}