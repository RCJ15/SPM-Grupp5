// Marcus hopefully approves of this.

#include "ItemComponents/ExplosiveComponent.h"
#include "CanBeBroken.h"
#include "ConveyorBeltUpgraded.h"
#include "FMODBlueprintStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "SpmG5Character.h"
#include "Kismet/GameplayStatics.h"

void UExplosiveComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (ShakeWhenTimerLow)
	{
		FTimerHandle ShakeWhenLowCountdownTimer;
		
		// Decides after how long a box should start shaking
		GetWorld()->GetTimerManager().SetTimer(
			ShakeWhenLowCountdownTimer,
			this,
			&UExplosiveComponent::StartShake,
			Lifetime - StartShakeAt,
			false
		);
	}	
}

void UExplosiveComponent::StartCountDownTimer()
{
	GetWorld()->GetTimerManager().SetTimer(
		BadBoxTimer,
		this,
		&UExplosiveComponent::Explode,
		Lifetime,
		false);
}

void UExplosiveComponent::StartShake()
{
	FTimerHandle ShakeWhenLowTimer;
	
	// Decides how often a box should shake
	GetWorld()->GetTimerManager().SetTimer(
		ShakeWhenLowTimer,
		this,
		&UExplosiveComponent::Shake,
		ShakeRate,
		true);
}

void UExplosiveComponent::Shake()
{
	FVector OldLoc = Owner->GetActorLocation();
	Owner->SetActorLocation(FVector(OldLoc.X + FMath::RandRange(-ShakeIntensity,ShakeIntensity), OldLoc.Y + FMath::RandRange(-ShakeIntensity,ShakeIntensity), OldLoc.Z));
}

void UExplosiveComponent::Explode()
{
	// Setup sweep sphere
	TArray<FHitResult> Hit;
	float Radius = 200.0f;
	FQuat Rotation = Owner->GetActorRotation().Quaternion();
	
	// Trace channel for Item, character and UCanBeBroken
	GetWorld()->SweepMultiByChannel(Hit,Owner->GetActorLocation(), Owner->GetActorLocation(), Rotation, ECC_GameTraceChannel6, FCollisionShape::MakeSphere(Radius));
	
	for (auto i : Hit)
	{
		if (i.GetActor() && i.GetComponent() && Cast<ASpmG5Character>(i.GetActor()))
		{
			ASpmG5Character* Character = Cast<ASpmG5Character>(i.GetActor());
			Character->Drop();
			Character->AddVelocity((Owner->GetActorLocation() - Character->GetActorLocation())/Owner->GetActorLocation()*10);
			Character->DoRagdoll();
		}
		else if (i.GetActor() && i.GetComponent() && Cast<AItem>(i.GetActor()))
		{
			AItem* OtherItem = Cast<AItem>(i.GetActor());
			//check if connected to conveyor
			if (OtherItem->ConveyorUpgraded != nullptr)
			{
				//disconnect from conveyor
				OtherItem->ConveyorUpgraded->RemoveFromBelt(OtherItem);
				OtherItem->ConveyorUpgraded = nullptr;
			}
			
			//add impulse
			if (OtherItem->BaseMesh->IsSimulatingPhysics())
				OtherItem->AddImpulse(Owner->GetActorLocation(), 5); //5 temp test för strength
			// if (OtherItem->GetIsFragile())
			// 	OtherItem->Disintegrate(false);
		}		
		else if (i.GetActor() && i.GetComponent() && i.GetActor()->Implements<UCanBeBroken>())
		{
			UE_LOG(LogTemp, Warning, TEXT("FGUCKKK"));
			AActor* Thing = Cast<AActor>(i.GetActor());
			//ICanBeBroken* BreakableActor = Cast<ICanBeBroken>(Thing);
			//spelaren måste droppa item
			
			Cast<ICanBeBroken>(Thing)->Break();
		}
	}
	if (CameraShake != nullptr)
		UGameplayStatics::PlayWorldCameraShake(this, CameraShake, Owner->GetActorLocation(),0,30000,1,false);
	
	//EXPLODE SFX - Ruben
	UFMODBlueprintStatics::PlayEventAtLocation(this, ExplodeSFX, FTransform(Owner->GetActorLocation()), true);
	FuseSFXInstance->Stop();
	
	//spela partikel effekt vid GetActorLocation()
	if (ExplosionParticles)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),ExplosionParticles,Owner->GetActorLocation(),Owner->GetActorRotation());
	
	if (Owner->GetMostRecentHolder())
	{
		ASpmG5Character* HoldingPlayer = Cast<ASpmG5Character>(Owner->GetMostRecentHolder());
		HoldingPlayer->Drop();
	}
	
	Owner->Disintegrate(false);
}
