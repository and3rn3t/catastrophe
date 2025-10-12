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

	// Update stamina system
	UpdateStamina(DeltaTime);

	// Update crouch system
	UpdateCrouch(DeltaTime);

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

	// Bind interact
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACatCharacter::Interact);

	// Bind climbing (uses Interact key to start/stop climbing)
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
