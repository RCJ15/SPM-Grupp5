// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlowPipe.generated.h"

UCLASS()
class SPMG5_API ABlowPipe : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlowPipe();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle BlowLoopTimer;
	bool Blowing = false;
	
	void ChangeBlowState();
	void Blow(float DeltaTime);
	void BLowLoop(float LoopRate);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	FVector BlowBoxSize = FVector(120.0f, 50.0f, 50.0f);

	UPROPERTY(EditAnywhere)
	FVector Offcset = FVector(10.0f, 0.0f, 0.0f);
	
	UPROPERTY(EditAnywhere)
	float BlowRate = 0.01;
	
	UPROPERTY(EditAnywhere)
	float ForceMultiplier = 500;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BoxMesh;
	
};
