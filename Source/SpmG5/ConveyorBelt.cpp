// Fill out your copyright notice in the Description page of Project Settings.


#include "ConveyorBelt.h"

#if WITH_EDITOR
#include "IDetailTreeNode.h"
#include "MeshUtilitiesCommon.h"
#include "SNegativeActionButton.h"
#endif

#include "Components/SplineComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"


// Sets default values
AConveyorBelt::AConveyorBelt()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//subobject needs to be created here
	Path =  CreateDefaultSubobject<USplineComponent>("Path",true);
}

// Called when the game starts or when spawned
void AConveyorBelt::BeginPlay()
{
	Super::BeginPlay();
	MaxItems = Conveyor.Num(); 
	Items.SetNum(MaxItems) ;
	//UE_LOG(LogTemp, Display, TEXT("Distance Between Items shall be: %f  "), DistBetweenItems);
	
	//fyller Items array med nullpointers
	for( int32 i = 0; i <MaxItems; i++ )
	{
		Items[i] = nullptr;
	}
	//UE_LOG(LogTemp, Display, TEXT("MaxItems: %d  "), MaxItems);
	

	for (int i = 0; i < Conveyor.Num(); i++)
	{
		if (!Conveyor[i])
		{
			On = false;
			UE_LOG(LogTemp, Error, TEXT("Can't create Belt. YOU ARE MISSING A SEGMENT IN THE CONVEYOR BELT: %s at index: %d"), *this->GetActorNameOrLabel(), i);
			return;
		}
		Conveyor[i]->Belt = this;
		Conveyor[i]->IndexInConveyorBelt=i;
	}

	if (!BoxSpawner)
	{
		UE_LOG(LogTemp, Error, TEXT("Conveyor belt is missing a BoxSpawner. YOU HAVE FORGOTTEN TO PUT A BOXSPAWNER ON CONVEYOR BELT: %s "), *this->GetActorNameOrLabel());
	}

	
	//sätter DistBetweenItems till längden av ett conveyor belt segment 
	//delat på hur många items som ska få plats där
	if (Conveyor.Num() > 0 && Conveyor[0] != nullptr)
	{
		FVector SegmentOrigin;
		FVector SegmentBoxExtent;
		Conveyor[0]->GetActorBounds(false, SegmentOrigin, SegmentBoxExtent);
		DistBetweenItems = (2*SegmentBoxExtent.Y)/ItemsPerSegment;
		
		//UE_LOG(LogTemp, Warning, TEXT("MaxItems BEING SET "));
	}
	PopulateTravelPath();
}

// Called every frame
void AConveyorBelt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (On)
	{
		//Move();
		//MoveRevolvingArray();
		MoveRevolvingArraySplinePath();
	}
	
	if (Remove)
	{
		DropItem(GetItemIndexFromSegmentIndex(RemoveAtIndex));
		Remove = false;
	}
	
	if (AddItem)
	{
		ReceiveItem(nullptr,GetItemIndexFromSegmentIndex(AddAtIndex)); 
		AddItem = false;
	}
}

void AConveyorBelt::SpawnItem(AItem* Item)
{
	AItem* AI = Item;
	if (Item == nullptr)
	{
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		AI = GetWorld()->SpawnActor<AItem>(BoxToSpawn, GetActorLocation(), Rotation);
		AI->Conveyor = this; //gör så item pekar på denna conveyor
		AI->SetPhysics(false);
	}

	ReceiveItem(AI, CurrentFirstIndex);
}

void AConveyorBelt::DropItem(AItem* Item)
{
	DropItem(GetItemIndex(Item));
}

void AConveyorBelt::DropItem(int Index)
{
	AItem* Item = Items[Index];
	if (Item == nullptr)
		return;
	//UE_LOG(LogTemp, Warning, TEXT("Removing %s at index %d"), *Item -> GetName(), Index);
	Item->SetPhysics(true);
	Item ->Conveyor = nullptr; //sluta peka på denna conveyor
	Item = nullptr;
	Items[Index] = nullptr;
	//borde också ge Item lite leftover velocity att fortsätta röra sig... så att det ser bättre ut
}

