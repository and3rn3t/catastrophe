// CATastrophe - A Mischief Simulator

#include "CatastropheGameMode.h"
#include "CatCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

ACatastropheGameMode::ACatastropheGameMode()
{
	// Set default pawn class to our cat character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_CatCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	bGameOver = false;
	HUDWidgetInstance = nullptr;
}

void ACatastropheGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Create and add HUD widget to viewport
	if (HUDWidgetClass)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			HUDWidgetInstance = CreateWidget<UUserWidget>(PC, HUDWidgetClass);
			if (HUDWidgetInstance)
			{
				HUDWidgetInstance->AddToViewport();
				UE_LOG(LogTemp, Log, TEXT("HUD Widget created and added to viewport"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HUDWidgetClass not set in GameMode!"));
	}
}

void ACatastropheGameMode::OnCatCaught()
{
	if (!bGameOver)
	{
		bGameOver = true;
		UE_LOG(LogTemp, Warning, TEXT("Game Over! Cat was caught!"));
		
		// Get the cat character and log final score
		ACatCharacter* Cat = Cast<ACatCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if (Cat)
		{
			UE_LOG(LogTemp, Warning, TEXT("Final Mischief Score: %.0f"), Cat->GetMischiefScore());
		}
	}
}

int32 ACatastropheGameMode::GetTotalDestructibleObjects() const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Destructible"), FoundActors);
	return FoundActors.Num();
}
