// Marcus hopefully approves of this.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "FMODAudioComponent.h"
#include "SongPlaylistAsset.h"
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
	UFUNCTION(BlueprintCallable)
	void TurnOff();
	UFUNCTION(BlueprintCallable)
	void TurnOn();
	UFUNCTION(BlueprintCallable)
	void UpdatePitch();
	
	UPROPERTY(BlueprintReadWrite)
	int TimeLeft;
	
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
	bool IsOn;
	
	UPROPERTY(EditAnywhere)
	bool TurnOnInBeginPlay;
	
	UPROPERTY(EditAnywhere)
	float Last30SecondsPitch = 1.25f;
	
	UPROPERTY(EditAnywhere)
	float Last10SecondsPitch = 1.5f;
	
	UPROPERTY(EditAnywhere)
	USongPlaylistAsset* Playlist; //playlist with all songs for this level
	TArray<int32> Copy; //used so all channels are played once before repeating
	UFMODAudioComponent* CurrentInstance; //current FMOD Event Instance playing
	
	void InitializeCopyArray(); //basically fylla på copy array med indexes
	
	int SpeedToSwitchChannel = 500;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			   FVector NormalImpulse,
			   const FHitResult& Hit);

};
