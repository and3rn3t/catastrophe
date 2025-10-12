// CATastrophe - A Mischief Simulator

#include "CatCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HumanNPC.h"

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

	// Initialize enhanced jump mechanics
	MaxJumpHoldTime = 0.3f;          // Can hold jump for 300ms
	MinJumpVelocity = 400.0f;        // Minimum jump height (tap)
	MaxJumpVelocity = 800.0f;        // Maximum jump height (hold)
	CoyoteTime = 0.15f;              // 150ms grace period after leaving edge
	JumpBufferTime = 0.1f;           // 100ms jump input buffering
	ImprovedAirControl = 0.5f;       // Better mid-air control

	// Initialize jump state
	bJumpHeld = false;
	JumpHoldTime = 0.0f;
	TimeSinceLeftGround = 0.0f;
	JumpBufferTimer = 0.0f;
	bWasGroundedLastFrame = true;

	// Apply improved air control
	GetCharacterMovement()->AirControl = ImprovedAirControl;

	// Initialize climbing system
	ClimbSpeed = 300.0f;             // Vertical climb speed
	ClimbDetectionRadius = 100.0f;   // How far to check for climbable surfaces
	ClimbCapsuleHeight = 96.0f;      // Capsule height while climbing
	ClimbCapsuleRadius = 42.0f;      // Capsule radius while climbing
	bCanClimbJump = true;            // Allow jumping off walls
	ClimbJumpVelocity = 500.0f;      // Jump power when leaving climb

	// Initialize climb state
	bIsClimbing = false;
	CurrentClimbableSurface = nullptr;
	ClimbSurfaceNormal = FVector::ZeroVector;
	ClimbHeightOnSurface = 0.0f;

	// Initialize stamina system
	MaxStamina = 100.0f;             // Maximum stamina pool
	CurrentStamina = MaxStamina;     // Start with full stamina
	StaminaDrainRate = 20.0f;        // Stamina per second while sprinting
	StaminaRegenRate = 15.0f;        // Stamina per second when regenerating
	StaminaRegenDelay = 2.0f;        // Delay before stamina starts regenerating
	ClimbStaminaDrainRate = 10.0f;   // Stamina per second while climbing
	MinStaminaToSprint = 5.0f;       // Minimum stamina needed to sprint
	MinStaminaToClimb = 10.0f;       // Minimum stamina needed to climb

	// Initialize stamina state
	TimeSinceStaminaUse = 0.0f;
	bStaminaDepleted = false;

	// Initialize stealth/crouch system
	CrouchSpeed = 200.0f;                // Move slower while crouched
	StandingCapsuleHalfHeight = 96.0f;   // Default standing height
	CrouchCapsuleHalfHeight = 58.0f;     // Crouched height (about 60% of standing)
	CrouchTransitionSpeed = 10.0f;       // Speed of crouch animation
	StealthDetectionMultiplier = 0.5f;   // 50% detection range while crouched

	// Initialize stealth state
	bIsCrouching = false;
	bWantsToCrouch = false;
	CurrentCapsuleHalfHeight = StandingCapsuleHalfHeight;

	// Initialize pounce/attack system
	PounceForce = 1200.0f;               // Forward launch force for pounce
	PounceCooldown = 1.5f;               // 1.5 second cooldown between pounces
	PounceStaminaCost = 15.0f;           // Stamina cost per pounce
	PounceRange = 200.0f;                // Detection range for pounce targets
	PounceAirControlMultiplier = 0.8f;   // Extra air control during pounce

	// Initialize pounce state
	bIsPouncing = false;
	PounceCooldownTimer = 0.0f;
	PounceDirection = FVector::ZeroVector;

	// Create tail base component
	TailBase = CreateDefaultSubobject<USceneComponent>(TEXT("TailBase"));
	TailBase->SetupAttachment(GetMesh(), TEXT("TailSocket")); // Attach to skeleton tail socket if available

	// Initialize tail physics system
	TailSegmentCount = 5;                // Number of tail segments
	TailSegmentLength = 15.0f;           // Length of each tail segment
	TailStiffness = 0.8f;                // How rigid the tail is (0-1)
	TailDamping = 0.9f;                  // Velocity damping (0-1)
	TailGravityScale = 0.3f;             // How much gravity affects the tail

	// Initialize sound system
	MeowSound = nullptr;                 // Set in Blueprint
	PurrSound = nullptr;                 // Set in Blueprint
	HissSound = nullptr;                 // Set in Blueprint
	LandingSound = nullptr;              // Set in Blueprint
	PounceSound = nullptr;               // Set in Blueprint
	MeowCooldown = 3.0f;                 // 3 seconds between meows
	PurrInterval = 5.0f;                 // Check for purr every 5 seconds
	PurrChance = 0.3f;                   // 30% chance to purr when idle
	bCanPlayMeow = true;                 // Can play meow immediately

	// Initialize sound state
	MeowCooldownTimer = 0.0f;
	PurrTimer = 0.0f;
	bIsPurring = false;
}

