// CATastrophe - A Mischief Simulator

#include "HumanNPC.h"
#include "CatCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AHumanNPC::AHumanNPC()
{
 	// Set this character to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Vision System Defaults
	DetectionRadius = 1000.0f;
	VisionConeAngle = 90.0f;
	SuspiciousVisionMultiplier = 1.3f;
	AlertedVisionMultiplier = 1.5f;
	bHasDetectedCat = false;
	bShowVisionConeDebug = false;

	// Hearing System Defaults
	HearingRadius = 800.0f;
	LoudNoiseHearingMultiplier = 1.5f;
	QuietNoiseHearingMultiplier = 0.7f;
	bShowHearingDebug = false;

	// State Machine Defaults
	CurrentState = EAIState::Idle;
	CurrentAlertLevel = EAlertLevel::Relaxed;

	// Investigation Defaults
	InvestigationSpeed = 200.0f;
	InvestigationDuration = 5.0f;
	InvestigationStopDistance = 100.0f;
	InvestigationTimer = 0.0f;
	bHasInvestigationTarget = false;

	// Chase Defaults
	ChaseSpeed = 400.0f;
	ChaseGiveUpDistance = 2000.0f;
	ChaseGiveUpTime = 10.0f;
	ChaseTimer = 0.0f;

	// Alert System Defaults
	SuspicionIncreaseRate = 15.0f;
	SuspicionDecreaseRate = 5.0f;
	SuspicionThreshold = 30.0f;
	AlertThreshold = 70.0f;
	CalmDownDelay = 3.0f;
	CurrentSuspicion = 0.0f;
	TimeSinceLastSuspiciousEvent = 0.0f;

	// Patrol Defaults
	PatrolSpeed = 150.0f;
	PatrolWaitTime = 2.0f;
	bPatrolLooping = true;
	PatrolPointReachedDistance = 50.0f;
	CurrentPatrolIndex = 0;
	PatrolWaitTimer = 0.0f;
	bWaitingAtPatrolPoint = false;
	bPatrolForward = true;

	// Idle Defaults
	IdleLookAroundInterval = 3.0f;
	IdleLookAroundAngle = 45.0f;
	IdleLookAroundTimer = 0.0f;
}

// Called when the game starts or when spawned
void AHumanNPC::BeginPlay()
{
	Super::BeginPlay();

	// Find the cat character in the world
	AActor* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	CatCharacter = Cast<ACatCharacter>(PlayerPawn);

	// Store original rotation for idle behavior
	OriginalRotation = GetActorRotation();

	// Start patrol if we have patrol points
	if (PatrolPoints.Num() > 0)
	{
		StartPatrol();
	}
}

// Called every frame
void AHumanNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CatCharacter)
	{
		return;
	}

	// Update alert system (suspicion decay)
	UpdateAlertSystem(DeltaTime);

	// Check for cat detection
	bool bCanSee = CanSeeCat();

	if (bCanSee && !bHasDetectedCat)
	{
		// Cat detected!
		bHasDetectedCat = true;
		LastKnownCatPosition = CatCharacter->GetActorLocation();
		
		// Increase suspicion based on what cat is doing
		if (CatCharacter->IsSprinting())
		{
			IncreaseSuspicion(SuspicionIncreaseRate * 2.0f * DeltaTime);
		}
		else if (!CatCharacter->IsCrouching())
		{
			IncreaseSuspicion(SuspicionIncreaseRate * DeltaTime);
		}
		else
		{
			// Cat is crouching, less suspicious
			IncreaseSuspicion(SuspicionIncreaseRate * 0.5f * DeltaTime);
		}

		OnCatDetected();
	}
	else if (!bCanSee && bHasDetectedCat)
	{
		bHasDetectedCat = false;
		OnCatEscaped();
		
		// Start investigating last known position
		if (CurrentAlertLevel != EAlertLevel::Relaxed)
		{
			StartInvestigation(LastKnownCatPosition);
		}
	}

	// If we can see the cat and alert level is high enough, chase!
	if (bCanSee && CurrentAlertLevel == EAlertLevel::Alerted)
	{
		if (CurrentState != EAIState::Chase)
		{
			StartChase();
		}
		LastKnownCatPosition = CatCharacter->GetActorLocation();
	}

	// Update current state
	switch (CurrentState)
	{
		case EAIState::Idle:
			UpdateIdleState(DeltaTime);
			break;
		case EAIState::Patrol:
			UpdatePatrolState(DeltaTime);
			break;
		case EAIState::Investigate:
			UpdateInvestigateState(DeltaTime);
			break;
		case EAIState::Chase:
			UpdateChaseState(DeltaTime);
			break;
	}

	// Debug visualization
	if (bShowVisionConeDebug)
	{
		DrawDebugVisionCone();
	}

	if (bShowHearingDebug)
	{
		DrawDebugHearingRadius();
	}
}

