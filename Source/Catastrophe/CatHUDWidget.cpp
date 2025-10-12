// CATastrophe - HUD Widget Implementation

#include "CatHUDWidget.h"
#include "CatCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

void UCatHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Cache player controller
	CachedPlayerController = GetOwningPlayer();

	// Try to find the cat character
	if (CachedPlayerController)
	{
		CachedCatCharacter = Cast<ACatCharacter>(CachedPlayerController->GetPawn());
	}

	// Verify all required widgets are bound
	if (!ScoreText)
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD: ScoreText widget not found! Make sure widget is named 'ScoreText'"));
	}
	if (!StaminaBar)
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD: StaminaBar widget not found! Make sure widget is named 'StaminaBar'"));
	}
	if (!StealthIcon)
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD: StealthIcon widget not found! Make sure widget is named 'StealthIcon'"));
	}
	if (!StealthStatusText)
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD: StealthStatusText widget not found! Make sure widget is named 'StealthStatusText'"));
	}
}

void UCatHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Update cached reference if needed
	if (!CachedCatCharacter && CachedPlayerController)
	{
		CachedCatCharacter = Cast<ACatCharacter>(CachedPlayerController->GetPawn());
	}

	// Auto-update HUD every frame
	if (CachedCatCharacter)
	{
		UpdateScoreDisplay(CachedCatCharacter->GetMischiefScore());
		UpdateStaminaDisplay(CachedCatCharacter->GetStaminaPercent());
		UpdateStealthDisplay(CachedCatCharacter->IsDetectedByNPC(), CachedCatCharacter->IsCrouching());
	}
}

void UCatHUDWidget::UpdateScoreDisplay(float Score)
{
	if (ScoreText)
	{
		FText ScoreDisplayText = FText::Format(
			FText::FromString(TEXT("Score: {0}")),
			FText::AsNumber(FMath::RoundToInt(Score))
		);
		ScoreText->SetText(ScoreDisplayText);
	}
}

void UCatHUDWidget::UpdateStaminaDisplay(float StaminaPercent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(StaminaPercent);

		// Change color based on stamina level
		FLinearColor BarColor;
		if (StaminaPercent < 0.3f)
		{
			BarColor = FLinearColor::Red;
		}
		else if (StaminaPercent < 0.6f)
		{
			BarColor = FLinearColor::Yellow;
		}
		else
		{
			BarColor = FLinearColor::Green;
		}
		StaminaBar->SetFillColorAndOpacity(BarColor);
	}
}

void UCatHUDWidget::UpdateStealthDisplay(bool bIsDetected, bool bIsCrouching)
{
	FLinearColor IconColor;
	FText StatusText;

	if (bIsDetected)
	{
		// Detected - Red
		IconColor = FLinearColor::Red;
		StatusText = FText::FromString(TEXT("DETECTED!"));
	}
	else if (bIsCrouching)
	{
		// Crouching/Sneaking - Blue
		IconColor = FLinearColor(0.0f, 0.5f, 1.0f, 1.0f); // Light Blue
		StatusText = FText::FromString(TEXT("SNEAKING"));
	}
	else
	{
		// Hidden - Green
		IconColor = FLinearColor::Green;
		StatusText = FText::FromString(TEXT("HIDDEN"));
	}

	// Apply to widgets
	if (StealthIcon)
	{
		StealthIcon->SetColorAndOpacity(IconColor);
	}

	if (StealthStatusText)
	{
		StealthStatusText->SetText(StatusText);
		StealthStatusText->SetColorAndOpacity(IconColor);
	}
}
