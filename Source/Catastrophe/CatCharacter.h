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

	// Enhanced Jump Mechanics
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float MaxJumpHoldTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float MinJumpVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float MaxJumpVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float CoyoteTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float JumpBufferTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float ImprovedAirControl;

	// Jump state tracking
	UPROPERTY()
	bool bJumpHeld;

	UPROPERTY()
	float JumpHoldTime;

	UPROPERTY()
	float TimeSinceLeftGround;

	UPROPERTY()
	float JumpBufferTimer;

	UPROPERTY()
	bool bWasGroundedLastFrame;

	// Climbing System
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing")
	float ClimbSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing")
	float ClimbDetectionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing")
	float ClimbCapsuleHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing")
	float ClimbCapsuleRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing")
	bool bCanClimbJump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing")
	float ClimbJumpVelocity;

	// Climbing state tracking
	UPROPERTY(BlueprintReadOnly, Category = "Climbing")
	bool bIsClimbing;

	UPROPERTY()
	AActor* CurrentClimbableSurface;

	UPROPERTY()
	FVector ClimbSurfaceNormal;

	UPROPERTY()
	float ClimbHeightOnSurface;

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

	// Enhanced Jump functions
	virtual void Jump() override;
	virtual void StopJumping() override;

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

	// Climbing functions
	UFUNCTION(BlueprintCallable, Category = "Climbing")
	void StartClimbing();

	UFUNCTION(BlueprintCallable, Category = "Climbing")
	void StopClimbing();

	UFUNCTION(BlueprintCallable, Category = "Climbing")
	bool CanClimb();

	UFUNCTION(BlueprintCallable, Category = "Climbing")
	AActor* DetectClimbableSurface();

	UFUNCTION(BlueprintPure, Category = "Climbing")
	bool IsClimbing() const { return bIsClimbing; }

private:
	// Helper functions for climbing
	void UpdateClimbing(float DeltaTime);
	void HandleClimbMovement(float DeltaTime);
	FVector GetClimbUpVector() const;
};
