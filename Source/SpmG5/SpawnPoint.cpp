// Marcus hopefully approves of this.


#include "SpawnPoint.h"

#include "Kismet/GameplayStatics.h"


void ASpawnPoint::BeginPlay()
{
	UGameplayStatics::GetPlayerPawn(GetWorld(), PlayerIndex)->SetActorLocation(GetActorLocation());
}


