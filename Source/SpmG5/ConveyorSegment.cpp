// Fill out your copyright notice in the Description page of Project Settings.


#include "ConveyorBelt.h"

#include "Item.h"
#include "Components/StaticMeshComponent.h"
#include "DSP/BufferDiagnostics.h"


// Sets default values
AConveyorSegment::AConveyorSegment()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);
}

// Called when the game starts or when spawned
void AConveyorSegment::BeginPlay()
{
	Super::BeginPlay();
	
	BaseMesh->OnComponentHit.AddDynamic(this, &AConveyorSegment::OnHit);
}

// Called every frame
void AConveyorSegment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

FVector AConveyorSegment::GetForward()
{
	//returnar Arrowns riktning
	
	return Arrow->GetForwardVector();
}

void AConveyorSegment::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		if (AItem* Item = Cast<AItem>(OtherActor))
		{
			if (!Belt)
				return;
			//BORDE GÖRAS PÅ ETT ANNAT SÄTT, DET HÄR ÄR LITE AV EN TEMP LÖSNING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//kolla att item inte precis ska droppas från conveyorn
			//så kolla om detta är last segment isf ignorera
			if (Belt->Conveyor[Belt->MaxItems-1] == this)
				return;
			
			
			UE_LOG(LogTemp, Warning, TEXT("Should add item to conveyor"));
			
			AConveyorSegment* Segment = this;
			//NOTE FÖR FRAMTIDEN ISTÄLLET FÖR ATT KOLLA OM DEN ÄR ÖVER 0.5 och byta
			//KOLLA ATT DEN ÄR UNDER 0.25 på current segment, 
			//eller över 0.75 på previous segment
			if (Belt->MovedDelta > 0.5)				
				Segment = Belt->Conveyor[Segment->IndexInConveyorBelt-1];				
			if (!Belt->HasItemInSegment(Segment))
			{
				Belt->ReceiveItem(Item,Segment);
			}
			
		}
	}
	
	/*
	//det här kanske borde vara på conveyor segment istället?
	//Kolla om den kolliderar med ett conveyor segment
	if (Cast<AConveyorSegment>(OtherActor))
	{
		if (AConveyorSegment* Segment = Cast<AConveyorSegment>(OtherActor))
		{
			//kolla om segment är tomt
			if (AConveyorBelt* Belt = Segment->Belt)
			{
				if (Segment->IndexInConveyorBelt == 0)
					return;
				//NOTE FÖR FRAMTIDEN ISTÄLLET FÖR ATT KOLLA OM DEN ÄR ÖVER 0.5 och byta
				//KOLLA ATT DEN ÄR UNDER 0.25 på current segment, 
				//eller över 0.75 på previous segment
				if (Belt->MovedDelta > 0.5)				
					Segment = Belt->Conveyor[Segment->IndexInConveyorBelt-1];				
				if (!Belt->HasItemInSegment(Segment))
				{
					Belt->ReceiveItem(this,Segment);	
				}
			}
		}
	}*/
	
}

