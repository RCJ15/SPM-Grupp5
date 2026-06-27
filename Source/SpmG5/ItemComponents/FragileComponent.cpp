// Marcus hopefully approves of this.


#include "ItemComponents/FragileComponent.h"
#include  "Item.h"
#include "NiagaraFunctionLibrary.h"

bool UFragileComponent::ShouldBreak()
{
	// Checks with how much force fragile item hits something
	int CurrentSpeed = Owner->GetVelocity().Size();
		
	if (CurrentSpeed > MaxSpeed)
	{
		
		ShouldBreakOnImpact = true;
	}
	
	return ShouldBreakOnImpact;	
}

void UFragileComponent::OnHit(AActor* OtherActor, FVector NormalImpulse)
{
	if (Owner->GetMostRecentHolder() && Owner->GetMostRecentHolder() != OtherActor)
	{
		if (ShouldBreak())
		{
			Owner->PrepareDestroy();		
		}
	}
}

void UFragileComponent::OnItemDestroy()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BreakEffect, Owner->GetActorLocation(), Owner->GetActorRotation());
}

void UFragileComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner->OnHitResult.AddDynamic(this, &UFragileComponent::OnHit);
}