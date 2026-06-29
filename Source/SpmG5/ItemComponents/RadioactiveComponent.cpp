// Marcus hopefully approves of this.


#include "ItemComponents/RadioactiveComponent.h"

#include "NiagaraComponent.h"
#include "SpmG5Character.h"

void URadioactiveComponent::BeginPlay()
{
	Super::BeginPlay();
	
	FTransform CenterPosition;
	FVector Min = FVector::ZeroVector;
	FVector Max = FVector::ZeroVector;
	Owner->BaseMesh->GetLocalBounds(Min,Max);
	CenterPosition.SetLocation(FVector(0.0f, 0.0f, (Max.Z - Min.Z)/2));
	
	RadiationRadius = Cast<USphereComponent>(Owner->AddComponentByClass(USphereComponent::StaticClass(),false, CenterPosition, true));
	RadiationRadius->InitSphereRadius(87.5);
	Owner->FinishAddComponent(RadiationRadius, false, CenterPosition);
	Owner->AddInstanceComponent(RadiationRadius);
	
	RadiationRadius->OnComponentBeginOverlap.AddDynamic(this, &URadioactiveComponent::OnOverlapStart);
	RadiationRadius->OnComponentEndOverlap.AddDynamic(this, &URadioactiveComponent::OnOverlapEnd);
	
	RadiationComponent = Cast<UNiagaraComponent>(Owner->AddComponentByClass(UNiagaraComponent::StaticClass(),false, CenterPosition, true));
	RadiationComponent->SetAsset(RadiationFX);
	Owner->FinishAddComponent(RadiationComponent, false, CenterPosition);
	Owner->AddInstanceComponent(RadiationComponent);
}

void URadioactiveComponent::OnOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASpmG5Character* Char = Cast<ASpmG5Character>(OtherActor))
	{
		Char->bIncreaseIncapacitation = true;
	}
}

void URadioactiveComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ASpmG5Character* Char = Cast<ASpmG5Character>(OtherActor))
	{
		Char->bIncreaseIncapacitation = false;
	}
}