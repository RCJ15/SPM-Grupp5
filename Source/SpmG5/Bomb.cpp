// Marcus hopefully approves of this.


#include "Bomb.h"
#include "ConveyorBelt.h"
#include "SpmG5Character.h"
#include "Kismet/GameplayStatics.h"

void ABomb::BeginPlay()
{
	//Ändra möjliga värden innan man kör parent begin play 
	//då den startar timern
	Lifetime = 10; //sätter lifetime till något nytt
	StartShakeAt = 5;
	Super::BeginPlay();
	ExampleDelegateVariable.AddUniqueDynamic(this, &ABomb::Explode); //subscribea på Explode metod
	
}

// Called every frame
void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bDoExplode)
	{
		ExampleDelegateVariable.Broadcast(); //denna kallas för att trigga explosion
		//Explode();
		bDoExplode = false;
	}
}

void ABomb::Explode()
{
	//skapa sweepsphere
	TArray<FHitResult> Hit;
	float Radius = 200.0f;
	//FVector End = GetActorLocation() + GetActorForwardVector() * Radius;
	FQuat Rotation = GetActorRotation().Quaternion();
		
	GetWorld()->SweepMultiByChannel(Hit,GetActorLocation(), GetActorLocation(), Rotation, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(Radius));

	//för varje item 
	for (auto i : Hit)
	{
		if (i.GetActor() && i.GetComponent() && Cast<AItem>(i.GetActor()))
		{
			AItem* OtherItem = Cast<AItem>(i.GetActor());
			//check if connected to conveyor
			if (OtherItem->Conveyor != nullptr)
			{
				//disconnect from conveyor
				OtherItem->Conveyor->DropItem(OtherItem);
				OtherItem->Conveyor = nullptr;
			}
			
			//add impulse
			OtherItem->AddImpulse(GetActorLocation(), 5); //5 temp test för strength
			
		}
		if (i.GetActor() && i.GetComponent() && Cast<ASpmG5Character>(i.GetActor()))
		{
			ASpmG5Character* Character = Cast<ASpmG5Character>(i.GetActor());
			//spelaren måste droppa item
			Character->Drop();
		}
	}
	if (Shake != nullptr)
		UGameplayStatics::PlayWorldCameraShake(this, Shake, GetActorLocation(),0,30000,1,false);
	//spela partikel effekt vid GetActorLocation()
	
	Disintegrate();
}

void ABomb::OnEndTimer_Implementation()
{
	Super::OnEndTimer_Implementation();
	ExampleDelegateVariable.Broadcast(); //denna kallas för att trigga explosion
}
