// Marcus hopefully approves of this.


#include "BlowPipe.h"

// Sets default values
ABlowPipe::ABlowPipe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	SceneComponent->SetupAttachment(BoxMesh);
}

// Called when the game starts or when spawned
void ABlowPipe::BeginPlay()
{
	Super::BeginPlay();
	BLowLoop(BlowRate);
}

void ABlowPipe::Blow()
{
	TArray<FHitResult> HitResult;
	FVector Location = GetActorLocation();	
	FVector End = Location * GetActorForwardVector() * 5;
	FCollisionShape Box = FCollisionShape::MakeBox(BlowBoxSize);
	FQuat Rotation = GetActorRotation().Quaternion();
	GetWorld()->SweepMultiByChannel(HitResult,Location, End, Rotation, ECC_GameTraceChannel4,Box);

	
	DrawDebugBox(GetWorld(), GetActorLocation(), BlowBoxSize, FColor::Red, false, 1);

	for (FHitResult Result : HitResult)
	{	//kolla om character, om character kalla på dens AddVelocity metod (inte implementerad)
		if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Result.GetComponent()))
			PrimComp->SetPhysicsLinearVelocity(GetActorForwardVector() * 1000);
	}
	
}

void ABlowPipe::BLowLoop(float LoopRate)
{
	GetWorld()->GetTimerManager().SetTimer(BlowLoopTimer, this, &ABlowPipe::Blow, LoopRate, true, -1);
}

// Called every frame
void ABlowPipe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

