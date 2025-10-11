// CATastrophe - A Mischief Simulator

#include "CatCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
ACatCharacter::ACatCharacter()
{
 	// Set this character to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Initialize cat stats
	MischiefScore = 0.0f;
	bIsSprinting = false;
	WalkSpeed = 400.0f;
	SprintSpeed = 800.0f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

// Called when the game starts or when spawned
void ACatCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("CATastrophe: Cat is ready to cause mischief!"));
}

// Called every frame
void ACatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind movement functions
	PlayerInputComponent->BindAxis("MoveForward", this, &ACatCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACatCharacter::MoveRight);

	// Bind camera rotation
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// Bind jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind sprint
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACatCharacter::StartSprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACatCharacter::StopSprinting);

	// Bind interact
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACatCharacter::Interact);
}

void ACatCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACatCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// Find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ACatCharacter::StartSprinting()
{
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	UE_LOG(LogTemp, Warning, TEXT("Cat is sprinting! (More risky!)"));
}

void ACatCharacter::StopSprinting()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ACatCharacter::Interact()
{
	// Perform a sphere trace to find interactable objects nearby
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation; // Same location for sphere trace
	float SphereRadius = 150.0f;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(SphereRadius);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	TArray<FHitResult> HitResults;
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECC_Visibility,
		Sphere,
		QueryParams
	);

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor)
			{
				// Check if the actor has an interaction interface or tag
				if (HitActor->ActorHasTag("Destructible"))
				{
					UE_LOG(LogTemp, Warning, TEXT("Cat knocked over: %s"), *HitActor->GetName());
					
					// Add physics impulse to knock it over
					UPrimitiveComponent* PrimitiveComp = Cast<UPrimitiveComponent>(HitActor->GetRootComponent());
					if (PrimitiveComp && PrimitiveComp->IsSimulatingPhysics())
					{
						FVector ImpulseDirection = (HitActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
						PrimitiveComp->AddImpulse(ImpulseDirection * 500.0f, NAME_None, true);
					}

					// Award points based on the tag
					if (HitActor->ActorHasTag("Vase"))
					{
						AddMischiefScore(10.0f);
					}
					else if (HitActor->ActorHasTag("Furniture"))
					{
						AddMischiefScore(15.0f);
					}
					else if (HitActor->ActorHasTag("Curtain"))
					{
						AddMischiefScore(20.0f);
					}
					else if (HitActor->ActorHasTag("Food"))
					{
						AddMischiefScore(25.0f);
						HitActor->Destroy(); // Eat the food!
					}
				}
			}
		}
	}

	// Visual debug for interaction range
	DrawDebugSphere(GetWorld(), StartLocation, SphereRadius, 12, FColor::Green, false, 0.5f);
}

void ACatCharacter::AddMischiefScore(float Points)
{
	MischiefScore += Points;
	UE_LOG(LogTemp, Warning, TEXT("Mischief Score: %.0f (+%.0f)"), MischiefScore, Points);
}
