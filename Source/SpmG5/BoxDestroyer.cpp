// Marcus hopefully approves of this.

#include "BoxDestroyer.h"

#include "GameManager.h"

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
	
	OnInteractedWithDestoyer.AddUniqueDynamic(this, &ABoxDestroyer::test);
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
				if (!Item->GetHasBeenDroppedOff())
				{
					HandleBox(Item);
				}
				int Score = Item->GetPoints();
				UE_LOG(LogTemp, Error, TEXT("Item Score: %d"), Score);
				GetWorld()->GetSubsystem<UScoreManager>()->AddScore(Score);
				UGameManager* GameManager = Cast<UGameManager>(GetWorld()->GetGameInstance());
				GameManager->BoxDead();
				Item->Destroy();
				//UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Item->GetPoints());
			}
			else
			{
				Item->Disintegrate(true);
			}
			OnInteractedWithDestoyer.Broadcast();
		}
	}
}
