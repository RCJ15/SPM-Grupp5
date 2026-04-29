// Marcus hopefully approves of this.


#include "BlowPipe.h"

// Sets default values
ABlowPipe::ABlowPipe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABlowPipe::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlowPipe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

