// Marcus hopefully approves of this.


#include "BlowPipe.h"

#include "SpmG5Character.h"

// Sets default values
ABlowPipe::ABlowPipe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	//SetRootComponent(SceneComponent);
	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	SceneComponent->SetupAttachment(BoxMesh);
}



// Called when the game starts or when spawned
void ABlowPipe::BeginPlay()
{
	Super::BeginPlay();
	BLowLoop(BlowRate);
}

void ABlowPipe::ChangeBlowState()
{
	if (!Blowing)
		Blowing = true;
	else	
		Blowing = false;	
}

void ABlowPipe::Blow(float DeltaTime)
{
	TArray<FHitResult> HitResult;
	FVector Location = GetActorLocation() + Offcset;	
	FVector End = Location * GetActorForwardVector() * 5;
	FCollisionShape Box = FCollisionShape::MakeBox(BlowBoxSize);
	FQuat Rotation = GetActorRotation().Quaternion();
	GetWorld()->SweepMultiByChannel(HitResult,Location, End, Rotation, ECC_GameTraceChannel4,Box);

	
	DrawDebugBox(GetWorld(), Location, FVector(BlowBoxSize.Y, BlowBoxSize.X, BlowBoxSize.Z), FColor::Red, false, 1);

	for (FHitResult Result : HitResult)
	{
		Result.GetComponent()->GetOwner()->SetActorLocation(Result.GetComponent()->GetOwner()->GetActorLocation() + GetActorForwardVector() * ForceMultiplier * DeltaTime);
	}
	
}

void ABlowPipe::BLowLoop(float LoopRate)
{
	GetWorld()->GetTimerManager().SetTimer(BlowLoopTimer, this, &ABlowPipe::ChangeBlowState, LoopRate, true, 1);
}

// Called every frame
void ABlowPipe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Blowing)
		Blow(DeltaTime);

}

