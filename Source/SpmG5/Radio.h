// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "Sound/AmbientSound.h"
#include "Radio.generated.h"

UCLASS()
class SPMG5_API ARadio : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARadio();
	UFUNCTION(BlueprintCallable)
	void SwitchChannel();
	void TurnOff();
	void TurnOn();
	
	UPROPERTY(EditAnywhere)
	bool bSwitchChannel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* BaseMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//void OnInteract(ASpmG5Character* InteractingPlayer);
private:
	UPROPERTY(EditAnywhere)
	TArray<USoundWave*> SoundWave; //array with music that can be played
	TArray<int32> Copy; //used so all channels are played once before repeating
	AAmbientSound* AmbientSound; //the music playing
	
	void InitializeCopyArray(); //basically fylla på copy array med indexes
	
	int SpeedToSwitchChannel = 500;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			   FVector NormalImpulse,
			   const FHitResult& Hit);

};
