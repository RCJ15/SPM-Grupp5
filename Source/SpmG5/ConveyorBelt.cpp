// Fill out your copyright notice in the Description page of Project Settings.


#include "ConveyorBelt.h"

#include "IDetailTreeNode.h"
#include "MeshUtilitiesCommon.h"
#include "SNegativeActionButton.h"
#include "Components/SplineComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"


// Sets default values
AConveyorBelt::AConveyorBelt()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//MaxItems = Conveyor.Num(); //This does not work
	Items.SetNum(MaxItems) ;
	//subobject needs to be created here
	Path =  CreateDefaultSubobject<USplineComponent>("Path",true);
	
	
}

// Called when the game starts or when spawned
void AConveyorBelt::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("Distance Between Items shall be: %f  "), DistBetweenItems);
	
	//fyller Items array med nullpointers
	for( int32 i = 0; i <MaxItems; i++ )
	{
		Items[i] = nullptr;
	}
	UE_LOG(LogTemp, Display, TEXT("MaxItems: %d  "), MaxItems);
	
	for (int i = 0; i < Conveyor.Num(); i++)
	{
		Conveyor[i]->Belt = this;
		Conveyor[i]->IndexInConveyorBelt=i;
	}
	
	
	//sätter DistBetweenItems till längden av ett conveyor belt segment 
	//delat på hur många items som ska få plats där
	if (Conveyor.Num() > 0 && Conveyor[0] != nullptr)
	{
		FVector SegmentOrigin;
		FVector SegmentBoxExtent;
		Conveyor[0]->GetActorBounds(false, SegmentOrigin, SegmentBoxExtent);
		DistBetweenItems = (2*SegmentBoxExtent.X)/ItemsPerSegment;
		
		UE_LOG(LogTemp, Warning, TEXT("MaxItems BEING SET "));
		//This is not working ::::
		//MaxItems = Conveyor.Num() * ItemsPerSegment; //antal segment * antal items möjliga per segment ( + 1 ????)
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
		ReceiveItem(nullptr,GetItemIndexFromSegmentIndex(AddAtIndex)); //borde göras om så att den
		AddItem = false;
	}
}

void AConveyorBelt::SpawnItem(AItem* Item)
{
	AItem* AI = Item;
	if (Item == nullptr)
	{
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		//FActorSpawnParameters SpawnInfo;
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
	UE_LOG(LogTemp, Warning, TEXT("Removing %s at index %d"), *Item -> GetName(), Index);
	//Items[Index]->SetActorEnableCollision(true);
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
	//ReceiveItem(Item,1);
	ReceiveItem(Item,GetItemIndexFromSegment(Segment));
}

//ska detta vara Segment index eller item index, item index va?
void AConveyorBelt::ReceiveItem(AItem* Item, int Index)
{
	//Items.Add(Item);
	//kolla om index är empty
	if (Items[Index] != nullptr)
		return;
	
	if (Item == nullptr)
	{
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		//FActorSpawnParameters SpawnInfo;
		AItem* AI = GetWorld()->SpawnActor<AItem>(BoxToSpawn, GetActorLocation(), Rotation);
		Item = AI;
	}
	UE_LOG(LogTemp, Warning, TEXT("Recieving %s at index %d"), *Item -> GetName(), Index);
	Item->Conveyor = this; //gör så item pekar på denna conveyor
	Item->SetPhysics(false);
	Items[Index] = Item;
}

//Ordo 2N komplexitet ungefär
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
				//DropItem(i); //
				DropItem(Item);
				//istället för att droppa här lägg in i array med stuff att droppa efter for loopen??
				//eller byta till array som inte tar bort utan bara byter pointer??
			}
		}
	}
	
	
	/*
	for (AItem* I : Items)
	{
		UE_LOG(LogTemp, Display, TEXT("Move Item %s"), *I-> GetName());
		FVector NewLocation = I -> GetActorLocation();
		UE_LOG(LogTemp, Warning, TEXT("Pos x: %f  y: %f  z: %f"), NewLocation.X,NewLocation.Y,NewLocation.Z);
		NewLocation.X += 1;
		I -> SetActorLocation(NewLocation);
	}*/
}