// Called when the game starts or when spawned
void ACatCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("CATastrophe: Cat is ready to cause mischief!"));

	// Initialize tail physics
	InitializeTail();
}

// Called every frame
void ACatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update stamina system
	UpdateStamina(DeltaTime);

	// Update crouch system
	UpdateCrouch(DeltaTime);

	// Update pounce system
	UpdatePounce(DeltaTime);

	// Update tail physics
	UpdateTailPhysics(DeltaTime);

	// Update sound system
	UpdateSoundSystem(DeltaTime);

	// Update climbing system
	if (bIsClimbing)
	{
		UpdateClimbing(DeltaTime);
	}

	bool bIsGrounded = GetCharacterMovement()->IsMovingOnGround();

	// Track time since leaving ground for coyote time
	if (!bIsGrounded && bWasGroundedLastFrame)
	{
		TimeSinceLeftGround = 0.0f;
	}

	if (!bIsGrounded)
	{
		TimeSinceLeftGround += DeltaTime;
	}

	// Handle variable jump height (hold for higher jump)
	if (bJumpHeld && !bIsGrounded && JumpHoldTime < MaxJumpHoldTime)
	{
		JumpHoldTime += DeltaTime;

		// Gradually increase jump velocity while holding
		float JumpProgress = FMath::Clamp(JumpHoldTime / MaxJumpHoldTime, 0.0f, 1.0f);
		float TargetVelocity = FMath::Lerp(MinJumpVelocity, MaxJumpVelocity, JumpProgress);

		// Only add upward velocity if we're still ascending
		if (GetVelocity().Z > 0)
		{
			FVector NewVelocity = GetVelocity();
			NewVelocity.Z = TargetVelocity;
			GetCharacterMovement()->Velocity = NewVelocity;
		}
	}

	// Handle jump buffering (pressing jump slightly before landing)
	if (JumpBufferTimer > 0.0f)
	{
		JumpBufferTimer -= DeltaTime;

		// If we just landed and have a buffered jump input, execute it
		if (bIsGrounded && !bWasGroundedLastFrame)
		{
			Jump();
			JumpBufferTimer = 0.0f;
		}
	}

	bWasGroundedLastFrame = bIsGrounded;
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
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACatCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACatCharacter::StopJumping);

	// Bind sprint
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACatCharacter::StartSprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACatCharacter::StopSprinting);

	// Bind crouch
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACatCharacter::ToggleCrouch);

	// Bind pounce (will use a new input action)
	PlayerInputComponent->BindAction("Pounce", IE_Pressed, this, &ACatCharacter::Pounce);

	// Bind interact
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACatCharacter::Interact);

	// Bind meow
	PlayerInputComponent->BindAction("Meow", IE_Pressed, this, &ACatCharacter::PlayMeow);

	// Climbing is automatic when near climbable surface and moving upward
}

void ACatCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		if (bIsClimbing)
		{
			// While climbing, forward/backward controls vertical movement
			HandleClimbMovement(Value);
		}
		else
		{
			// Normal ground movement
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);

			// Auto-start climbing if moving forward into a climbable surface
			if (Value > 0.0f && CanClimb())
			{
				StartClimbing();
			}
		}
	}
}

void ACatCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		if (bIsClimbing)
		{
			// While climbing, left/right strafes along the wall
			FVector RightVector = FVector::CrossProduct(GetClimbUpVector(), ClimbSurfaceNormal);
			RightVector.Normalize();

			FVector NewLocation = GetActorLocation() + (RightVector * Value * ClimbSpeed * GetWorld()->GetDeltaSeconds());
			SetActorLocation(NewLocation, true);
		}
		else
		{
			// Normal ground movement
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, Value);
		}
	}
}

