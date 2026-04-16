// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	BaseMesh->OnComponentHit.AddDynamic(this, &AItem::OnHit);
	
	/*if (IsLarge)
	{
		SetActorScale3D(FVector(1.5, 0.7, 0.7));
	}*/
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItem::CalculateIfBreakIfFragile()
{
	if (IsFragile)
	{
		int CurrentSpeed = GetVelocity().Size();
		UE_LOG(LogTemp, Warning, TEXT("Current Speed: %d"), CurrentSpeed);
		
		if (CurrentSpeed > MaxSpeedIfFragile)
		{
			Disintegrate();
		}
	}
}

void AItem::Disintegrate()
{
	Destroy();
}

void AItem::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		if (MostRecentHolder)
		{
			if (OtherActor != MostRecentHolder)
			{
				MostRecentHolder = nullptr;
				CalculateIfBreakIfFragile();
			}
		}
	}
}

bool AItem::GetIsFragile()
{
	return IsFragile;
}

bool AItem::GetIsLarge()
{
	return IsLarge;
}

void AItem::SetMostRecentHolder(AActor* holder)
{
	MostRecentHolder = holder;
	//return (MostRecentHolder) ? MostRecentHolder : nullptr;
}
