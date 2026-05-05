// Marcus hopefully approves of this.

#include "BoxDestroyer.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABoxDestroyer::ABoxDestroyer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CoreMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoreMesh"));
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	
	SetRootComponent(CoreMesh);
	BaseMesh->SetupAttachment(CoreMesh);
	LidMesh->SetupAttachment(BaseMesh);
}

void ABoxDestroyer::BeginPlay()
{
	Super::BeginPlay();
	
	// So far, box destruction is called when a box touches the lid of the mesh
	LidMesh->OnComponentHit.AddDynamic(this, &ABoxDestroyer::OnHit);
}

void ABoxDestroyer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoxDestroyer::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		AItem* Item = Cast<AItem>(OtherActor);
		
		if (Item)
		{
			if (!IsTrashChute)
			{
				// Add box to be collected's score to score manager
				GetWorld()->GetSubsystem<UScoreManager>()->AddScore(Item->GetPoints());
				//UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Item->GetPoints());
			}
			
			Item->Disintegrate();
		}
	}
}