// Marcus hopefully approves of this.


#include "Radio.h"

#include "FMODBlueprintStatics.h"
#include "Components/AudioComponent.h"

int32 LastSongIndex;

// Sets default values
ARadio::ARadio()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);
}

// Called when the game starts or when spawned
void ARadio::BeginPlay()
{
	Super::BeginPlay();
	
	BaseMesh->OnComponentHit.AddDynamic(this, &ARadio::OnHit);
	
	InitializeCopyArray();
	TurnOn();
}

//använde det här som källa https://forums.unrealengine.com/t/choosing-random-numbers-in-a-range-all-at-least-once/344734/2
void ARadio::SwitchChannel()
{
	if (CurrentInstance)
	{
		CurrentInstance->Stop();
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Switch Channel"));
	//när den kastas i marken ska den byta kanal
	
	//ta en random kanal från 
	
	if (Copy.Num() == 0) {
		InitializeCopyArray();
	}
	
	int32 Index = FMath::RandRange(0, Copy.Num() - 1);
	
	if (Copy[Index] == LastSongIndex) {
		Index++;
		Index %= Copy.Num();
	}
	
	LastSongIndex = Copy[Index];
	
	//sätt aktiv kanal till SoundWave[Index];
	
	//wchar_t* Name = SoundWave[Index]->GetName();
	UE_LOG(LogTemp, Warning, TEXT("Index: %d and Song: %s"), Copy[Index], *Songs[Copy[Index]]->GetName());
	
	//AmbientSound->GetAudioComponent()->Sound=SoundWave[Index]; //det här ser så fel ut
	//AmbientSound->Play();
	
	UFMODEvent* Evt = Songs[Copy[Index]];
	
	CurrentInstance = UFMODBlueprintStatics::PlayEventAttached(
		Evt, //FMOD event asset
		BaseMesh, //component to attach to
		NAME_None, //optional socket name
		FVector::ZeroVector, 
		EAttachLocation::KeepRelativeOffset, 
		true, 
		true, 
		true
		);

	Copy.RemoveAtSwap(Index);
	
}

//possibly om man håller in en knapp så kommer den stängas av/sättas på beroende på tillstånd
void ARadio::TurnOff()
{
	//stop playing music
	//AmbientSound->Stop();
	if (CurrentInstance)
		CurrentInstance->Stop();
	CurrentInstance = nullptr;
}

void ARadio::TurnOn()
{
	//start playing music
	//AmbientSound->Play();
	if (CurrentInstance)
		CurrentInstance->Play();
	if (!CurrentInstance)
		SwitchChannel();
		
}


// Called every frame
void ARadio::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}


void ARadio::InitializeCopyArray()
{
	UE_LOG(LogTemp, Warning, TEXT("Initialize Copy Array"));
	//initialize copy array with indexes of songs
	for (int i = 0; i < Songs.Num(); i++)
	{
		Copy.Add(i);
	}
	UE_LOG(LogTemp, Warning, TEXT("Array Initialized with %d slots"), Copy.Num());
}

void ARadio::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	//om man kastar radion så byter man kanal
	//FAST DÅ MÅSTE MAN KUNNA TA UPP RADIOIN!!!!
	/*
	int CurrentSpeed = GetVelocity().Size();
	UE_LOG(LogTemp, Warning, TEXT("RADIO Current Speed: %d"), CurrentSpeed);
	if (OtherActor && OtherActor != this)
	{
		
		UE_LOG(LogTemp, Warning, TEXT("RADIO Current Speed: %d"), CurrentSpeed);
		
		if (CurrentSpeed > SpeedToSwitchChannel)
		{
			SwitchChannel();
		}
	}*/
}

