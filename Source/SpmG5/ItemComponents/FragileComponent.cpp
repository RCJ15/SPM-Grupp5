// Marcus hopefully approves of this.


#include "ItemComponents/FragileComponent.h"
#include  "Item.h"
#include "NiagaraFunctionLibrary.h"

bool UFragileComponent::ShouldBreak()
{
	// Checks with how much force fragile item hits something
	if (!Owner)
		return false;
	int CurrentSpeed = Owner->GetVelocity().Size();
		
	if (CurrentSpeed > MaxSpeed)
	{
		return true;
	}
	
	return ShouldBreakOnImpact;	
}

void UFragileComponent::OnHit(AActor* OtherActor, FVector NormalImpulse)
{
	if (ShouldBreak() && Owner)
		Owner->PrepareDestroy();		
}

void UFragileComponent::OnItemDestroy()
{
	Super::OnItemDestroy();
	if (!Owner)
		return;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BreakEffect, Owner->GetActorLocation(), Owner->GetActorRotation());
}

void UFragileComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner->OnHitResult.AddUniqueDynamic(this, &UFragileComponent::OnHit);
}

