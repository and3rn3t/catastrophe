// CATastrophe - A Mischief Simulator

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HumanNPC.generated.h"

/**
 * AI States for Human NPC behavior
 */
UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Patrol UMETA(DisplayName = "Patrol"),
	Investigate UMETA(DisplayName = "Investigate"),
	Chase UMETA(DisplayName = "Chase")
};

/**
 * Alert levels for Human NPC
 */
UENUM(BlueprintType)
enum class EAlertLevel : uint8
{
	Relaxed UMETA(DisplayName = "Relaxed"),
	Suspicious UMETA(DisplayName = "Suspicious"),
	Alerted UMETA(DisplayName = "Alerted")
};

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

	// Vision System
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Vision")
	float DetectionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Vision")
	float VisionConeAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Vision")
	float SuspiciousVisionMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Vision")
	float AlertedVisionMultiplier;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Vision")
	bool bHasDetectedCat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Vision")
	bool bShowVisionConeDebug;

	// Hearing System
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Hearing")
	float HearingRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Hearing")
	float LoudNoiseHearingMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Hearing")
	float QuietNoiseHearingMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Hearing")
	bool bShowHearingDebug;

	// State Machine
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|State")
	EAIState CurrentState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|State")
	EAlertLevel CurrentAlertLevel;

	// Investigation System
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Investigation")
	float InvestigationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Investigation")
	float InvestigationDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Investigation")
	float InvestigationStopDistance;

	UPROPERTY()
	FVector LastKnownCatPosition;

	UPROPERTY()
	float InvestigationTimer;

	UPROPERTY()
	bool bHasInvestigationTarget;

	// Chase System
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Chase")
	float ChaseSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Chase")
	float ChaseGiveUpDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Chase")
	float ChaseGiveUpTime;

	UPROPERTY()
	float ChaseTimer;

	// Alert System
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Alert")
	float SuspicionIncreaseRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Alert")
	float SuspicionDecreaseRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Alert")
	float SuspicionThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Alert")
	float AlertThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Alert")
	float CalmDownDelay;

	UPROPERTY()
	float CurrentSuspicion;

	UPROPERTY()
	float TimeSinceLastSuspiciousEvent;

	// Patrol System
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrol")
	float PatrolSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrol")
	float PatrolWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrol")
	float PatrolPointReachedDistance;

	UPROPERTY()
	int32 CurrentPatrolIndex;

	UPROPERTY()
	float PatrolWaitTimer;

	UPROPERTY()
	bool bWaitingAtPatrolPoint;

	UPROPERTY()
	bool bPatrolForward;

	// Idle System
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Idle")
	float IdleLookAroundInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Idle")
	float IdleLookAroundAngle;

	UPROPERTY()
	float IdleLookAroundTimer;

	UPROPERTY()
	FRotator OriginalRotation;

	// Reference to the cat character
	UPROPERTY()
	class ACatCharacter* CatCharacter;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Patrol System Configuration (public for easy access)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrol")
	TArray<AActor*> PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrol")
	bool bPatrolLooping;

	// Vision System
	UFUNCTION(BlueprintCallable, Category = "AI|Vision")
	bool CanSeeCat();

	UFUNCTION(BlueprintCallable, Category = "AI|Vision")
	float GetEffectiveDetectionRadius() const;

	// Hearing System
	UFUNCTION(BlueprintCallable, Category = "AI|Hearing")
	void OnNoiseHeard(FVector NoiseLocation, float NoiseLoudness, AActor* NoiseInstigator);

	UFUNCTION(BlueprintCallable, Category = "AI|Hearing")
	bool CanHearNoise(FVector NoiseLocation, float NoiseLoudness) const;

	// State Machine
	UFUNCTION(BlueprintCallable, Category = "AI|State")
	void SetAIState(EAIState NewState);

	UFUNCTION(BlueprintPure, Category = "AI|State")
	EAIState GetAIState() const { return CurrentState; }

	UFUNCTION(BlueprintCallable, Category = "AI|State")
	void SetAlertLevel(EAlertLevel NewLevel);

	UFUNCTION(BlueprintPure, Category = "AI|State")
	EAlertLevel GetAlertLevel() const { return CurrentAlertLevel; }

	// Investigation
	UFUNCTION(BlueprintCallable, Category = "AI|Investigation")
	void StartInvestigation(FVector Location);

	UFUNCTION(BlueprintPure, Category = "AI|Investigation")
	bool IsInvestigating() const { return CurrentState == EAIState::Investigate; }

	// Chase
	UFUNCTION(BlueprintCallable, Category = "AI|Chase")
	void StartChase();

	UFUNCTION(BlueprintCallable, Category = "AI|Chase")
	void StopChase();

	UFUNCTION(BlueprintPure, Category = "AI|Chase")
	bool IsChasing() const { return CurrentState == EAIState::Chase; }

	// Suspicion
	UFUNCTION(BlueprintCallable, Category = "AI|Alert")
	void IncreaseSuspicion(float Amount);

	UFUNCTION(BlueprintCallable, Category = "AI|Alert")
	void DecreaseSuspicion(float Amount);

	UFUNCTION(BlueprintPure, Category = "AI|Alert")
	float GetSuspicionLevel() const { return CurrentSuspicion; }

	UFUNCTION(BlueprintPure, Category = "AI|Alert")
	float GetSuspicionPercent() const { return FMath::Clamp(CurrentSuspicion / AlertThreshold, 0.0f, 1.0f); }

	// Patrol
	UFUNCTION(BlueprintCallable, Category = "AI|Patrol")
	void StartPatrol();

	UFUNCTION(BlueprintCallable, Category = "AI|Patrol")
	void StopPatrol();

	UFUNCTION(BlueprintPure, Category = "AI|Patrol")
	bool IsPatrolling() const { return CurrentState == EAIState::Patrol; }

	UFUNCTION(BlueprintCallable, Category = "AI|Patrol")
	AActor* GetCurrentPatrolPoint() const;

	UFUNCTION(BlueprintCallable, Category = "AI|Patrol")
	void MoveToNextPatrolPoint();

	UFUNCTION(BlueprintPure, Category = "AI|Patrol")
	int32 GetCurrentPatrolIndex() const { return CurrentPatrolIndex; }

	UFUNCTION(BlueprintCallable, Category = "AI|Patrol")
	void SetCurrentPatrolIndex(int32 NewIndex) { CurrentPatrolIndex = FMath::Clamp(NewIndex, 0, PatrolPoints.Num() - 1); }

	UFUNCTION(BlueprintPure, Category = "AI|Patrol")
	bool IsPatrolForward() const { return bPatrolForward; }

	UFUNCTION(BlueprintCallable, Category = "AI|Patrol")
	void SetPatrolForward(bool bForward) { bPatrolForward = bForward; }

	// Blueprint Events
	UFUNCTION(BlueprintImplementableEvent, Category = "AI|Events")
	void OnCatDetected();

	UFUNCTION(BlueprintImplementableEvent, Category = "AI|Events")
	void OnCatEscaped();

	UFUNCTION(BlueprintImplementableEvent, Category = "AI|Events")
	void OnStateChanged(EAIState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI|Events")
	void OnAlertLevelChanged(EAlertLevel NewLevel);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI|Events")
	void OnNoiseDetected(FVector NoiseLocation);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI|Events")
	void OnInvestigationStarted(FVector Location);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI|Events")
	void OnInvestigationComplete();

	UFUNCTION(BlueprintImplementableEvent, Category = "AI|Events")
	void OnChaseStarted();

	UFUNCTION(BlueprintImplementableEvent, Category = "AI|Events")
	void OnChaseEnded();

private:
	// State Update Functions
	void UpdateIdleState(float DeltaTime);
	void UpdatePatrolState(float DeltaTime);
	void UpdateInvestigateState(float DeltaTime);
	void UpdateChaseState(float DeltaTime);

	// Alert System Helpers
	void UpdateAlertSystem(float DeltaTime);
	void UpdateAlertLevel();

	// Patrol Helpers
	bool HasReachedPatrolPoint() const;

	// Movement Helpers
	void MoveTowards(FVector TargetLocation, float Speed, float DeltaTime);
	void RotateTowards(FVector TargetLocation, float DeltaTime);

	// Debug Visualization
	void DrawDebugVisionCone();
	void DrawDebugHearingRadius();
};
