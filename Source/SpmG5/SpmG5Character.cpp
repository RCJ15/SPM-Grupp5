// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpmG5Character.h"
#include "Item.h"
#include "Engine/LocalPlayer.h"
//#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
//#include "GameFramework/SpringArmComponent.h"
#include "ConveyorBelt.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "INodeAndChannelMappings.h"
#include "InputActionValue.h"
#include "InteractiveToolActionSet.h"
#include "SpmG5.h"
#include "ConveyorSegment.h"
#include "StateTreeTypes.h"

ASpmG5Character::ASpmG5Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	/*CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
													
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	FollowCamera->bUsePawnControlRotation = false;*/
	HoldingLocation = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HoldingLocation"));
	HoldingLocation->SetupAttachment(RootComponent);
	

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ASpmG5Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASpmG5Character::Move);
		//EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ASpmG5Character::Look);

		// Looking
		//EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASpmG5Character::Look);

		// Pickup and Drop
		EnhancedInputComponent->BindAction(PickupOrDropAction, ETriggerEvent::Started, this, &ASpmG5Character::PickupAndDrop);
		EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Started, this, &ASpmG5Character::Throw);
		
		//interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ASpmG5Character::Interact);
	}
	else
	{
		UE_LOG(LogSpmG5, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ASpmG5Character::PickupAndDrop(const FInputActionValue& Value)
{	
	float Distance = 5.0f;
	FVector Location = HoldingLocation->GetComponentLocation();	
	FVector End = Location + GetActorForwardVector() * Distance;
	FCollisionShape Box = FCollisionShape::MakeBox(PickUpBoxSize);
	FQuat Rotation = GetActorRotation().Quaternion();
		
	GetWorld()->SweepSingleByChannel(HitResultBox,Location, End, Rotation, ECC_GameTraceChannel1,Box);
	GetWorld()->SweepSingleByChannel(HitResultConvayer,Location, End, Rotation, ECC_GameTraceChannel2,Box);
	
	if (!HeldItem)//Pickup
		Pickup();	
	
	else //Drop				
		Drop();	
}

void ASpmG5Character::Pickup()
{
	if (HitResultBox.GetActor() && HitResultBox.GetComponent() && Cast<AItem>(HitResultBox.GetActor()))
	{
		//Fult men vet inte hur man kan göra det på bättre sätt
		HeldItem = Cast<AItem>(HitResultBox.GetActor());
		HasItem = true;
		AttatchPackage();			
	}
	else //KANSKE VILL ÄNDRA SÅ MAN KOLLAR PÅ ITEM ISTÄLLET FÖR SEGMENT			
		InteractWithCoinvayer();	
}

void ASpmG5Character::AttatchPackage()
{
	HeldItem->SetPhysics(false);
	HeldItem->ResetVelocity();
	HeldItem->SetActorRelativeLocation(HoldingLocation->GetComponentLocation());
	HeldItem->SetActorRelativeRotation(FRotator(0,0,0));
	HeldItem->SetMostRecentHolder(this);
}

void ASpmG5Character::InteractWithCoinvayer()
{
	if (HitResultConvayer.GetActor() && Cast<AConveyorSegment>(HitResultConvayer.GetActor()))
	{
		AConveyorSegment* Segment = Cast<AConveyorSegment>(HitResultConvayer.GetActor());
		//kolla om segment är tomt
		if (AConveyorBelt* Belt = Segment->Belt)
		{
			if (Segment->IndexInConveyorBelt == 0)
				return;
			//NOTE FÖR FRAMTIDEN ISTÄLLET FÖR ATT KOLLA OM DEN ÄR ÖVER 0.5 och byta
			//KOLLA ATT DEN ÄR UNDER 0.25 på current segment, 
			//eller över 0.75 på previous segment
			if (Belt->MovedDelta > 0.5)				
				Segment = Belt->Conveyor[Segment->IndexInConveyorBelt-1];				
			if (Belt->HasItemInSegment(Segment))
			{
				HeldItem = Belt->GetItemFromSegment(Segment);
				Belt->DropItem(HeldItem);
				AttatchPackage();
			}
			else			
				Belt->ReceiveItem(HeldItem,Segment);			
		}
	}
}

void ASpmG5Character::Drop()
{
	InteractWithCoinvayer();	
	//Testar att sätta den innan och efter	
	HeldItem->ResetVelocity();
	HeldItem->SetPhysics(true);
	HeldItem->ResetVelocity();
	
	HeldItem = nullptr;
	HasItem = false;	
}

void ASpmG5Character::Throw(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Throw"))
	if (!HeldItem)
		return;
	
	//Testar att sätta den innan och efter
	HeldItem->SetPhysics(true);
	HeldItem->AddVelocity(ThrowForce);
	
	//Resettar inför pickup
	HeldItem = nullptr;
	HasItem = false;
}

void ASpmG5Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HeldItem)
	{
		FVector HoldingLocationWorld = HoldingLocation->GetComponentLocation();
		HeldItem->SetActorLocationAndRotation(HoldingLocationWorld, GetActorRotation());		
	}
}

void ASpmG5Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

/*void ASpmG5Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}*/

void ASpmG5Character::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator YawRotation(0, 0, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

/*void ASpmG5Character::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}*/

void ASpmG5Character::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void ASpmG5Character::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}
bool ASpmG5Character::GetHasItem()
{
	return HasItem;
}

AItem* ASpmG5Character::GetItem()
{
	return HeldItem;
}
