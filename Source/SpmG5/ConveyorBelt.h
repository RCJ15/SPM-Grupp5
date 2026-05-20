// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
//#include "ConveyorSegment.h"
#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "BoxSpawner.h"
#include "Components/ArrowComponent.h"
#include "ConveyorBelt.generated.h"

UCLASS()
class SPMG5_API AConveyorSegment : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;
public:	
	// Sets default values for this actor's properties
	AConveyorSegment();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UArrowComponent* Arrow;
	int Direction = 1; //1 är fram, -1 är bakåt
	int IndexInConveyorBelt = -1;
	
	UPROPERTY()
	AConveyorBelt* Belt = nullptr;
	
	UPROPERTY()
	AConveyorBeltUpgraded* BeltUpgraded = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FVector GetForward();
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			   FVector NormalImpulse,
			   const FHitResult& Hit);

};

UCLASS()
class SPMG5_API AConveyorBelt : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConveyorBelt();
	void DropItem(AItem*); //borde ha param som vad som ska droppas
	void ReceiveItem(AItem*); //borde ha param på vad som ska läggas in
	void ReceiveItem(AItem*, AConveyorSegment*);
	bool HasItemInSegment(AConveyorSegment*);
	AItem* GetItemFromSegment(AConveyorSegment*);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Category="Conveyor Settings")
	bool On;
	UPROPERTY(EditAnywhere, Category="Conveyor Settings")
	bool RunWhileEmpty; //if the conveyor should still be able to be on while it has no items.
	UPROPERTY(EditAnywhere, Category="Conveyor Settings", BlueprintReadWrite)
	bool SpawnItemsSelf = true;
	UPROPERTY(EditAnywhere, Category="Conveyor Settings")
	float Speed = 0.5; //Antal lådor som spawnar per sekund ish (1 = en låda per sekund) (0,5 = en låda varannan sekund)
	UPROPERTY(EditAnywhere, Category="Conveyor Settings")
	float RunOffset = 25; //offset for how far the items should continue to move after leaving a segment/the last segment

	int MaxItems = 10; //Max antal items som får plats på conveyor beltet på en och samma gång
	UPROPERTY(EditAnywhere)
	int ItemsPerSegment = 1; //hur många items som ska få plats på ett segment
	UPROPERTY(VisibleAnywhere)
	float DistBetweenItems = 100; //offset för hur långt mellanrum det ska vara mellan items
	UPROPERTY(VisibleAnywhere)
	float OldCurrDistMoved; //håller koll på hur långt obj rört sig
	
	UPROPERTY(VisibleAnywhere)
	float MovedDelta; //håller koll på vart objekt ska vara samt när "shifts" ska göras i arrayen
	
	int CurrentSegment; //använder för att hålla reda på vilka segment som hör ihop med vilka items
	
	int CurrentFirstIndex = 0; //börjar på 0
	
	UPROPERTY(EditAnywhere, Category="Function tests | Remove ")
	bool Remove = false;
	UPROPERTY(EditAnywhere, Category="Function tests | Remove")
	int RemoveAtIndex = 0;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AItem> BoxToSpawn;
	
	UPROPERTY(EditAnywhere)
	ABoxSpawner* BoxSpawner;
	UPROPERTY(EditAnywhere, Category="Function tests | Add")
	bool AddItem = false;
	UPROPERTY(EditAnywhere, Category="Function tests | Add")
	int AddAtIndex = 0;
	
	FVector PathOffset = FVector(0,0,100);
	int GuardRailLengthOffset = 10; //Detta är extra längden från guard rails som bör subtraheras
	UPROPERTY(EditAnywhere)
	USplineComponent* Path;
	
	TArray<AItem*> Items;

	UPROPERTY(EditAnywhere, Category="Conveyor Settings")
	TArray<AConveyorSegment*> Conveyor; //är alla segment i hela conveyorn
	
private:
	void SpawnItem(AItem*);
	
	void DropItem(int);
	void ReceiveItem(AItem*, int); 
	void Move(); //version 1
	void MoveRevolvingArray(); //versionen där jag försökte rotera arrayen
	void MoveRevolvingArraySplinePath(); //-..- following a spline path instead
	
	//helper functions
	int GetSegmentIndexFromItemIndex(int Index); //räknar ut vilket segment som correspondar till ett specifikt index
	int GetItemIndexFromSegmentIndex(int Index);
	void ShouldTurnOff();
	void PopulateTravelPath();
	int GetItemIndexFromSegment(AConveyorSegment*);
	int GetItemIndex(AItem*);
	int GetLastIndex();
	int GetArrayShift();
	void UpdateCurrentSegment();
	void UpdateCurrentFirstIndex();

	
};