void ACatCharacter::StartSprinting()
{
	// Can't sprint while crouching
	if (bIsCrouching)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't sprint while crouching!"));
		return;
	}

	// Check if we have enough stamina to sprint
	if (CurrentStamina < MinStaminaToSprint || bStaminaDepleted)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough stamina to sprint!"));
		return;
	}

	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	UE_LOG(LogTemp, Warning, TEXT("Cat is sprinting!"));
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

					// Apply physics impulse to knock it over
					UPrimitiveComponent* PrimitiveComp = Cast<UPrimitiveComponent>(HitActor->GetRootComponent());
					if (PrimitiveComp && PrimitiveComp->IsSimulatingPhysics())
					{
						FVector ImpulseDirection = (HitActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
						PrimitiveComp->AddImpulse(ImpulseDirection * 500.0f, NAME_None, true);
						
						// Emit noise when knocking things over
						EmitNoise(40.0f); // Moderate noise
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

void ACatCharacter::Jump()
{
	bool bIsGrounded = GetCharacterMovement()->IsMovingOnGround();

	// Coyote time: Allow jump for brief period after leaving ground
	bool bCanCoyoteJump = !bIsGrounded && TimeSinceLeftGround < CoyoteTime;

	if (bIsGrounded || bCanCoyoteJump)
	{
		// Execute jump with minimum velocity
		Super::Jump();

		bJumpHeld = true;
		JumpHoldTime = 0.0f;

		// Set initial jump velocity to minimum
		FVector NewVelocity = GetVelocity();
		NewVelocity.Z = MinJumpVelocity;
		GetCharacterMovement()->Velocity = NewVelocity;

		UE_LOG(LogTemp, Log, TEXT("Cat jumped! (Hold for higher jump)"));
	}
	else
	{
		// Buffer the jump input for landing
		JumpBufferTimer = JumpBufferTime;
		UE_LOG(LogTemp, Log, TEXT("Jump buffered - will execute on landing"));
	}
}

void ACatCharacter::StopJumping()
{
	Super::StopJumping();
	bJumpHeld = false;

	// Cut vertical velocity when releasing jump early for tighter control
	if (GetVelocity().Z > 0)
	{
		FVector NewVelocity = GetVelocity();
		NewVelocity.Z *= 0.5f; // Cut jump short
		GetCharacterMovement()->Velocity = NewVelocity;
	}
}

// ============================================================================
// CLIMBING SYSTEM IMPLEMENTATION
// ============================================================================

void ACatCharacter::StartClimbing()
{
	if (bIsClimbing || !CanClimb())
	{
		return;
	}

	// Can't climb while crouching
	if (bIsCrouching)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't climb while crouching! Stand up first."));
		return;
	}

	// Check if we have enough stamina to start climbing
	if (CurrentStamina < MinStaminaToClimb)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough stamina to climb!"));
		return;
	}

	AActor* ClimbableSurface = DetectClimbableSurface();
	if (!ClimbableSurface)
	{
		return;
	}

	bIsClimbing = true;
	CurrentClimbableSurface = ClimbableSurface;

	// Disable gravity and switch to flying movement mode
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCharacterMovement()->GravityScale = 0.0f;

	// Orient character to face away from the wall
	FRotator NewRotation = (-ClimbSurfaceNormal).Rotation();
	SetActorRotation(NewRotation);

	// Track starting height
	ClimbHeightOnSurface = GetActorLocation().Z;

	UE_LOG(LogTemp, Warning, TEXT("Started climbing!"));
}

void ACatCharacter::StopClimbing()
{
	if (!bIsClimbing)
	{
		return;
	}

	bIsClimbing = false;
	CurrentClimbableSurface = nullptr;

	// Re-enable gravity and return to walking mode
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetCharacterMovement()->GravityScale = 1.0f;

	UE_LOG(LogTemp, Warning, TEXT("Stopped climbing"));
}

bool ACatCharacter::CanClimb()
{
	// Can't climb if already climbing or in air
	if (bIsClimbing)
	{
		return false;
	}

	// Check if there's a climbable surface nearby
	return (DetectClimbableSurface() != nullptr);
}

