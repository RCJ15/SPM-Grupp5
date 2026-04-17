// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "BoxDestroyer.h"
#include "Item.generated.h"

UCLASS()
class SPMG5_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	bool IsFragile;

	UPROPERTY(EditAnywhere)
	bool IsLarge;
	
	UPROPERTY(EditAnywhere)
	bool IsDangerous;
	
	UPrimitiveComponent* PrimComp;
	AActor* MostRecentHolder;
	
	void CalculateIfBreakIfFragile();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void SetPhysics(bool SetTo);
	void ResetVelocity(){PrimComp->SetPhysicsLinearVelocity(FVector(0,0,0));}
	void AddVelocity(int x, int y, int z){PrimComp->AddForce(FVector(x,y,z));}
	
	virtual void Disintegrate();
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			   FVector NormalImpulse,
			   const FHitResult& Hit);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* BaseMesh;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIsFragile();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIsLarge();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIsDangerous();
	
	UFUNCTION(BlueprintCallable)
	void SetMostRecentHolder(AActor* holder);
	
	UPROPERTY(EditAnywhere)
	int MaxSpeedIfFragile = 500;
	
	void SetIsLarge(bool SetTo);
	void SetIsFragile(bool SetTo);
	void SetIsDangerous(bool SetTo);
};
