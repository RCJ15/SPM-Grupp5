// Marcus hopefully approves of this.

#include "Item.h"
#include "BaseItemComponent.h"


void UBaseItemComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<AItem>(GetOwner());
}

int UBaseItemComponent::GetPoints()
{
	return 0;
}

