// Marcus hopefully approves of this.


#include "SpawnAI.h"

// Sets default values
ASpawnAI::ASpawnAI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpawnAI::BeginPlay()
{
	Super::BeginPlay();
	
	ConvertAllPercentageToBoxes();
	/*UE_LOG(LogTemp, Warning, TEXT("Fragile boxes: %d"), FragileBoxesPercentage);
	UE_LOG(LogTemp, Warning, TEXT("Fragile boxes remaining: %d"), RemainingFragileBoxes);
	UE_LOG(LogTemp, Warning, TEXT("Large boxes: %d"), LargeBoxesPercentage);
	UE_LOG(LogTemp, Warning, TEXT("Large boxes remaining: %d"), RemainingLargeBoxes);
	UE_LOG(LogTemp, Warning, TEXT("Bomb boxes: %d"), BombBoxesPercentage);
	UE_LOG(LogTemp, Warning, TEXT("Bomb boxes remaining: %d"), RemainingBombBoxes);
	UE_LOG(LogTemp, Warning, TEXT("ToxicWaste boxes: %d"), ToxicWasteBoxesPercentage);
	UE_LOG(LogTemp, Warning, TEXT("ToxicWaste boxes remaining: %d"), RemainingToxicWasteBoxes);
	UE_LOG(LogTemp, Warning, TEXT("FlashBang boxes: %d"), FlashBangBoxesPercentage);
	UE_LOG(LogTemp, Warning, TEXT("FlashBang boxes remaining: %d"), RemainingFlashBangBoxes);*/
}

// Called every frame
void ASpawnAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawnAI::ConvertAllPercentageToBoxes()
{
	ConvertPercentageToBox(FragileBoxesPercentage, RemainingFragileBoxes);
	ConvertPercentageToBox(LargeBoxesPercentage, RemainingLargeBoxes);
	ConvertPercentageToBox(BombBoxesPercentage, RemainingBombBoxes);
	ConvertPercentageToBox(ToxicWasteBoxesPercentage, RemainingToxicWasteBoxes);
	ConvertPercentageToBox(FlashBangBoxesPercentage, RemainingFlashBangBoxes);
}

void ASpawnAI::ConvertPercentageToBox(int& Percentage, int& TypeOfRemainingBoxes)
{
	float TempPercentage = Percentage;
	Percentage = AmountOfBoxesPerLevel * (TempPercentage / 100);
	TypeOfRemainingBoxes = Percentage;
}

