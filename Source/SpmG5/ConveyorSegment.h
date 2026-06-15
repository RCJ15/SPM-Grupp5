#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "ConveyorSegment.generated.h"

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