AActor* ACatCharacter::DetectClimbableSurface()
{
	FVector StartLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector EndLocation = StartLocation + (ForwardVector * ClimbDetectionRadius);

	// Perform sphere trace to find climbable surfaces
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(30.0f),
		QueryParams
	);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();

		// Check if the hit actor is tagged as "Climbable"
		if (HitActor && HitActor->ActorHasTag("Climbable"))
		{
			// Store the surface normal for later use
			ClimbSurfaceNormal = HitResult.ImpactNormal;

			// Debug visualization
			DrawDebugLine(GetWorld(), StartLocation, HitResult.Location, FColor::Cyan, false, 0.1f, 0, 2.0f);

			return HitActor;
		}
	}

	return nullptr;
}

void ACatCharacter::UpdateClimbing(float DeltaTime)
{
	if (!bIsClimbing)
	{
		return;
	}

	// Check if we're still near a climbable surface
	AActor* ClimbableSurface = DetectClimbableSurface();
	if (!ClimbableSurface)
	{
		// Lost contact with surface, stop climbing
		StopClimbing();
		return;
	}

	// Update the surface normal
	CurrentClimbableSurface = ClimbableSurface;

	// Keep character oriented to face away from wall
	FRotator TargetRotation = (-ClimbSurfaceNormal).Rotation();
	FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 10.0f);
	SetActorRotation(NewRotation);

	// Handle jump off wall
	if (bCanClimbJump && GetInputAxisValue("Jump") > 0.0f)
	{
		// Jump away from the wall
		StopClimbing();

		FVector JumpDirection = ClimbSurfaceNormal + FVector(0, 0, 0.5f); // Slightly upward
		JumpDirection.Normalize();

		LaunchCharacter(JumpDirection * ClimbJumpVelocity, true, true);

		UE_LOG(LogTemp, Warning, TEXT("Jumped off wall!"));
	}

	// Auto-dismount if moving backward (away from wall)
	float BackwardInput = GetInputAxisValue("MoveForward");
	if (BackwardInput < -0.5f)
	{
		StopClimbing();
	}
}

void ACatCharacter::HandleClimbMovement(float Value)
{
	if (!bIsClimbing)
	{
		return;
	}

	// Move up/down along the surface
	FVector UpVector = GetClimbUpVector();
	FVector NewLocation = GetActorLocation() + (UpVector * Value * ClimbSpeed * GetWorld()->GetDeltaSeconds());

	// Apply the movement with collision
	FHitResult HitResult;
	SetActorLocation(NewLocation, true, &HitResult);

	// Update height tracking
	ClimbHeightOnSurface = NewLocation.Z;

	// If we hit something above us (ceiling/top of object), stop climbing
	if (HitResult.bBlockingHit && Value > 0.0f)
	{
		// Reached top - dismount with a little boost
		StopClimbing();
		LaunchCharacter(FVector(0, 0, 200.0f), false, false);
		UE_LOG(LogTemp, Warning, TEXT("Reached top of climbable surface!"));
	}
}

FVector ACatCharacter::GetClimbUpVector() const
{
	// Calculate the "up" direction relative to the wall
	// This is perpendicular to both the surface normal and world right
	FVector WorldUp = FVector(0, 0, 1);
	FVector RightVector = FVector::CrossProduct(WorldUp, ClimbSurfaceNormal);
	FVector UpVector = FVector::CrossProduct(ClimbSurfaceNormal, RightVector);
	UpVector.Normalize();

	return UpVector;
}

// ============================================================================
// STAMINA SYSTEM IMPLEMENTATION
// ============================================================================

