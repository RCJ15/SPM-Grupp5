// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
//#include "BoxDestroyer.h"
#include "AudioEnums.h"
#include "FMODEvent.h"
#include "NiagaraSystem.h"
#include "Item.generated.h"

class AConveyorBelt; // forward-declaration

UENUM(BlueprintType)
enum class BoxAddress : uint8
{
	CIRCLE,
	SQUARE,
	TRIANGLE,
	TOTAL_COUNT = 3
};

UCLASS()
class AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();
	
	UPROPERTY(VisibleAnywhere)
	AConveyorBelt* Conveyor = nullptr; //pekar til conveyor om den är attached

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="Audio")
	EAudioItemType AudioType;
	
	UPROPERTY(EditAnywhere, Category="Audio")
	UFMODEvent* CollisionSFX;
	
	UPROPERTY(EditAnywhere, Category="Audio")
	float SFXNormalImpulseMin = 50.0f;
	
	UPROPERTY(EditAnywhere, Category="Audio")
	float SFXNormalImpulseMax = 1000.0f;
	
	UPROPERTY(EditAnywhere, Category="Audio")
	float SFXCollisionVolumeMin = 0.0f;
	
	UPROPERTY(EditAnywhere, Category="Audio")
	float SFXCollisionVolumeMax = 1.0f;
	
	UPROPERTY(EditAnywhere, Category="Audio")
	UFMODEvent* DestroySFX;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	BoxAddress Address = BoxAddress::SQUARE;
	
	UPROPERTY(EditAnywhere)
	bool IsFragile;

	UPROPERTY(EditAnywhere)
	bool IsLarge;
	
	UPROPERTY(EditAnywhere)
	bool IsSuspicious;
	
	UPROPERTY(EditAnywhere)
	bool IsScanned = false;
	
	UPROPERTY(EditAnywhere)
	int SmallBoxPoints = 10;
	
	UPROPERTY(EditAnywhere)
	int LargeBoxPoints = 20;
	
	UPROPERTY(EditAnywhere)
	int FragileBoxPoints = 5;
	
	UPROPERTY(EditAnywhere)
	int WrongBoxPoints = -15;
	
	int Points;
	
	UPrimitiveComponent* PrimComp;
	AActor* MostRecentHolder;
	
	void CalculateIfBreakIfFragile();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void SetPoints();
	
	UFUNCTION(BlueprintCallable)
	void SetNegativePoints();
	
	void SetPhysics(bool SetTo);
	void ResetVelocity(){PrimComp->SetPhysicsLinearVelocity(FVector(0,0,0));}
	void AddVelocity(int Force){PrimComp->SetPhysicsLinearVelocity(GetActorForwardVector() * Force + GetActorUpVector() * Force/2);}
	
	UFUNCTION(BlueprintCallable)
	virtual void Disintegrate();
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			   FVector NormalImpulse,
			   const FHitResult& Hit);
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//USceneComponent* BaseStaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* BaseMesh;
	
	void AddImpulse(FVector Point, float Strength);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetAudioType();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIsFragile();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIsLarge();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIsSuspicious();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIsScanned();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetPoints();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	BoxAddress GetAddress();
	
	UFUNCTION(BlueprintCallable)
	void SetMostRecentHolder(AActor* holder);
	
	UPROPERTY(EditAnywhere)
	int MaxSpeedIfFragile = 500;
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* FragileBreakParticles;
	
	void SetIsLarge(bool SetTo);
	void SetIsFragile(bool SetTo);
	void SetIsSuspicious(bool SetTo);
	void SetAddress(BoxAddress NewAddress);
	
	UFUNCTION(BlueprintCallable)
	void SetIsScanned(bool SetTo);
	
	
	//FULT!!!!! TA BORT EFTER SPELTEST!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	UPROPERTY(EditAnywhere)
	bool PlaySound = true;
	
	void SetPlaySound(bool SetTo){PlaySound = SetTo;}
	
	
};
