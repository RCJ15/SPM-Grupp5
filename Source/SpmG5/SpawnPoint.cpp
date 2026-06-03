// Marcus hopefully approves of this.


#include "SpawnPoint.h"

#include "SpmG5Character.h"
#include "Kismet/GameplayStatics.h"


void ASpawnPoint::BeginPlay()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), PlayerIndex);

	if (IsValid(PlayerPawn))
	{
		PlayerPawn->SetActorLocation(GetActorLocation());
		Cast<ASpmG5Character>(PlayerPawn)->SetSpawnLocation(GetActorLocation());
	}
}