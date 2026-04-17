// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Item.h"
#include "Logging/LogMacros.h"
#include "SpmG5Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class ASpmG5Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	/*
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera #1#
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	*/
	
protected:

	UPROPERTY(EditAnywhere)
	USceneComponent* HoldingLocation;
	
	UPROPERTY(EditAnywhere)
	FVector PickUpBoxSize = FVector(80.0f, 50.0f, 120.0f);
	UPROPERTY(EditAnywhere)
	float Offset = 10.0f;

	FHitResult HitResult;
	AItem* HeldItem = nullptr;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	//UPROPERTY(EditAnywhere, Category="Input")
	//UInputAction* LookAction;

	/** Mouse Look Input Action */
	//UPROPERTY(EditAnywhere, Category="Input")
	//UInputAction* MouseLookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PickupAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* DropAction;

public:

	/** Constructor */
	ASpmG5Character();	

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	virtual void Tick(float DeltaTime) override;
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	/*
	void Look(const FInputActionValue& Value);
	*/

	void Pickup(const FInputActionValue& Value);

	void Drop(const FInputActionValue& Value);

public:


	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	/*UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);*/

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

public:

	/** Returns CameraBoom subobject **/
	/*FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject *#1#
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }*/
};

