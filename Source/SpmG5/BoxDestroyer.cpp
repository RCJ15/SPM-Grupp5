// Marcus hopefully approves of this.

#include "BoxDestroyer.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABoxDestroyer::ABoxDestroyer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);
}

// Called when the game starts or when spawned
void ABoxDestroyer::BeginPlay()
{
	Super::BeginPlay();
	
	BaseMesh->OnComponentHit.AddDynamic(this, &ABoxDestroyer::OnHit);
}

// Called every frame
void ABoxDestroyer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoxDestroyer::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("SOMETHING HIT IT"));
	
	if (OtherActor && OtherActor != this)
	{
		AItem* Item = Cast<AItem>(OtherActor);
		
		if (Item)
		{
			Item->Disintegrate();
		}
	}
}