void ACatCharacter::UpdateStamina(float DeltaTime)
{
	bool bIsUsingStamina = false;

	// Drain stamina while sprinting
	if (bIsSprinting)
	{
		ConsumeStamina(StaminaDrainRate * DeltaTime);
		bIsUsingStamina = true;

		// Force stop sprinting if stamina depleted
		if (CurrentStamina <= 0.0f)
		{
			StopSprinting();
			bStaminaDepleted = true;
			UE_LOG(LogTemp, Warning, TEXT("Stamina depleted! Need to rest."));
		}
	}

	// Drain stamina while climbing
	if (bIsClimbing)
	{
		ConsumeStamina(ClimbStaminaDrainRate * DeltaTime);
		bIsUsingStamina = true;

		// Force stop climbing if stamina too low
		if (CurrentStamina < MinStaminaToClimb)
		{
			StopClimbing();
			UE_LOG(LogTemp, Warning, TEXT("Too tired to keep climbing!"));
		}
	}

	// Track time since last stamina use
	if (bIsUsingStamina)
	{
		TimeSinceStaminaUse = 0.0f;
	}
	else
	{
		TimeSinceStaminaUse += DeltaTime;
	}

	// Regenerate stamina after delay
	if (!bIsUsingStamina && TimeSinceStaminaUse >= StaminaRegenDelay)
	{
		RegenerateStamina(StaminaRegenRate * DeltaTime);

		// Clear depleted flag once we've recovered some stamina
		if (bStaminaDepleted && CurrentStamina >= MinStaminaToSprint)
		{
			bStaminaDepleted = false;
			UE_LOG(LogTemp, Log, TEXT("Stamina recovered!"));
		}
	}
}

void ACatCharacter::ConsumeStamina(float Amount)
{
	CurrentStamina = FMath::Max(0.0f, CurrentStamina - Amount);
}

void ACatCharacter::RegenerateStamina(float Amount)
{
	CurrentStamina = FMath::Min(MaxStamina, CurrentStamina + Amount);
}

// ============================================================================
// Stealth/Crouch System Implementation
// ============================================================================

void ACatCharacter::ToggleCrouch()
{
	if (bIsCrouching)
	{
		StopCrouching();
	}
	else
	{
		StartCrouching();
	}
}

void ACatCharacter::StartCrouching()
{
	// Can't crouch while climbing or in the air
	if (bIsClimbing || !GetCharacterMovement()->IsMovingOnGround())
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't crouch while climbing or in the air!"));
		return;
	}

	// Can't crouch while sprinting
	if (bIsSprinting)
	{
		StopSprinting();
	}

	bWantsToCrouch = true;
	UE_LOG(LogTemp, Log, TEXT("Cat is crouching (stealth mode)"));
}

void ACatCharacter::StopCrouching()
{
	// Check if there's enough room to stand up
	if (!CanStandUp())
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough room to stand up!"));
		return;
	}

	bWantsToCrouch = false;
	UE_LOG(LogTemp, Log, TEXT("Cat is standing up"));
}

void ACatCharacter::UpdateCrouch(float DeltaTime)
{
	// Determine target capsule height
	float TargetHeight = bWantsToCrouch ? CrouchCapsuleHalfHeight : StandingCapsuleHalfHeight;

	// Smoothly interpolate current height to target
	if (!FMath::IsNearlyEqual(CurrentCapsuleHalfHeight, TargetHeight, 0.1f))
	{
		CurrentCapsuleHalfHeight = FMath::FInterpTo(
			CurrentCapsuleHalfHeight,
			TargetHeight,
			DeltaTime,
			CrouchTransitionSpeed
		);

		// Update capsule component
		GetCapsuleComponent()->SetCapsuleHalfHeight(CurrentCapsuleHalfHeight);

		// Adjust mesh location to keep feet on ground
		if (GetMesh())
		{
			float HeightDifference = StandingCapsuleHalfHeight - CurrentCapsuleHalfHeight;
			GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -CurrentCapsuleHalfHeight));
		}
	}

	// Update crouching state based on actual height
	bool bWasCrouching = bIsCrouching;
	bIsCrouching = FMath::IsNearlyEqual(CurrentCapsuleHalfHeight, CrouchCapsuleHalfHeight, 1.0f);

	// Adjust movement speed when crouch state changes
	if (bIsCrouching != bWasCrouching)
	{
		if (bIsCrouching)
		{
			// Entered crouch state
			GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
		}
		else
		{
			// Exited crouch state
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		}
	}
}

bool ACatCharacter::CanStandUp() const
{
	if (!bIsCrouching)
	{
		return true;
	}

	// Check if there's enough vertical space to stand up
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation; // Same location for capsule check

	// Calculate the height difference
	float HeightDifference = StandingCapsuleHalfHeight - CrouchCapsuleHalfHeight;

	// Create a capsule shape for the standing size
	FCollisionShape StandingCapsule = FCollisionShape::MakeCapsule(
		GetCapsuleComponent()->GetScaledCapsuleRadius(),
		StandingCapsuleHalfHeight
	);

	// Lift the check slightly above current position
	EndLocation.Z += HeightDifference;

	// Check for obstacles above
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHasSpace = !GetWorld()->SweepTestByChannel(
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECC_Pawn,
		StandingCapsule,
		QueryParams
	);

	return bHasSpace;
}