void AConveyorBelt::ReceiveItem(AItem* Item)
{
	ReceiveItem(Item,GetSegmentIndexFromItemIndex(GetItemIndex(Item)));
}

void AConveyorBelt::ReceiveItem(AItem* Item, AConveyorSegment* Segment)
{
	ReceiveItem(Item,GetItemIndexFromSegment(Segment));
}

void AConveyorBelt::ReceiveItem(AItem* Item, int Index)
{
	if (Items[Index] != nullptr)
		return;
	
	if (Item == nullptr)
	{
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		AItem* AI = GetWorld()->SpawnActor<AItem>(BoxToSpawn, GetActorLocation(), Rotation);
		Item = AI;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Recieving %s at index %d"), *Item -> GetName(), Index);
	Item->Conveyor = this; 
	Item->SetPhysics(false);
	Items[Index] = Item;
}

//V1 OLD
void AConveyorBelt::Move()
{
	//stäng av om det inte finns några items
	ShouldTurnOff();
	
	AConveyorSegment* Segment;
	for (int i = 0; i < Items.Num(); i++)
	{
		if (i+ CurrentSegment < Conveyor.Num())
		{
			Segment = Conveyor[i+ CurrentSegment];
		}
		else
		{
			Segment = Conveyor[Conveyor.Num()-1];
		}
		
		//UE_LOG(LogTemp, Display, TEXT("Current First Item: %d Current item: %d  Current segment: %d "), CurrentFirstIndex, i, i+CurrentSegment);
		
		AItem* Item = Items[i];
		int SDir = Segment->Direction;
		FVector NewLocation = Speed * Segment->GetActorForwardVector() * SDir;
		if (Items[i] == nullptr) //gå till nästa plats om det inte finns något item på denna plats
		{
			OldCurrDistMoved += abs(NewLocation.X);
			continue;
		}
		UE_LOG(LogTemp, Display, TEXT("ITEM WAS NOT NULL"));
		
		Item->SetActorLocation(Item -> GetActorLocation() +NewLocation);
		//UE_LOG(LogTemp, Display, TEXT("Forward Vector X %f  "), Segment->GetActorForwardVector().X);
		//UE_LOG(LogTemp, Display, TEXT("Forward Vector Y %f  "), Segment->GetActorForwardVector().Y);
		
		//håll koll på dist moved
		if (i == 0)
		{
			UE_LOG(LogTemp, Display, TEXT("i = %d Current dist moved: %f  "), i, OldCurrDistMoved);
			UE_LOG(LogTemp, Display, TEXT("current segment är: %d  "), i+ CurrentSegment);
			//lägg till distans på CurrDistMoved
			if (abs(Segment->GetActorForwardVector().X) > abs(Segment->GetActorForwardVector().Y))
			{
				UE_LOG(LogTemp, Display, TEXT("Adding dist on X, because X is: %f  "), Segment->GetActorForwardVector().X);
				OldCurrDistMoved += abs(NewLocation.X);
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("Adding dist on Y, because Y is: %f  "), Segment->GetActorForwardVector().Y);
				OldCurrDistMoved += abs(NewLocation.Y);
			}
		
			//om man har rört sig så långt
			if (OldCurrDistMoved >= DistBetweenItems)
			{
				OldCurrDistMoved = 0;
				CurrentSegment += 1;
				if (CurrentSegment >= Conveyor.Num())
					CurrentSegment = 0;
			}
		}
		
		//om man har rört sig så långt
		if (OldCurrDistMoved >= DistBetweenItems)
		{
			//Kolla på sista objektet om det ska falla av!
			if (i == Items.Num() - 1 && CurrentSegment >= Conveyor.Num() - 1)
			{
				DropItem(Item);
				//istället för att droppa här lägg in i array med stuff att droppa efter for loopen??
				//eller byta till array som inte tar bort utan bara byter pointer??
			}
		}
	}
}

