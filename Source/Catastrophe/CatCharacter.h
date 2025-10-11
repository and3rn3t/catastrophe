// CATastrophe - A Mischief Simulator

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CatCharacter.generated.h"

/**
 * The playable cat character that causes mischief around the house
 */
UCLASS()
class CATASTROPHE_API ACatCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACatCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Camera boom positioning the camera behind the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// Follow camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	// Cat stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cat Stats")
	float MischiefScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cat Stats")
	bool bIsSprinting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cat Stats")
	float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cat Stats")
	float WalkSpeed;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Movement functions
	void MoveForward(float Value);
	void MoveRight(float Value);
	void StartSprinting();
	void StopSprinting();

	// Interaction function
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Interact();

	// Add mischief score
	UFUNCTION(BlueprintCallable, Category = "Cat Stats")
	void AddMischiefScore(float Points);

	// Get mischief score
	UFUNCTION(BlueprintPure, Category = "Cat Stats")
	float GetMischiefScore() const { return MischiefScore; }

	// Check if sprinting
	UFUNCTION(BlueprintPure, Category = "Cat Stats")
	bool IsSprinting() const { return bIsSprinting; }
};