// ============================================================================
// POUNCE/ATTACK SYSTEM IMPLEMENTATION
// ============================================================================

void ACatCharacter::Pounce()
{
	if (!CanPounce())
	{
		return;
	}

	// Can't pounce while climbing or crouching
	if (bIsClimbing || bIsCrouching)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't pounce while climbing or crouching!"));
		return;
	}

	// Consume stamina
	ConsumeStamina(PounceStaminaCost);

	// Calculate pounce direction
	FVector ForwardVector = GetActorForwardVector();
	FVector UpVector = FVector(0, 0, 1);

	// Combine forward and slightly upward for arc trajectory
	PounceDirection = (ForwardVector * 0.8f + UpVector * 0.4f).GetSafeNormal();

	// Launch character in pounce direction
	LaunchCharacter(PounceDirection * PounceForce, true, true);

	// Set pouncing state
	bIsPouncing = true;
	PounceCooldownTimer = PounceCooldown;

	// Increase air control during pounce
	float OriginalAirControl = GetCharacterMovement()->AirControl;
	GetCharacterMovement()->AirControl = OriginalAirControl * PounceAirControlMultiplier;

	// Check for objects in pounce path
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + (PounceDirection * PounceRange);

	TArray<FHitResult> HitResults;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(50.0f);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

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
			if (HitActor && HitActor->ActorHasTag("Destructible"))
			{
				// Apply stronger impulse to pounced objects
				UPrimitiveComponent* PrimitiveComp = Cast<UPrimitiveComponent>(HitActor->GetRootComponent());
				if (PrimitiveComp && PrimitiveComp->IsSimulatingPhysics())
				{
					FVector ImpulseDirection = (HitActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
					PrimitiveComp->AddImpulse(ImpulseDirection * PounceForce * 1.5f, NAME_None, true);
				}

				// Award bonus points for pouncing
				if (HitActor->ActorHasTag("Vase"))
				{
					AddMischiefScore(15.0f); // Bonus +5 for pouncing
				}
				else if (HitActor->ActorHasTag("Furniture"))
				{
					AddMischiefScore(20.0f); // Bonus +5
				}
				else if (HitActor->ActorHasTag("Curtain"))
				{
					AddMischiefScore(25.0f); // Bonus +5
				}
				else if (HitActor->ActorHasTag("Food"))
				{
					AddMischiefScore(30.0f); // Bonus +5
				}

				UE_LOG(LogTemp, Warning, TEXT("Pounced on: %s! Bonus points!"), *HitActor->GetName());
			}
		}
	}

	// Debug visualization
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 0.5f, 0, 3.0f);

	// Play pounce sound
	PlayPounceSound();

	UE_LOG(LogTemp, Warning, TEXT("Cat pounced!"));
}

bool ACatCharacter::CanPounce() const
{
	// Check cooldown
	if (PounceCooldownTimer > 0.0f)
	{
		return false;
	}

	// Check stamina
	if (CurrentStamina < PounceStaminaCost)
	{
		return false;
	}

	// Can't pounce while already pouncing
	if (bIsPouncing)
	{
		return false;
	}

	// Must be grounded or recently grounded (coyote time)
	bool bIsGrounded = GetCharacterMovement()->IsMovingOnGround();
	if (!bIsGrounded && TimeSinceLeftGround > CoyoteTime)
	{
		return false;
	}

	return true;
}

void ACatCharacter::UpdatePounce(float DeltaTime)
{
	// Update cooldown timer
	if (PounceCooldownTimer > 0.0f)
	{
		PounceCooldownTimer -= DeltaTime;
		if (PounceCooldownTimer < 0.0f)
		{
			PounceCooldownTimer = 0.0f;
		}
	}

	// Check if pounce has ended (landed)
	if (bIsPouncing && GetCharacterMovement()->IsMovingOnGround())
	{
		HandlePounceLanding();
	}
}