//V2 OLD
void AConveyorBelt::MoveRevolvingArray()
{
	//stäng av om det inte finns några items
	ShouldTurnOff();
	
	for (int i = 0; i < Items.Num(); i++)
	{

		AConveyorSegment* Segment = Conveyor[0];
		int SegmentIndex = GetSegmentIndexFromItemIndex(i);
		if (SegmentIndex < Conveyor.Num())
			Segment = Conveyor[SegmentIndex];
		//UE_LOG(LogTemp, Display, TEXT("Current First Item: %d Current item: %d  Current segment: %d "), CurrentFirstIndex, i, i+CurrentSegment);
		
		AItem* Item = Items[i];
		if (Items[i] == nullptr) //gå till nästa plats om det inte finns något item på denna plats
			continue;
		//UE_LOG(LogTemp, Display, TEXT("ITEM WAS NOT NULL"));
		int SDir = Segment->Direction;
		FVector NewLocation = Speed * Segment->GetActorForwardVector() * SDir;
		
		
		Item->SetActorLocation(Item -> GetActorLocation() +NewLocation);
		//UE_LOG(LogTemp, Display, TEXT("Forward Vector X %f  "), Segment->GetActorForwardVector().X);
		//UE_LOG(LogTemp, Display, TEXT("Forward Vector Y %f  "), Segment->GetActorForwardVector().Y);
		
		//håll koll på dist moved
		if (i == 0)//CurrentFirstIndex)
		{
			//UE_LOG(LogTemp, Display, TEXT("i = %d Current dist moved: %f  "), i, CurrDistMoved);
			//lägg till distans på CurrDistMoved
			if (abs(Segment->GetActorForwardVector().X) > abs(Segment->GetActorForwardVector().Y))
			{
				OldCurrDistMoved += abs(NewLocation.X);
			}
			else
			{
				OldCurrDistMoved += abs(NewLocation.Y);
			}
		
			//om man har rört sig så långt
			if (OldCurrDistMoved >= DistBetweenItems)
			{
				//Kolla på sista objektet om det ska falla av!
				if (CurrentSegment >= Conveyor.Num() - 1)
				{
					DropItem(GetLastIndex());
				}
				//Increase current first index
				CurrentFirstIndex=CurrentFirstIndex-1; //OBS BEHÖVER GÖRA SÅ ATT DEN HÄR LOOPAR OM DEN ÄR VID MAX
				if (CurrentFirstIndex < 0) //BORDE BRYTA UT TILL EGEN LITEN METOD!!!
					CurrentFirstIndex = Items.Num() - 1;
				SpawnItem(nullptr);//JUST FOR FUN SHOULD NOT ACTUALLY BE HERE MAYBE??
				
				OldCurrDistMoved = 0;
				CurrentSegment += 1;
				if (CurrentSegment >= Conveyor.Num())
					CurrentSegment = 0;
			}
		}
	}
}

