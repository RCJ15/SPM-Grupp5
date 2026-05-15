// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "ConveyorBelt.h"
#include "ConveyorBeltUpgraded.generated.h"


//class AConveyorSegment;

struct FObjectOnBelt
{
	AItem* Item;
	float MovedDelta = 0; //how far this has moved along the spline
	FVector Offset = FVector(0,0,0); //Offset to add to its position on the spline
	//FObjectOnBelt* Prev;
	FObjectOnBelt* Next = nullptr;
	
	FObjectOnBelt(AItem* I)
	{
		FObjectOnBelt(I,nullptr, 0, FVector(0,0,0));
	}
	
	FObjectOnBelt(AItem* I, FObjectOnBelt* N, float M, FVector O)
	{
		Item = I;
		Next = N;
		MovedDelta = M;
		Offset = O;
	}
};


UCLASS()
class SPMG5_API AConveyorBeltUpgraded : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, Category="Conveyor Settings")
	bool bIsOn;
	UPROPERTY(EditAnywhere, Category="Conveyor Settings")
	float Speed; //Antal lådor som spawnar per sekund ish (1 = en låda per sekund) (0,5 = en låda varannan sekund)
	
	FVector PathOffset = FVector(0,0,100);
	int GuardRailLengthOffset = 10; //Detta är extra längden från guard rails som bör subtraheras
	UPROPERTY()
	USplineComponent* Path;
	
	UPROPERTY(EditAnywhere, Category="Conveyor Settings")
	TArray<AConveyorSegment*> Segments; //är alla segment i hela conveyorn
	
	UPROPERTY(EditAnywhere)
	ABoxSpawner* BoxSpawner;
	
	// Sets default values for this actor's properties
	AConveyorBeltUpgraded();
	
	void RemoveFromBelt(AItem* Item);
	void AddToBelt(AItem* Item);
	
	bool ItemIsAtEndOfBelt(AItem* Item);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	FObjectOnBelt* First = nullptr;
	
	void Move();
	FObjectOnBelt* FindObjectOnBelt(AItem* Item);
	
	void GenerateSpline();
	void RemoveFromBelt(FObjectOnBelt* Obj);
	
};