void ACatCharacter::HandlePounceLanding()
{
	bIsPouncing = false;

	// Reset air control to normal
	GetCharacterMovement()->AirControl = ImprovedAirControl;

	// Check for landing on destructible objects
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation - FVector(0, 0, 100.0f);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility,
		QueryParams
	);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && HitActor->ActorHasTag("Destructible"))
		{
			// Apply downward force on landing
			UPrimitiveComponent* PrimitiveComp = Cast<UPrimitiveComponent>(HitActor->GetRootComponent());
			if (PrimitiveComp && PrimitiveComp->IsSimulatingPhysics())
			{
				FVector DownwardForce = FVector(0, 0, -1) * PounceForce * 0.5f;
				PrimitiveComp->AddImpulseAtLocation(DownwardForce, GetActorLocation(), NAME_None);

				UE_LOG(LogTemp, Warning, TEXT("Landed on %s with impact!"), *HitActor->GetName());
			}
		}
	}

	// Play landing sound
	PlayLandingSound();

	UE_LOG(LogTemp, Log, TEXT("Pounce landing complete"));
}

// ============================================================================
// TAIL PHYSICS SYSTEM IMPLEMENTATION
// ============================================================================

void ACatCharacter::InitializeTail()
{
	// Initialize tail segment positions and velocities
	TailSegmentPositions.Empty();
	TailSegmentVelocities.Empty();

	FVector TailStartPosition = TailBase->GetComponentLocation();

	for (int32 i = 0; i < TailSegmentCount; ++i)
	{
		// Position segments in a line behind the cat
		FVector SegmentPosition = TailStartPosition - (GetActorForwardVector() * TailSegmentLength * i);
		TailSegmentPositions.Add(SegmentPosition);
		TailSegmentVelocities.Add(FVector::ZeroVector);
	}

	UE_LOG(LogTemp, Log, TEXT("Tail physics initialized with %d segments"), TailSegmentCount);
}

void ACatCharacter::UpdateTailPhysics(float DeltaTime)
{
	if (TailSegmentPositions.Num() != TailSegmentCount || TailSegmentVelocities.Num() != TailSegmentCount)
	{
		InitializeTail();
		return;
	}

	// First segment follows the tail base (attached to character)
	FVector TailBasePosition = TailBase->GetComponentLocation();

	// Update each segment using Verlet integration
	for (int32 i = 0; i < TailSegmentCount; ++i)
	{
		SimulateTailSegment(i, DeltaTime);
	}

	// Constraint pass: Enforce segment length constraints (multiple iterations for stability)
	for (int32 Iteration = 0; Iteration < 3; ++Iteration)
	{
		// First segment constrained to tail base
		FVector Direction = TailSegmentPositions[0] - TailBasePosition;
		float Distance = Direction.Size();

		if (Distance > 0.001f)
		{
			Direction /= Distance;
			TailSegmentPositions[0] = TailBasePosition + Direction * TailSegmentLength;
		}

		// Subsequent segments constrained to previous segment
		for (int32 i = 1; i < TailSegmentCount; ++i)
		{
			FVector ToNext = TailSegmentPositions[i] - TailSegmentPositions[i - 1];
			float SegmentDistance = ToNext.Size();

			if (SegmentDistance > 0.001f)
			{
				ToNext /= SegmentDistance;

				// Apply stiffness constraint
				float Offset = (SegmentDistance - TailSegmentLength) * TailStiffness;
				TailSegmentPositions[i] = TailSegmentPositions[i] - ToNext * Offset;
			}
		}
	}

	// Notify Blueprint that tail has been updated (for visual representation)
	OnTailPositionsUpdated();
}

void ACatCharacter::SimulateTailSegment(int32 SegmentIndex, float DeltaTime)
{
	if (SegmentIndex < 0 || SegmentIndex >= TailSegmentPositions.Num())
	{
		return;
	}

	FVector& Position = TailSegmentPositions[SegmentIndex];
	FVector& Velocity = TailSegmentVelocities[SegmentIndex];

	// Apply gravity
	FVector Gravity = FVector(0, 0, -980.0f) * TailGravityScale;
	Velocity += Gravity * DeltaTime;

	// Apply damping
	Velocity *= TailDamping;

	// Inherit some velocity from character movement (tail follows body)
	if (SegmentIndex == 0)
	{
		FVector CharacterVelocity = GetVelocity();
		Velocity += CharacterVelocity * 0.1f * DeltaTime; // 10% influence
	}

	// Apply velocity to position
	Position += Velocity * DeltaTime;
}

