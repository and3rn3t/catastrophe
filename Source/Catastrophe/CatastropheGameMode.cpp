// CATastrophe - A Mischief Simulator

#include "CatastropheGameMode.h"
#include "CatCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ACatastropheGameMode::ACatastropheGameMode()
{
	// Set default pawn class to our cat character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_CatCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	bGameOver = false;
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
