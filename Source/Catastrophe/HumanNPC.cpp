// CATastrophe - A Mischief Simulator

#include "HumanNPC.h"
#include "CatCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AHumanNPC::AHumanNPC()
{
 	// Set this character to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Default AI values
	DetectionRadius = 1000.0f;
	VisionConeAngle = 90.0f;
	bHasDetectedCat = false;
}

// Called when the game starts or when spawned
void AHumanNPC::BeginPlay()
{
	Super::BeginPlay();

	// Find the cat character in the world
	AActor* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	CatCharacter = Cast<ACatCharacter>(PlayerPawn);
}

// Called every frame
void AHumanNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CatCharacter)
	{
		bool bCanSee = CanSeeCat();

		if (bCanSee && !bHasDetectedCat)
		{
			// Cat is sprinting and in detection cone - catch them!
			if (CatCharacter->IsSprinting())
			{
				bHasDetectedCat = true;
				OnCatDetected();
				UE_LOG(LogTemp, Warning, TEXT("Human caught the cat sprinting!"));
			}
		}
		else if (!bCanSee && bHasDetectedCat)
		{
			bHasDetectedCat = false;
			OnCatEscaped();
		}

		// Debug visualization
		if (bCanSee)
		{
			DrawDebugLine(GetWorld(), GetActorLocation(), CatCharacter->GetActorLocation(),
				FColor::Red, false, -1.0f, 0, 2.0f);
		}
	}
}

bool AHumanNPC::CanSeeCat()
{
	if (!CatCharacter)
	{
		return false;
	}

	FVector HumanLocation = GetActorLocation();
	FVector CatLocation = CatCharacter->GetActorLocation();
	FVector DirectionToCat = (CatLocation - HumanLocation).GetSafeNormal();

	// Apply stealth multiplier to detection radius (crouching reduces detection range)
	float EffectiveDetectionRadius = DetectionRadius * CatCharacter->GetStealthDetectionMultiplier();

	// Check distance
	float Distance = FVector::Dist(HumanLocation, CatLocation);
	if (Distance > EffectiveDetectionRadius)
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