// ============================================================================
// SOUND SYSTEM IMPLEMENTATION
// ============================================================================

void ACatCharacter::PlayMeow()
{
	if (!bCanPlayMeow || MeowCooldownTimer > 0.0f)
	{
		return;
	}

	if (MeowSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, MeowSound, GetActorLocation());
		UE_LOG(LogTemp, Log, TEXT("Cat says: Meow!"));
		
		// Emit noise for nearby NPCs to hear
		EmitNoise(50.0f); // Medium loudness
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MeowSound not set in Blueprint!"));
	}

	// Start cooldown
	MeowCooldownTimer = MeowCooldown;
	bCanPlayMeow = false;
}

void ACatCharacter::PlayPurr()
{
	if (bIsPurring)
	{
		return; // Already purring
	}

	if (PurrSound)
	{
		// Play purr sound (should be looping in sound asset)
		UGameplayStatics::PlaySoundAtLocation(this, PurrSound, GetActorLocation());
		bIsPurring = true;
		UE_LOG(LogTemp, Log, TEXT("Cat is purring..."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PurrSound not set in Blueprint!"));
	}
}

void ACatCharacter::StopPurr()
{
	if (!bIsPurring)
	{
		return;
	}

	bIsPurring = false;
	UE_LOG(LogTemp, Log, TEXT("Cat stopped purring"));
}

void ACatCharacter::PlayHiss()
{
	if (HissSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HissSound, GetActorLocation());
		UE_LOG(LogTemp, Log, TEXT("Cat hisses!"));
		
		// Emit loud noise
		EmitNoise(80.0f); // Very loud
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HissSound not set in Blueprint!"));
	}
}

void ACatCharacter::PlayLandingSound()
{
	if (LandingSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LandingSound, GetActorLocation());
		UE_LOG(LogTemp, Log, TEXT("Cat landed with a thump"));
		
		// Emit noise based on fall velocity
		float LandingVelocity = FMath::Abs(GetVelocity().Z);
		float Loudness = FMath::Clamp(LandingVelocity / 10.0f, 10.0f, 60.0f);
		EmitNoise(Loudness);
	}
}

void ACatCharacter::PlayPounceSound()
{
	if (PounceSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PounceSound, GetActorLocation());
		UE_LOG(LogTemp, Log, TEXT("Cat pounce sound!"));
		
		// Emit loud noise
		EmitNoise(70.0f); // Loud
	}
}

void ACatCharacter::EmitNoise(float Loudness)
{
	if (!GetWorld())
	{
		return;
	}

	// Find all Human NPCs in range and notify them
	TArray<AActor*> FoundNPCs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHumanNPC::StaticClass(), FoundNPCs);

	FVector NoiseLocation = GetActorLocation();

	for (AActor* Actor : FoundNPCs)
	{
		AHumanNPC* NPC = Cast<AHumanNPC>(Actor);
		if (NPC)
		{
			NPC->OnNoiseHeard(NoiseLocation, Loudness, this);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Cat emitted noise (Loudness: %.1f) at location %s"), Loudness, *NoiseLocation.ToString());
}

void ACatCharacter::UpdateSoundSystem(float DeltaTime)
{
	// Update meow cooldown
	if (MeowCooldownTimer > 0.0f)
	{
		MeowCooldownTimer -= DeltaTime;
		if (MeowCooldownTimer <= 0.0f)
		{
			MeowCooldownTimer = 0.0f;
			bCanPlayMeow = true;
		}
	}

	// Update purr timer and random purring
	PurrTimer += DeltaTime;
	if (PurrTimer >= PurrInterval)
	{
		PurrTimer = 0.0f;
		TryPlayRandomPurr();
	}
}

void ACatCharacter::TryPlayRandomPurr()
{
	// Only purr if idle (not moving much, not jumping, not climbing)
	bool bIsIdle = GetVelocity().Size() < 50.0f &&
	               GetCharacterMovement()->IsMovingOnGround() &&
	               !bIsClimbing &&
	               !bIsPouncing;

	if (bIsIdle && !bIsPurring)
	{
		// Random chance to start purring
		float RandomValue = FMath::FRand();
		if (RandomValue < PurrChance)
		{
			PlayPurr();
		}
	}
	else if (!bIsIdle && bIsPurring)
	{
		// Stop purring if no longer idle
		StopPurr();
	}
}
