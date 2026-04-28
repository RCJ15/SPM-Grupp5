// Marcus hopefully approves of this.


#include "Radio.h"

#include "Components/AudioComponent.h"

// Sets default values
ARadio::ARadio()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);
}

//använde det här som källa https://forums.unrealengine.com/t/choosing-random-numbers-in-a-range-all-at-least-once/344734/2
void ARadio::SwitchChannel()
{
	UE_LOG(LogTemp, Warning, TEXT("Switch Channel"));
	//när den kastas i marken ska den byta kanal
	
	//ta en random kanal från 
	
	if (Copy.Num() == 0)
		InitializeCopyArray();
	
	int32 Index = FMath::RandRange(0, Copy.Num() - 1);
	
	//sätt aktiv kanal till SoundWave[Index];
	
	//wchar_t* Name = SoundWave[Index]->GetName();
	UE_LOG(LogTemp, Warning, TEXT("Index: %d and Song: %s"), Copy[Index], *SoundWave[Copy[Index]]->GetName());
	
	//AmbientSound->GetAudioComponent()->Sound=SoundWave[Index]; //det här ser så fel ut
	//AmbientSound->Play();
	
	Copy.RemoveAtSwap(Index);
	
}

//possibly om man håller in en knapp så kommer den stängas av/sättas på beroende på tillstånd
void ARadio::TurnOff()
{
	//stop playing music
	//AmbientSound->Stop();
}

void ARadio::TurnOn()
{
	//start playing music
	//AmbientSound->Play();
}

// Called when the game starts or when spawned
void ARadio::BeginPlay()
{
	Super::BeginPlay();
	
	//gör så att AmbientSound loopar?
	//AmbientSound->GetAudioComponent()->
	InitializeCopyArray();
	//SwitchChannel();
	//TurnOn();
	
	//börja spela music
}

// Called every frame
void ARadio::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSwitchChannel)
	{
		SwitchChannel();
		bSwitchChannel = false;
	}
}

/*void ARadio::OnInteract(ASpmG5Character* InteractingPlayer)
{
}*/

void ARadio::InitializeCopyArray()
{
	UE_LOG(LogTemp, Warning, TEXT("Initialize Copy Array"));
	//initialize copy array with indexes of soundWave
	for (int i = 0; i < SoundWave.Num(); i++)
	{
		Copy.Add(i);
	}
	UE_LOG(LogTemp, Warning, TEXT("Array Initialized with %d slots"), Copy.Num());
}

void ARadio::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	int CurrentSpeed = GetVelocity().Size();
	UE_LOG(LogTemp, Warning, TEXT("RADIO Current Speed: %d"), CurrentSpeed);
	if (OtherActor && OtherActor != this)
	{
		
		UE_LOG(LogTemp, Warning, TEXT("RADIO Current Speed: %d"), CurrentSpeed);
		
		if (CurrentSpeed > SpeedToSwitchChannel)
		{
			SwitchChannel();
		}
	}
}

