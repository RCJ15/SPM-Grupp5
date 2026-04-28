// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "ConveyorBelt.h"
#include "StateTreeTypes.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);

	PrimComp = Cast<UPrimitiveComponent>(GetRootComponent());
}

void AItem::AddImpulse(FVector Point, float Strength)
{
	//hitta riktning mellan point och this object
	FVector Imp = GetActorLocation() - Point;
	//för att få 1 längd på vektor
	//och 2 riktning
	//lägg velocity på detta objekt
	//i riktningen * strength (/längd på vektor, så längre bort blir påverkad mindre)
	
	UE_LOG(LogTemp, Warning, TEXT("Impulse on object!"));
	//DrawDebugSphere(GetWorld(), GetActorLocation(), 50, 20, FColor::Blue, false, 0.2);
	//this->AddVelocity(10);
	//this->AddVelocity(Strength);
	FVector Impulse = FVector(1,2,1);
	//BaseMesh->AddForce(BaseMesh->GetBodyInstance()->GetBodyMass() * Impulse * 1000);
	PrimComp->AddImpulse(Imp*30 * Strength * Impulse);
}

void AItem::Explode()
{
	//skapa sweepsphere
	TArray<FHitResult> Hit;
	float Radius = 200.0f;
	//FVector End = GetActorLocation() + GetActorForwardVector() * Radius;
	FQuat Rotation = GetActorRotation().Quaternion();
		
	GetWorld()->SweepMultiByChannel(Hit,GetActorLocation(), GetActorLocation(), Rotation, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(Radius));
	//DrawDebugSphere(GetWorld(),GetActorLocation(), Radius, 16, UE::StateTree::Colors::Red);
	//DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 20, FColor::Red, false, 0.1);
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
	}
	
	//spela partikel effekt vid GetActorLocation()
	Disintegrate();
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	BaseMesh->OnComponentHit.AddDynamic(this, &AItem::OnHit);
	
	SetPoints();
	
	/*if (IsLarge)
	{
		SetActorScale3D(FVector(1.5, 0.7, 0.7));
	}*/
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bDoExplode)
	{
		Explode();
		bDoExplode = false;
	}
}

void AItem::SetPoints()
{
	if (IsSuspicious)
	{
		Points = -15;
		return;
	}
	
	if (IsLarge)
	{
		Points += LargeBoxPoints;
	}
	else
	{
		Points += SmallBoxPoints;
	}
	
	if (IsFragile)
	{
		Points += FragileBoxPoints;
	}
}

void AItem::SetPhysics(bool SetTo)
{
	if (SetTo)
		PrimComp->SetCollisionProfileName("DefultItem");
	else 
		PrimComp->SetCollisionProfileName("OnlyRaycast");
	// SetActorEnableCollision(SetTo);
	PrimComp->SetEnableGravity(SetTo);
	PrimComp->SetSimulatePhysics(SetTo);
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

bool AItem::GetIsSuspicious()
{
	return IsSuspicious;
}

int32 AItem::GetPoints()
{
	return Points;
}

void AItem::SetMostRecentHolder(AActor* Holder)
{
	MostRecentHolder = Holder;
	//return (MostRecentHolder) ? MostRecentHolder : nullptr;
}

void AItem::SetIsLarge(bool SetTo)
{
	IsLarge = SetTo;
}

void AItem::SetIsFragile(bool SetTo)
{
	IsFragile = SetTo;
}

void AItem::SetIsSuspicious(bool SetTo)
{
	IsSuspicious = SetTo;
}