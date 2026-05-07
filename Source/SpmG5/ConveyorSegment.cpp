// Fill out your copyright notice in the Description page of Project Settings.


#include "ConveyorSegment.h"

#include "Components/StaticMeshComponent.h"
#include "DSP/BufferDiagnostics.h"

// Sets default values
AConveyorSegment::AConveyorSegment()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);
}

// Called when the game starts or when spawned
void AConveyorSegment::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AConveyorSegment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AConveyorSegment::GetForward()
{
	//returnar Arrowns riktning
	
	return Arrow->GetForwardVector();
}