// Vision System
bool AHumanNPC::CanSeeCat()
{
	if (!CatCharacter)
	{
		return false;
	}

	FVector HumanLocation = GetActorLocation();
	FVector CatLocation = CatCharacter->GetActorLocation();
	FVector DirectionToCat = (CatLocation - HumanLocation).GetSafeNormal();

	// Get effective detection radius based on alert level and stealth
	float EffectiveRadius = GetEffectiveDetectionRadius();

	// Check distance
	float Distance = FVector::Dist(HumanLocation, CatLocation);
	if (Distance > EffectiveRadius)
	{
		return false;
	}

	// Check if cat is in vision cone
	FVector ForwardVector = GetActorForwardVector();
	float DotProduct = FVector::DotProduct(ForwardVector, DirectionToCat);
	float AngleInRadians = FMath::Acos(DotProduct);
	float AngleInDegrees = FMath::RadiansToDegrees(AngleInRadians);

	if (AngleInDegrees > VisionConeAngle / 2.0f)
	{
		return false;
	}

	// Perform line trace to check for obstacles
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		HumanLocation,
		CatLocation,
		ECC_Visibility,
		QueryParams
	);

	// If we hit something, check if it's the cat
	if (bHit && HitResult.GetActor() == CatCharacter)
	{
		return true;
	}

	return !bHit; // If nothing blocked the view, we can see the cat
}

float AHumanNPC::GetEffectiveDetectionRadius() const
{
	float BaseRadius = DetectionRadius;

	// Apply alert level multiplier
	switch (CurrentAlertLevel)
	{
		case EAlertLevel::Suspicious:
			BaseRadius *= SuspiciousVisionMultiplier;
			break;
		case EAlertLevel::Alerted:
			BaseRadius *= AlertedVisionMultiplier;
			break;
		default:
			break;
	}

	// Apply stealth multiplier from cat
	if (CatCharacter)
	{
		BaseRadius *= CatCharacter->GetStealthDetectionMultiplier();
	}

	return BaseRadius;
}

// Hearing System
void AHumanNPC::OnNoiseHeard(FVector NoiseLocation, float NoiseLoudness, AActor* NoiseInstigator)
{
	if (!CanHearNoise(NoiseLocation, NoiseLoudness))
	{
		return;
	}

	// Increase suspicion based on noise loudness
	IncreaseSuspicion(NoiseLoudness);

	OnNoiseDetected(NoiseLocation);

	// If we're not chasing, investigate the noise
	if (CurrentState != EAIState::Chase)
	{
		StartInvestigation(NoiseLocation);
	}
}

bool AHumanNPC::CanHearNoise(FVector NoiseLocation, float NoiseLoudness) const
{
	float Distance = FVector::Dist(GetActorLocation(), NoiseLocation);
	float EffectiveHearingRadius = HearingRadius;

	// Adjust hearing radius based on noise loudness
	if (NoiseLoudness > 50.0f)
	{
		EffectiveHearingRadius *= LoudNoiseHearingMultiplier;
	}
	else if (NoiseLoudness < 20.0f)
	{
		EffectiveHearingRadius *= QuietNoiseHearingMultiplier;
	}

	return Distance <= EffectiveHearingRadius;
}

// State Machine
void AHumanNPC::SetAIState(EAIState NewState)
{
	if (CurrentState == NewState)
	{
		return;
	}

	CurrentState = NewState;
	OnStateChanged(NewState);

	// Reset state-specific timers
	switch (NewState)
	{
		case EAIState::Idle:
			IdleLookAroundTimer = 0.0f;
			break;
		case EAIState::Patrol:
			bWaitingAtPatrolPoint = false;
			PatrolWaitTimer = 0.0f;
			break;
		case EAIState::Investigate:
			InvestigationTimer = 0.0f;
			break;
		case EAIState::Chase:
			ChaseTimer = 0.0f;
			break;
	}
}

void AHumanNPC::SetAlertLevel(EAlertLevel NewLevel)
{
	if (CurrentAlertLevel == NewLevel)
	{
		return;
	}

	CurrentAlertLevel = NewLevel;
	OnAlertLevelChanged(NewLevel);
}

// Investigation
void AHumanNPC::StartInvestigation(FVector Location)
{
	LastKnownCatPosition = Location;
	bHasInvestigationTarget = true;
	SetAIState(EAIState::Investigate);
	OnInvestigationStarted(Location);
}

// Chase
void AHumanNPC::StartChase()
{
	SetAIState(EAIState::Chase);
	OnChaseStarted();
}

