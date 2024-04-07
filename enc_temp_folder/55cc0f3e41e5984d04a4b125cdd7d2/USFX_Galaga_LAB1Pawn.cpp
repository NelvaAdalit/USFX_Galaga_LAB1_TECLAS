// Copyright Epic Games, Inc. All Rights Reserved.

#include "USFX_Galaga_LAB1Pawn.h"
#include "USFX_Galaga_LAB1Projectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"//para el 
#include "GameFramework/PlayerInput.h" 
#include "GameFramework/PlayerInput.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"


/*const FName AUSFX_Galaga_LAB1Pawn::MoveForwardBinding("MoveForward");
const FName AUSFX_Galaga_LAB1Pawn::MoveRightBinding("MoveRight");
const FName AUSFX_Galaga_LAB1Pawn::FireForwardBinding("FireForward");
const FName AUSFX_Galaga_LAB1Pawn::FireRightBinding("FireRight");*/

AUSFX_Galaga_LAB1Pawn::AUSFX_Galaga_LAB1Pawn()
{	
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);
	
	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when ship does
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->SetRelativeRotation(FRotator(-80.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm

	// Movement
	MoveSpeed = 1000.0f;
	// Weapon
	GunOffset = FVector(90.f, 0.f, 0.f);
	FireRate = 0.1f;
	bCanFire = true;
}

void AUSFX_Galaga_LAB1Pawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	//check(PlayerInputComponent);

	// set up gameplay key bindings
	/*ayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(FireForwardBinding);
	PlayerInputComponent->BindAxis(FireRightBinding);*/


	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Forward", this,&AUSFX_Galaga_LAB1Pawn::Adelante);
	PlayerInputComponent->BindAxis("Back", this,&AUSFX_Galaga_LAB1Pawn::Atras);
	PlayerInputComponent->BindAxis("Right", this,&AUSFX_Galaga_LAB1Pawn::Derecha);
	PlayerInputComponent->BindAxis("Left", this,&AUSFX_Galaga_LAB1Pawn::Izquierda);	

	FInputAxisKeyMapping fowardKey("Foward", EKeys::Y, 1.f);
	GetWorld()->GetFirstPlayerController()->PlayerInput->AddAxisMapping(fowardKey);
	FInputAxisKeyMapping backKey("Back", EKeys::B, 1.f);
	GetWorld()->GetFirstPlayerController()->PlayerInput->AddAxisMapping(backKey);


}


void AUSFX_Galaga_LAB1Pawn::Adelante(float amount)
{
}

void AUSFX_Galaga_LAB1Pawn::Atras(float amount)
{
}

void AUSFX_Galaga_LAB1Pawn::Derecha(float amount)
{
}

void AUSFX_Galaga_LAB1Pawn::Izquierda(float amount)
{
}




void AUSFX_Galaga_LAB1Pawn::Tick(float DeltaSeconds)
{
	Super::Tick( DeltaSeconds);	

	// Find movement direction
	//posicionNave = GetActorLocation();
	/*const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);*/

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	//const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

	// Calculate  movement
	//const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	// If non-zero size, move this actor
	/*if (Movement.SizeSquared() > 0.0f)
	{
		//const FRotator NewRotation = Movement.Rotation();
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);
		
		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, NewRotation, true);
		}
	}*/
	
	// Create fire direction vector
	/*const float FireForwardValue = GetInputAxisValue(FireForwardBinding);
	const float FireRightValue = GetInputAxisValue(FireRightBinding);
	const FVector FireDirection = FVector(FireForwardValue, FireRightValue, 0.f);*/

	// Try and fire a shot
	//FireShot(FireDirection);
}

void AUSFX_Galaga_LAB1Pawn::FireShot(FVector FireDirection)
{
	// If it's ok to fire again
	if (bCanFire == true)
	{
		// If we are pressing fire stick in a direction
		if (FireDirection.SizeSquared() > 0.0f)
		{
			const FRotator FireRotation = FireDirection.Rotation();
			// Spawn projectile at an offset from this pawn
			const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				// spawn the projectile
				World->SpawnActor<AUSFX_Galaga_LAB1Projectile>(SpawnLocation, FireRotation);
			}

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AUSFX_Galaga_LAB1Pawn::ShotTimerExpired, FireRate);

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			bCanFire = false;
		}
	}
}

void AUSFX_Galaga_LAB1Pawn::ShotTimerExpired()
{
	bCanFire = true;
}

void AUSFX_Galaga_LAB1Pawn::BeginPlay()
{
}






