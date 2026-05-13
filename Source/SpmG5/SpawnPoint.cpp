// Marcus hopefully approves of this.


#include "SpawnPoint.h"

#include "Kismet/GameplayStatics.h"


void ASpawnPoint::BeginPlay()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), PlayerIndex);

	if (IsValid(PlayerPawn))
	{
		PlayerPawn->SetActorLocation(GetActorLocation());
	}
}