//NOT USED OR FUNCTIONING RIGHT NOW!!!!!!!!!!
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
		UE_LOG(LogTemp, Display, TEXT("ITEM WAS NOT NULL"));
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
				//UE_LOG(LogTemp, Display, TEXT("Adding dist on X, because X is: %f  "), Segment->GetActorForwardVector().X);
				OldCurrDistMoved += abs(NewLocation.X);
			}
			else
			{
				//UE_LOG(LogTemp, Display, TEXT("Adding dist on Y, because Y is: %f  "), Segment->GetActorForwardVector().Y);
				OldCurrDistMoved += abs(NewLocation.Y);
			}
		
			//om man har rört sig så långt
			if (OldCurrDistMoved >= DistBetweenItems)
			{
				//Kolla på sista objektet om det ska falla av!
				if (CurrentSegment >= Conveyor.Num() - 1)//true)//i == Items.Num() - 1 && CurrentSegment >= Conveyor.Num() - 1)
				{
					//Drop last item, vilket kommer vara -1 från första
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

void AConveyorBelt::MoveRevolvingArraySplinePath()
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
		//if (Item == nullptr) //gå till nästa plats om det inte finns något item på denna plats
			//continue;
		//UE_LOG(LogTemp, Display, TEXT("ITEM WAS NOT NULL"));
		//int SDir = Segment->Direction;
		//FVector NewLocation = Speed * Segment->GetActorForwardVector() * SDir;
		//CurrDistMoved += Speed;
		//FVector NewLoc = Path->FindLocationClosestToWorldLocation(Item -> GetActorLocation() + Speed,ESplineCoordinateSpace::World);
		FVector NewLoc = Path->GetLocationAtSplineInputKey(SegmentIndex+MovedDelta,ESplineCoordinateSpace::World);
		if (Item)
			Item->SetActorLocation(NewLoc);
		//if (Item)
			//Item->SetActorRotation(Path->GetRotationAtSplineInputKey(SegmentIndex+MovedDelta,ESplineCoordinateSpace::World));
		
		//den flippar efter den bör flippa.. så like yeah
		/*if (MovedDelta >= 0.5 && i-1 >= 0 && Segment->GetActorForwardVector() != Conveyor[CalculateCurrentSegment(i-1)]->GetActorForwardVector()) //(MovedDelta >= 0.5 && i+1 < Conveyor.Num() && Conveyor[i]->GetActorForwardVector() != Conveyor[i+1]->GetActorForwardVector()) //börja rotera
		{
			//UE_LOG(LogTemp, Warning, TEXT("SHOULD ROTATE"));
			Item->SetActorRotation(Path->GetRotationAtSplineInputKey(SegmentIndex+MovedDelta,ESplineCoordinateSpace::World));
			//FRotator NewRot = Segment->GetActorRotation() * -MovedDelta;
			//Item->SetActorRotation(NewRot);
		}*/
		
		
		//Item->CurrDistMoved += Speed;
		
		
		//håll koll på dist moved (valt att kolla på en fast position)
		if (i == CurrentFirstIndex)//GetLastIndex())//CurrentFirstIndex)
		{
			MovedDelta += (Speed/100);
			//om man har rört sig så långt
			//if (Item->GetActorLocation() == Path->GetLocationAtSplineInputKey(i, ESplineCoordinateSpace::World))
			//if it has reached or passed the next inputkey point
			//float InputKey = Path->FindInputKeyClosestToWorldLocation(Item->GetActorLocation());
			if (MovedDelta>=1 )//Item->CurrDistMoved >= i+1) //kan inte ha i här för i är alltid 0
			{
				
				UE_LOG(LogTemp, Warning, TEXT("Has passed Input Key Loc! key: %f  i: %d"), MovedDelta, SegmentIndex+1);
				
				DropItem(GetItemIndexFromSegmentIndex(Conveyor.Num()-1));//droppar item på last segment
				//DropItem(GetLastIndex());
				UpdateCurrentFirstIndex();
				
				//CurrDistMoved = 0;
				UpdateCurrentSegment();
				
				
				//HERE YOU CAN TRIGGER SPAWNING THE NEXT ITEM!!!
				//SpawnItem(nullptr);//JUST FOR FUN SHOULD NOT ACTUALLY BE HERE MAYBE??
				
				SpawnItem(BoxSpawner -> SpawnBox(1));
				MovedDelta = 0;
			}
			/*if (CurrDistMoved >= DistBetweenItems)
			{
				UE_LOG(LogTemp, Warning, TEXT("CurrDistMoved >= DistBetweenItems"));
				UpdateCurrentFirstIndex();
				CurrDistMoved = 0;
				UpdateCurrentSegment();
			}*/
		}
		
		//Kolla om objekt har nått slutet av the path
		/*if (Item->CurrDistMoved>= Conveyor.Num()-1  )//Path->GetLocationAtSplineInputKey(i, ESplineCoordinateSpace::World) == Item->GetActorLocation())
		{
			DropItem(GetLastIndex());
			
			//HERE YOU CAN TRIGGER SPAWNING THE NEXT ITEM!!!
			SpawnItem(nullptr);//JUST FOR FUN SHOULD NOT ACTUALLY BE HERE MAYBE??
		}*/
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
	/*
	if (CurrentFirstIndex == 0 || GetArrayShift() == Items.Num())
		return Index;
	if (Index > CurrentFirstIndex)
		return Index - GetArrayShift();
	if (Index < CurrentFirstIndex)
		if (Index + CurrentFirstIndex >= Items.Num())
			return Index + CurrentFirstIndex - Items.Num();
		return Index + CurrentFirstIndex;*/
}

void AConveyorBelt::ShouldTurnOff()
{
	if (RunWhileEmpty)
		return;
	
	//stäng av om det inte finns några items
	if (Items.Num() == 0) //detta kommer inte hända när jag kör nullpointers och inte tar bort actual stuff!!
	{
		UE_LOG(LogTemp, Warning, TEXT("Turning off"));
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
		UE_LOG(LogTemp, Warning, TEXT("Turning off"));
	}
}

void AConveyorBelt::PopulateTravelPath()
{
	

	//FSplinePoint myPoint = FSplinePoint();
	Path -> ClearSplinePoints();
	//för varje conveyor segment, lägg till som punkt på path
	for (int i = 0; i < Conveyor.Num(); i++)
	{
		//myPoint.Position = FVector(0.0f,0.0f,0.0f);
		//myPoint.Position = Conveyor[i]->GetActorLocation();
		//myPoint.Position = Conveyor[i]->GetActorLocation() + PathOffset;
		//myPoint.Rotation = FRotator(0.0f,0.0f,0.0f);
		//myPoint.Rotation = Conveyor[i]->GetActorRotation();
		//myPoint.Scale = FVector(0.0f,0.0f,0.0f);
		//myPoint.Scale = Conveyor[i]->GetActorScale();

		//Path->AddPoint(myPoint,true);
		Path->AddSplinePointAtIndex(Conveyor[i]->GetActorLocation() + PathOffset,i,ESplineCoordinateSpace::World);
		Path -> SetSplinePointType(i,ESplinePointType::Linear,true);
		//Path -> AddSplinePoint(myPoint,i,ESplineCoordinateSpace::World);
		//Path -> AddPoint(myPoint,true);
		//Path -> AddSplineWorldPoint(Conveyor[i]->GetActorLocation() + PathOffset);
		//FSplinePoint P = Path->GetSplinePointAt(i,ESplineCoordinateSpace::World);
		//P.Rotation = Conveyor[i]->GetActorRotation();
		UE_LOG(LogTemp, Warning, TEXT("Added s at i: %d  with  vector:  X %f  Y  %f"),i, (Path->GetSplinePointAt(i,ESplineCoordinateSpace::World)).Position.X, (Path->GetSplinePointAt(i,ESplineCoordinateSpace::World)).Position.Y);
	}
	
	
	
	//lägg till punkt utanför arrayen för offset där de ska falla
	FVector SegmentOrigin;
	FVector SegmentBoxExtent;
	Conveyor[0]->GetActorBounds(false, SegmentOrigin, SegmentBoxExtent);
	FVector DirOffset = FVector(0,0,0) + (Conveyor[Conveyor.Num()-1] -> GetActorForwardVector() * (SegmentBoxExtent*2));
	
	Path->AddSplinePointAtIndex(Conveyor[Conveyor.Num()-1]->GetActorLocation() + DirOffset + PathOffset,Conveyor.Num(),ESplineCoordinateSpace::World);
	Path -> SetSplinePointType(Conveyor.Num(),ESplinePointType::Linear,true);
	
	
	//Path->RemoveSplinePoint(11);
	//Path->RemoveSplinePoint(10);
	
	/*for (FSplinePoint Point : Path)
	{
		UE_LOG(LogTemp, Warning, TEXT("Point:  X %f  Y  %f  Z %f"), Point.Position.X,Point.Position.Y, Point.Position.Z);
	}*/
	UE_LOG(LogTemp, Warning, TEXT("Path Created, coneyor num: %d"), Conveyor.Num());
	UE_LOG(LogTemp, Warning, TEXT("Path:  %d"), Path->GetNumberOfSplinePoints ());
	
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

