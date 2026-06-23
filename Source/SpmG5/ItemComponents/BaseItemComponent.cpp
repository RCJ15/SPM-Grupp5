// Marcus hopefully approves of this.

#include "Item.h"
#include "BaseItemComponent.h"


// Sets default values for this component's properties
UBaseItemComponent::UBaseItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UBaseItemComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<AItem>(GetOwner());
}


// Called every frame
void UBaseItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}

int UBaseItemComponent::GetPoints()
{
	return 0;
}