void AHumanNPC::StopChase()
{
	// Return to patrol or idle
	if (PatrolPoints.Num() > 0)
	{
		StartPatrol();
	}
	else
	{
		SetAIState(EAIState::Idle);
	}

	OnChaseEnded();
}

// Suspicion System
void AHumanNPC::IncreaseSuspicion(float Amount)
{
	CurrentSuspicion = FMath::Clamp(CurrentSuspicion + Amount, 0.0f, 100.0f);
	TimeSinceLastSuspiciousEvent = 0.0f;
	UpdateAlertLevel();
}

void AHumanNPC::DecreaseSuspicion(float Amount)
{
	CurrentSuspicion = FMath::Clamp(CurrentSuspicion - Amount, 0.0f, 100.0f);
	UpdateAlertLevel();
}

// Patrol System
void AHumanNPC::StartPatrol()
{
	if (PatrolPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot start patrol: No patrol points set"));
		SetAIState(EAIState::Idle);
		return;
	}

	SetAIState(EAIState::Patrol);
}

void AHumanNPC::StopPatrol()
{
	SetAIState(EAIState::Idle);
}

AActor* AHumanNPC::GetCurrentPatrolPoint() const
{
	if (PatrolPoints.IsValidIndex(CurrentPatrolIndex))
	{
		return PatrolPoints[CurrentPatrolIndex];
	}
	return nullptr;
}

// State Update Functions
void AHumanNPC::UpdateIdleState(float DeltaTime)
{
	// Periodically look around
	IdleLookAroundTimer += DeltaTime;

	if (IdleLookAroundTimer >= IdleLookAroundInterval)
	{
		IdleLookAroundTimer = 0.0f;

		// Random rotation within look around angle
		float RandomAngle = FMath::FRandRange(-IdleLookAroundAngle, IdleLookAroundAngle);
		FRotator NewRotation = OriginalRotation;
		NewRotation.Yaw += RandomAngle;

		SetActorRotation(FMath::RInterpTo(GetActorRotation(), NewRotation, DeltaTime, 2.0f));
	}
}

void AHumanNPC::UpdatePatrolState(float DeltaTime)
{
	AActor* CurrentPoint = GetCurrentPatrolPoint();
	if (!CurrentPoint)
	{
		SetAIState(EAIState::Idle);
		return;
	}

	if (bWaitingAtPatrolPoint)
	{
		PatrolWaitTimer += DeltaTime;

		if (PatrolWaitTimer >= PatrolWaitTime)
		{
			bWaitingAtPatrolPoint = false;
			PatrolWaitTimer = 0.0f;
			MoveToNextPatrolPoint();
		}
		return;
	}

	// Move towards current patrol point
	FVector TargetLocation = CurrentPoint->GetActorLocation();
	MoveTowards(TargetLocation, PatrolSpeed, DeltaTime);

	// Check if reached patrol point
	if (HasReachedPatrolPoint())
	{
		bWaitingAtPatrolPoint = true;
		PatrolWaitTimer = 0.0f;
	}
}

void AHumanNPC::UpdateInvestigateState(float DeltaTime)
{
	if (!bHasInvestigationTarget)
	{
		// No target, return to previous state
		if (PatrolPoints.Num() > 0)
		{
			StartPatrol();
		}
		else
		{
			SetAIState(EAIState::Idle);
		}
		return;
	}

	InvestigationTimer += DeltaTime;

	// Move towards investigation target
	MoveTowards(LastKnownCatPosition, InvestigationSpeed, DeltaTime);

	// Check if we've reached the investigation point
	float DistanceToTarget = FVector::Dist(GetActorLocation(), LastKnownCatPosition);
	
	if (DistanceToTarget <= InvestigationStopDistance || InvestigationTimer >= InvestigationDuration)
	{
		// Investigation complete
		bHasInvestigationTarget = false;
		InvestigationTimer = 0.0f;
		OnInvestigationComplete();

		// Return to patrol or idle
		if (PatrolPoints.Num() > 0)
		{
			StartPatrol();
		}
		else
		{
			SetAIState(EAIState::Idle);
		}
	}
}

void AHumanNPC::UpdateChaseState(float DeltaTime)
{
	if (!CatCharacter)
	{
		StopChase();
		return;
	}

	ChaseTimer += DeltaTime;

	FVector CatLocation = CatCharacter->GetActorLocation();
	float DistanceToCat = FVector::Dist(GetActorLocation(), CatLocation);

	// Give up if cat is too far or we've been chasing too long
	if (DistanceToCat > ChaseGiveUpDistance || ChaseTimer >= ChaseGiveUpTime)
	{
		StopChase();
		return;
	}

	// If we lost sight of the cat, investigate last known position
	if (!CanSeeCat())
	{
		StartInvestigation(LastKnownCatPosition);
		return;
	}

	// Chase the cat
	MoveTowards(CatLocation, ChaseSpeed, DeltaTime);
}

