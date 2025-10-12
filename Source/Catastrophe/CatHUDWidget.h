// CATastrophe - HUD Widget for displaying game information

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "CatHUDWidget.generated.h"

/**
 * Main HUD widget that displays score, stamina, and stealth status
 * Automatically updates all UI elements - no Blueprint overrides needed!
 */
UCLASS()
class CATASTROPHE_API UCatHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Widget references (bound automatically by name matching)
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ScoreText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* StaminaBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* StealthIcon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* StealthStatusText;

	// Optional: Stamina label
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	class UTextBlock* StaminaLabel;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// Reference to the cat character (cached)
	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	class ACatCharacter* CachedCatCharacter;

	// Cached reference to player controller
	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	class APlayerController* CachedPlayerController;

	// Auto-update functions (called automatically)
	void UpdateScoreDisplay(float Score);
	void UpdateStaminaDisplay(float StaminaPercent);
	void UpdateStealthDisplay(bool bIsDetected, bool bIsCrouching);
};
