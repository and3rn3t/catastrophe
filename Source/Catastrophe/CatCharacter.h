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

	// Stamina System
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float CurrentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaRegenRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaRegenDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float ClimbStaminaDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MinStaminaToSprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MinStaminaToClimb;

	// Stamina state tracking
	UPROPERTY()
	float TimeSinceStaminaUse;

	UPROPERTY()
	bool bStaminaDepleted;

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

	// Stealth/Crouch System
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stealth")
	float CrouchSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stealth")
	float CrouchCapsuleHalfHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stealth")
	float StandingCapsuleHalfHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stealth")
	float CrouchTransitionSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stealth")
	float StealthDetectionMultiplier;

	// Stealth state tracking
	UPROPERTY(BlueprintReadOnly, Category = "Stealth")
	bool bIsCrouching;

	UPROPERTY()
	bool bWantsToCrouch;

	UPROPERTY()
	float CurrentCapsuleHalfHeight;

	// Pounce/Attack System
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pounce")
	float PounceForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pounce")
	float PounceCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pounce")
	float PounceStaminaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pounce")
	float PounceRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pounce")
	float PounceAirControlMultiplier;

	// Pounce state tracking
	UPROPERTY(BlueprintReadOnly, Category = "Pounce")
	bool bIsPouncing;

	UPROPERTY()
	float PounceCooldownTimer;

	UPROPERTY()
	FVector PounceDirection;

	// Tail Physics System
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tail", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* TailBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tail")
	int32 TailSegmentCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tail")
	float TailSegmentLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tail")
	float TailStiffness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tail")
	float TailDamping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tail")
	float TailGravityScale;

	// Tail segment tracking
	UPROPERTY()
	TArray<FVector> TailSegmentPositions;

	UPROPERTY()
	TArray<FVector> TailSegmentVelocities;

	// Sound System
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* MeowSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* PurrSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* HissSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* LandingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* PounceSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	float MeowCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	float PurrInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	float PurrChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	bool bCanPlayMeow;

	// Sound state tracking
	UPROPERTY()
	float MeowCooldownTimer;

	UPROPERTY()
	float PurrTimer;

	UPROPERTY()
	bool bIsPurring;

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

	// Stealth/Crouch functions
	UFUNCTION(BlueprintCallable, Category = "Stealth")
	void ToggleCrouch();

	UFUNCTION(BlueprintCallable, Category = "Stealth")
	void StartCrouching();

	UFUNCTION(BlueprintCallable, Category = "Stealth")
	void StopCrouching();

	UFUNCTION(BlueprintPure, Category = "Stealth")
	bool IsCrouching() const { return bIsCrouching; }

	UFUNCTION(BlueprintPure, Category = "Stealth")
	float GetStealthDetectionMultiplier() const { return bIsCrouching ? StealthDetectionMultiplier : 1.0f; }

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

	// Stamina functions
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void ConsumeStamina(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void RegenerateStamina(float Amount);

	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetCurrentStamina() const { return CurrentStamina; }

	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetMaxStamina() const { return MaxStamina; }

	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetStaminaPercent() const { return MaxStamina > 0 ? CurrentStamina / MaxStamina : 0.0f; }

	UFUNCTION(BlueprintPure, Category = "Stamina")
	bool HasStamina(float Amount) const { return CurrentStamina >= Amount; }

	UFUNCTION(BlueprintPure, Category = "Stamina")
	bool IsStaminaDepleted() const { return bStaminaDepleted; }

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

	// Pounce/Attack functions
	UFUNCTION(BlueprintCallable, Category = "Pounce")
	void Pounce();

	UFUNCTION(BlueprintCallable, Category = "Pounce")
	bool CanPounce() const;

	UFUNCTION(BlueprintPure, Category = "Pounce")
	bool IsPouncing() const { return bIsPouncing; }

	UFUNCTION(BlueprintPure, Category = "Pounce")
	float GetPounceCooldownPercent() const { return PounceCooldown > 0 ? PounceCooldownTimer / PounceCooldown : 1.0f; }

	// Tail Physics (Blueprint events for animation)
	UFUNCTION(BlueprintImplementableEvent, Category = "Tail")
	void OnTailPositionsUpdated();

	UFUNCTION(BlueprintPure, Category = "Tail")
	TArray<FVector> GetTailSegmentPositions() const { return TailSegmentPositions; }

	// Sound functions
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayMeow();

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayPurr();

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void StopPurr();

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayHiss();

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayLandingSound();

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayPounceSound();

	UFUNCTION(BlueprintPure, Category = "Sound")
	bool IsPurring() const { return bIsPurring; }

private:
	// Helper functions for climbing
	void UpdateClimbing(float DeltaTime);
	void HandleClimbMovement(float DeltaTime);
	FVector GetClimbUpVector() const;

	// Helper functions for stamina
	void UpdateStamina(float DeltaTime);

	// Helper functions for crouching
	void UpdateCrouch(float DeltaTime);
	bool CanStandUp() const;

	// Helper functions for pounce
	void UpdatePounce(float DeltaTime);
	void HandlePounceLanding();

	// Helper functions for tail physics
	void InitializeTail();
	void UpdateTailPhysics(float DeltaTime);
	void SimulateTailSegment(int32 SegmentIndex, float DeltaTime);

	// Helper functions for sound system
	void UpdateSoundSystem(float DeltaTime);
	void TryPlayRandomPurr();
};
