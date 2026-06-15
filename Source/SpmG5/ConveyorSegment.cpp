// Fill out your copyright notice in the Description page of Project Settings.


#include "ConveyorBelt.h"
#include "ConveyorBeltUpgraded.h"

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
			if (!BeltUpgraded)
				return;
			if (BeltUpgraded->ItemIsAtEndOfBelt(Item))
				return;
			BeltUpgraded->AddToBelt(Item);
		}
	}
}

