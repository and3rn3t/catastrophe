// CATastrophe - A Mischief Simulator

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HumanNPC.generated.h"

/**
 * Human NPC that patrols and watches for the cat
 */
UCLASS()
class CATASTROPHE_API AHumanNPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHumanNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Detection radius
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float DetectionRadius;

	// Vision cone angle (in degrees)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float VisionConeAngle;

	// Whether the human has detected the cat
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	bool bHasDetectedCat;

	// Reference to the cat character
	UPROPERTY()
	class ACatCharacter* CatCharacter;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Check if the cat is in detection range
	UFUNCTION(BlueprintCallable, Category = "AI")
	bool CanSeeCat();

	// Called when cat is detected
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnCatDetected();

	// Called when cat escapes detection
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnCatEscaped();
};
