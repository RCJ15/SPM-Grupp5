// Marcus hopefully approves of this.


#include "ItemComponents/RadioactiveComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "SpmG5Character.h"

void URadioactiveComponent::BeginPlay()
{
	Super::BeginPlay();
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),Radiation,Owner->GetActorLocation(),Owner->GetActorRotation());
	
	RadiationRadius = NewObject<USphereComponent>(Owner, USphereComponent::StaticClass(), TEXT("RadiationRadius"));
	
	RadiationRadius->InitSphereRadius(3.255);
	RadiationRadius->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RadiationRadius->SetVisibility(true);
	
	RadiationRadius->SetGenerateOverlapEvents(true);
	
	RadiationRadius->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	RadiationRadius->RegisterComponent();
	
	
	RadiationRadius->OnComponentBeginOverlap.AddDynamic(this, &URadioactiveComponent::OnOverlapStart);
	RadiationRadius->OnComponentEndOverlap.AddDynamic(this, &URadioactiveComponent::OnOverlapEnd);
	
}

void URadioactiveComponent::OnOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASpmG5Character* Char = Cast<ASpmG5Character>(OtherActor))
		Char->IncreaseIncapacitation(true);
}

void URadioactiveComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ASpmG5Character* Char = Cast<ASpmG5Character>(OtherActor))
		Char->IncreaseIncapacitation(false);
}


