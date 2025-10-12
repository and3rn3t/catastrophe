// CATastrophe - A Mischief Simulator

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CatastropheGameMode.generated.h"

/**
 * Main game mode for CATastrophe
 */
UCLASS()
class CATASTROPHE_API ACatastropheGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACatastropheGameMode();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called when the cat is caught
	UFUNCTION(BlueprintCallable, Category = "Game")
	void OnCatCaught();

	// Get total objects in level
	UFUNCTION(BlueprintPure, Category = "Game")
	int32 GetTotalDestructibleObjects() const;

protected:
	// Game state
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
	bool bGameOver;

	// HUD Widget Class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	// Current HUD Widget Instance
	UPROPERTY()
	class UUserWidget* HUDWidgetInstance;
};