//V3 CURRENTLY USED MOVE METHOD!
void AConveyorBelt::MoveRevolvingArraySplinePath()
{
	try
	{
		//stäng av om det inte finns några items
		ShouldTurnOff();
	
		for (int i = 0; i < Items.Num(); i++)
		{
			//AConveyorSegment* Segment = Conveyor[0];
			int SegmentIndex = GetSegmentIndexFromItemIndex(i);
			//if (SegmentIndex < Conveyor.Num())
			//Segment = Conveyor[SegmentIndex];
		
			AItem* Item = Items[i];
		
			FVector NewLoc = Path->GetLocationAtSplineInputKey(SegmentIndex+MovedDelta,ESplineCoordinateSpace::World);
			if (IsValid(Item))
				Item->SetActorLocation(NewLoc);
		
			if (GetSegmentIndexFromItemIndex(i) == Conveyor.Num() - 1)
			{
				//Åk dubbelt så långt för pathen är hälfetn så lång 
				NewLoc = Path->GetLocationAtSplineInputKey(SegmentIndex+MovedDelta*2,ESplineCoordinateSpace::World);
				if (IsValid(Item))
					Item->SetActorLocation(NewLoc);
				if (MovedDelta*2>=1)
					DropItem(GetItemIndexFromSegmentIndex(Conveyor.Num()-1));
			}
		
			//Rotera objekten?
			//if (Item)
			//Item->SetActorRotation(Path->GetRotationAtSplineInputKey(SegmentIndex+MovedDelta,ESplineCoordinateSpace::World));
		
			//håll koll på dist moved (valt att kolla på en fast position)
			if (i == CurrentFirstIndex)
			{
				MovedDelta += Speed * GetWorld()->GetDeltaSeconds();
			
				if (MovedDelta>=1 )
				{
					//DropItem(GetItemIndexFromSegmentIndex(Conveyor.Num()-1));//droppar item på last segment
					UpdateCurrentFirstIndex();
				
					//CurrDistMoved = 0;
					UpdateCurrentSegment();
					if (BoxSpawner && SpawnItemsSelf)
					{
						AItem* ItemToSpawm = BoxSpawner->SpawnBox();
						//HERE YOU CAN TRIGGER SPAWNING THE NEXT ITEM!!!
						if (ItemToSpawm) SpawnItem(ItemToSpawm);
					}
					
					MovedDelta = 0;
				}
			}
		}
	}
	catch(...)
	{
		UE_LOG(LogTemp, Error, TEXT("Try Catch in ConveyorBelt.cpp MoveRevolvingArraySplinePath() FAILED!"));
	}
	
}

int AConveyorBelt::GetSegmentIndexFromItemIndex(int Index) 
{
	if (Index == CurrentFirstIndex)
		return 0;
	if (Index > CurrentFirstIndex)
	{
		return Index - CurrentFirstIndex;
	}
	//else
	int Shift = GetArrayShift();
	if (Shift == Items.Num())
	{
		return Index;
	}
	//else
	return Index + Shift;
	
	//return (Shift==Items.Num() ? Index + Shift : Index); //om Shift är 0 blir det interpreted som max av koden, så hantera det som 0
}


//VET INTE HUR KORREKT DEN HÄR ÄR!!!!!!!
int AConveyorBelt::GetItemIndexFromSegmentIndex(int Index)
{
	int Result = -1;
	if (CurrentFirstIndex == 0)
		Result = Index;
	if (Index < CurrentFirstIndex)
		Result = CurrentFirstIndex + Index;
	else if (Index >= CurrentFirstIndex)
		Result = Index + CurrentFirstIndex;
	if (Result >= Items.Num())
		Result -= Items.Num();
	return Result;
}

void AConveyorBelt::ShouldTurnOff()
{
	if (RunWhileEmpty)
		return;
	
	//stäng av om det inte finns några items
	if (Items.Num() == 0) //detta kommer inte hända när jag kör nullpointers och inte tar bort actual stuff!!
	{
		//UE_LOG(LogTemp, Warning, TEXT("Turning off"));
		On = false;
		return;
	}
	//version som fungerar när tomm = alla nullptrs
	//Check if all is empty, then turn off
	for (int i = 0; i < Items.Num(); i++)
	{
		if (Items[i] != nullptr)
			break;
		if (i +1 >= Items.Num())
			On = false;
		//UE_LOG(LogTemp, Warning, TEXT("Turning off"));
	}
}

