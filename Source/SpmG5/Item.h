// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
//#include "BoxDestroyer.h"
#include "Item.generated.h"

class AConveyorBelt; // forward-declaration

UCLASS()
class AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();
	
	UPROPERTY(VisibleAnywhere)
	AConveyorBelt* Conveyor = nullptr; //pekar til conveyor om den är attatched

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	bool IsFragile;

	UPROPERTY(EditAnywhere)
	bool IsLarge;
	
	UPROPERTY(EditAnywhere)
	bool IsDangerous;
	
	UPROPERTY(EditAnywhere)
	int SmallBoxPoints = 10;
	
	UPROPERTY(EditAnywhere)
	int LargeBoxPoints = 20;
	
	UPROPERTY(EditAnywhere)
	int FragileBoxPoints = 5;
	
	int Points;
	
	UPrimitiveComponent* PrimComp;
	AActor* MostRecentHolder;
	
	void CalculateIfBreakIfFragile();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void SetPoints();
	void SetPhysics(bool SetTo);
	void ResetVelocity(){PrimComp->SetPhysicsLinearVelocity(FVector(0,0,0));}
	void AddVelocity(int Force){PrimComp->SetPhysicsLinearVelocity(GetActorForwardVector() * Force + GetActorUpVector() * Force/2);}
	
	UFUNCTION(BlueprintCallable)
	virtual void Disintegrate();
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			   FVector NormalImpulse,
			   const FHitResult& Hit);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* BaseStaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* BaseMesh;
	
	void AddImpulse(FVector Point, float Strength);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIsFragile();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIsLarge();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIsDangerous();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetPoints();
	
	UFUNCTION(BlueprintCallable)
	void SetMostRecentHolder(AActor* holder);
	
	UPROPERTY(EditAnywhere)
	int MaxSpeedIfFragile = 500;
	
	void SetIsLarge(bool SetTo);
	void SetIsFragile(bool SetTo);
	void SetIsDangerous(bool SetTo);
};