// Alert System Helpers
void AHumanNPC::UpdateAlertSystem(float DeltaTime)
{
	TimeSinceLastSuspiciousEvent += DeltaTime;

	// Decrease suspicion if enough time has passed since last event
	if (TimeSinceLastSuspiciousEvent >= CalmDownDelay)
	{
		DecreaseSuspicion(SuspicionDecreaseRate * DeltaTime);
	}
}

void AHumanNPC::UpdateAlertLevel()
{
	EAlertLevel NewLevel = EAlertLevel::Relaxed;

	if (CurrentSuspicion >= AlertThreshold)
	{
		NewLevel = EAlertLevel::Alerted;
	}
	else if (CurrentSuspicion >= SuspicionThreshold)
	{
		NewLevel = EAlertLevel::Suspicious;
	}

	SetAlertLevel(NewLevel);
}

// Patrol Helpers
void AHumanNPC::MoveToNextPatrolPoint()
{
	if (PatrolPoints.Num() == 0)
	{
		return;
	}

	if (bPatrolLooping)
	{
		// Loop through patrol points
		CurrentPatrolIndex = (CurrentPatrolIndex + 1) % PatrolPoints.Num();
	}
	else
	{
		// Ping-pong between patrol points
		if (bPatrolForward)
		{
			CurrentPatrolIndex++;
			if (CurrentPatrolIndex >= PatrolPoints.Num())
			{
				CurrentPatrolIndex = PatrolPoints.Num() - 2;
				bPatrolForward = false;
			}
		}
		else
		{
			CurrentPatrolIndex--;
			if (CurrentPatrolIndex < 0)
			{
				CurrentPatrolIndex = 1;
				bPatrolForward = true;
			}
		}
	}
}

bool AHumanNPC::HasReachedPatrolPoint() const
{
	AActor* CurrentPoint = GetCurrentPatrolPoint();
	if (!CurrentPoint)
	{
		return false;
	}

	float Distance = FVector::Dist(GetActorLocation(), CurrentPoint->GetActorLocation());
	return Distance <= PatrolPointReachedDistance;
}

// Movement Helpers
void AHumanNPC::MoveTowards(FVector TargetLocation, float Speed, float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();
	FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();

	// Rotate towards target
	RotateTowards(TargetLocation, DeltaTime);

	// Move towards target
	FVector NewLocation = CurrentLocation + (Direction * Speed * DeltaTime);
	SetActorLocation(NewLocation);
}

void AHumanNPC::RotateTowards(FVector TargetLocation, float DeltaTime)
{
	FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
	FRotator TargetRotation = Direction.Rotation();
	FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 5.0f);
	SetActorRotation(NewRotation);
}

// Debug Visualization
void AHumanNPC::DrawDebugVisionCone()
{
	if (!GetWorld())
	{
		return;
	}

	FVector HumanLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	float EffectiveRadius = GetEffectiveDetectionRadius();

	// Draw vision cone
	float HalfAngle = FMath::DegreesToRadians(VisionConeAngle / 2.0f);
	
	FVector RightBound = ForwardVector.RotateAngleAxis(VisionConeAngle / 2.0f, FVector::UpVector);
	FVector LeftBound = ForwardVector.RotateAngleAxis(-VisionConeAngle / 2.0f, FVector::UpVector);

	FColor ConeColor = FColor::Green;
	if (CurrentAlertLevel == EAlertLevel::Suspicious)
	{
		ConeColor = FColor::Yellow;
	}
	else if (CurrentAlertLevel == EAlertLevel::Alerted)
	{
		ConeColor = FColor::Red;
	}

	DrawDebugLine(GetWorld(), HumanLocation, HumanLocation + (RightBound * EffectiveRadius), ConeColor, false, -1.0f, 0, 2.0f);
	DrawDebugLine(GetWorld(), HumanLocation, HumanLocation + (LeftBound * EffectiveRadius), ConeColor, false, -1.0f, 0, 2.0f);
	DrawDebugLine(GetWorld(), HumanLocation, HumanLocation + (ForwardVector * EffectiveRadius), ConeColor, false, -1.0f, 0, 2.0f);

	// Draw arc
	DrawDebugCone(GetWorld(), HumanLocation, ForwardVector, EffectiveRadius, HalfAngle, HalfAngle, 16, ConeColor, false, -1.0f, 0, 1.0f);
}

void AHumanNPC::DrawDebugHearingRadius()
{
	if (!GetWorld())
	{
		return;
	}

	FVector HumanLocation = GetActorLocation();
	FColor HearingColor = FColor::Cyan;

	DrawDebugSphere(GetWorld(), HumanLocation, HearingRadius, 16, HearingColor, false, -1.0f, 0, 1.0f);
}