void AConveyorBelt::PopulateTravelPath()
{
	

	//FSplinePoint myPoint = FSplinePoint();
	Path -> ClearSplinePoints();
	//för varje conveyor segment, lägg till som punkt på path
	for (int i = 0; i < Conveyor.Num(); i++)
	{
		Path->AddSplinePointAtIndex(Conveyor[i]->GetActorLocation() + PathOffset,i,ESplineCoordinateSpace::World);
		Path -> SetSplinePointType(i,ESplinePointType::Linear,true);
		//UE_LOG(LogTemp, Warning, TEXT("Added s at i: %d  with  vector:  X %f  Y  %f"),i, (Path->GetSplinePointAt(i,ESplineCoordinateSpace::World)).Position.X, (Path->GetSplinePointAt(i,ESplineCoordinateSpace::World)).Position.Y);
	}
	
	//lägg till punkt utanför arrayen för offset där de ska falla
	FVector SegmentOrigin;
	FVector SegmentBoxExtent;
	Conveyor[0]->GetActorBounds(false, SegmentOrigin, SegmentBoxExtent);
	//FVector OldForward = FVector(0,0,0) + (Conveyor[Conveyor.Num()-1] -> GetActorForwardVector());
	//FVector NewForward = FVector(0,0,0) + (Conveyor[Conveyor.Num()-1] -> GetForward());
	//FVector DirOffset1 = FVector(0,0,0) + (Conveyor[Conveyor.Num()-1] -> GetActorForwardVector() * (SegmentBoxExtent));
	FVector DirOffset = FVector(0,0,0) + (Conveyor[Conveyor.Num()-1] -> GetForward() * (SegmentBoxExtent.X-GuardRailLengthOffset));
	
	//UE_LOG(LogTemp, Warning, TEXT("Box extent X: %f Y:%f"), SegmentBoxExtent.X, SegmentBoxExtent.Y);
	
	//UE_LOG(LogTemp, Warning, TEXT("Last segment dir 1: x:%f y:%f z:%f   2: x:%f y:%f z:%f"), OldForward.X, OldForward.Y, OldForward.Z, NewForward.X, NewForward.Y, NewForward.Z);
	//UE_LOG(LogTemp, Warning, TEXT("Last segment dir 1: x:%f y:%f z:%f   2: x:%f y:%f z:%f"), DirOffset1.X, DirOffset1.Y, DirOffset1.Z, DirOffset.X, DirOffset.Y, DirOffset.Z);
	
	Path->AddSplinePointAtIndex(Conveyor[Conveyor.Num()-1]->GetActorLocation() + DirOffset  + PathOffset,Conveyor.Num(),ESplineCoordinateSpace::World);
	Path -> SetSplinePointType(Conveyor.Num(),ESplinePointType::Linear,true);
	
	//UE_LOG(LogTemp, Warning, TEXT("Path Created, coneyor num: %d"), Conveyor.Num());
	//UE_LOG(LogTemp, Warning, TEXT("Path:  %d"), Path->GetNumberOfSplinePoints ());
}

int AConveyorBelt::GetItemIndexFromSegment(AConveyorSegment* Segment)
{
	for (int i = 0; i < Conveyor.Num(); i++)
	{
		if (Conveyor[i] == Segment)
			return GetItemIndexFromSegmentIndex(i);
	}
	return -1;
}

//där -1 representerar att item inte hittades
int AConveyorBelt::GetItemIndex(AItem* Item)
{
	for (int i = 0; i < Items.Num(); i++)
	{
		if (Items[i] == nullptr)
			continue;
		if (Items[i] == Item)
		{
			return i;
		}
	}
	return -1;
}

int AConveyorBelt::GetLastIndex()
{
	int LastIndex;
	if (CurrentFirstIndex != 0)
		LastIndex = CurrentFirstIndex -1;
	else
		LastIndex = Items.Num() - 1;
	return LastIndex;
}

int AConveyorBelt::GetArrayShift()
{
	return abs(Items.Num() - CurrentFirstIndex);
}

void AConveyorBelt::UpdateCurrentSegment()
{
	CurrentSegment += 1;
	if (CurrentSegment >= Conveyor.Num())
		CurrentSegment = 0;
}

void AConveyorBelt::UpdateCurrentFirstIndex()
{
	//Move current first index
	CurrentFirstIndex=CurrentFirstIndex-1;
	if (CurrentFirstIndex < 0) 
		CurrentFirstIndex = Items.Num() - 1;
}

bool AConveyorBelt::HasItemInSegment(AConveyorSegment* Segment)
{
	AItem* Item = Items[GetItemIndexFromSegment(Segment)];
	if (Item != nullptr)
		return true;
	return false;
}

AItem* AConveyorBelt::GetItemFromSegment(AConveyorSegment* Segment)
{
	return Items[GetItemIndexFromSegment(Segment)];
}

