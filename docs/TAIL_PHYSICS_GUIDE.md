# Tail Physics System - Implementation Guide

> **Status:** ✅ Complete
> **Version:** 1.0
> **Last Updated:** October 12, 2025

## Overview

The tail physics system simulates realistic cat tail movement using Verlet integration and constraint-based physics. The tail dynamically responds to character movement, gravity, and environmental forces.

## Features

### Core Mechanics

-   **Multi-Segment Simulation:** Tail divided into multiple connected segments
-   **Physics-Based Movement:** Uses Verlet integration for smooth, natural motion
-   **Dynamic Response:** Reacts to character movement, jumps, and direction changes
-   **Constraint System:** Maintains segment length and stiffness
-   **Gravity Influence:** Configurable gravity scale for tail droop
-   **Blueprint Integration:** Provides segment positions for visual representation

### Tail Properties

| Property            | Default Value | Description                           |
| ------------------- | ------------- | ------------------------------------- |
| `TailSegmentCount`  | 5             | Number of tail segments (bones)       |
| `TailSegmentLength` | 15.0          | Length of each segment in units       |
| `TailStiffness`     | 0.8           | How rigid the tail is (0.0-1.0)       |
| `TailDamping`       | 0.9           | Velocity damping factor (0.0-1.0)     |
| `TailGravityScale`  | 0.3           | Gravity multiplier for tail (0.0-1.0) |

## Physics Implementation

### Verlet Integration

The system uses Verlet integration for stable physics simulation:

```cpp
void SimulateTailSegment(int32 SegmentIndex, float DeltaTime)
{
    FVector& Position = TailSegmentPositions[SegmentIndex];
    FVector& Velocity = TailSegmentVelocities[SegmentIndex];

    // Apply forces
    FVector Gravity = FVector(0, 0, -980.0f) * TailGravityScale;
    Velocity += Gravity * DeltaTime;

    // Apply damping
    Velocity *= TailDamping;

    // Inherit character movement (tail follows body)
    if (SegmentIndex == 0)
    {
        FVector CharacterVelocity = GetVelocity();
        Velocity += CharacterVelocity * 0.1f * DeltaTime;
    }

    // Update position
    Position += Velocity * DeltaTime;
}
```

### Constraint Solving

Multi-iteration constraint solver maintains segment connections:

1. **First Segment:** Constrained to tail base (attached to character)
2. **Subsequent Segments:** Constrained to previous segment
3. **Length Constraint:** Each segment maintains fixed distance
4. **Stiffness Application:** Allows slight flexibility based on stiffness value

```cpp
// 3 iterations for stability
for (int32 Iteration = 0; Iteration < 3; ++Iteration)
{
    // Constrain first segment to tail base
    FVector Direction = TailSegmentPositions[0] - TailBasePosition;
    float Distance = Direction.Size();
    Direction /= Distance;
    TailSegmentPositions[0] = TailBasePosition + Direction * TailSegmentLength;

    // Constrain other segments to previous
    for (int32 i = 1; i < TailSegmentCount; ++i)
    {
        FVector ToNext = TailSegmentPositions[i] - TailSegmentPositions[i - 1];
        float SegmentDistance = ToNext.Size();
        ToNext /= SegmentDistance;

        float Offset = (SegmentDistance - TailSegmentLength) * TailStiffness;
        TailSegmentPositions[i] -= ToNext * Offset;
    }
}
```

## C++ Implementation

### Header File (`CatCharacter.h`)

```cpp
// Tail Physics System
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tail")
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
```

### Key Functions

#### `InitializeTail()`

Called in `BeginPlay()` to set up initial tail configuration:

-   Creates arrays for segment positions and velocities
-   Positions segments in line behind character
-   Initializes all velocities to zero

#### `UpdateTailPhysics(DeltaTime)`

Called every frame in `Tick()`:

1. Validates arrays are correct size
2. Simulates physics for each segment
3. Runs constraint solver (3 iterations)
4. Calls Blueprint event `OnTailPositionsUpdated()`

#### `SimulateTailSegment(int32 SegmentIndex, float DeltaTime)`

Physics simulation for individual segment:

-   Applies gravity (scaled by `TailGravityScale`)
-   Applies velocity damping
-   Inherits character velocity (first segment only)
-   Updates position based on velocity

#### `OnTailPositionsUpdated()` [Blueprint Event]

Called after physics update completes. Blueprints can override this to:

-   Update skeletal mesh bone transforms
-   Position visual tail mesh components
-   Trigger tail-based animations or effects

## Blueprint Integration

### Accessing Tail Data

```cpp
UFUNCTION(BlueprintPure, Category = "Tail")
TArray<FVector> GetTailSegmentPositions() const { return TailSegmentPositions; }

UFUNCTION(BlueprintImplementableEvent, Category = "Tail")
void OnTailPositionsUpdated();
```

### Visual Representation Methods

#### Method 1: Instanced Static Meshes

**Best for:** Simple tails, low overhead

```cpp
// In BP_CatCharacter Blueprint
Event OnTailPositionsUpdated()
{
    TArray<FVector> Positions = GetTailSegmentPositions();

    for (int i = 0; i < Positions.Length; i++)
    {
        TailSegmentMeshes[i].SetWorldLocation(Positions[i]);

        // Orient toward next segment
        if (i < Positions.Length - 1)
        {
            FVector Direction = Positions[i+1] - Positions[i];
            TailSegmentMeshes[i].SetWorldRotation(Direction.Rotation());
        }
    }
}
```

#### Method 2: Skeletal Mesh Bones

**Best for:** Animated tails, smooth deformation

```cpp
// In BP_CatCharacter Blueprint
Event OnTailPositionsUpdated()
{
    TArray<FVector> Positions = GetTailSegmentPositions();
    USkeletalMeshComponent* Mesh = GetMesh();

    for (int i = 0; i < Positions.Length; i++)
    {
        FName BoneName = FName("tail_" + FString::FromInt(i));
        Mesh->SetBoneLocationByName(BoneName, Positions[i], EBoneSpaces::WorldSpace);
    }
}
```

#### Method 3: Spline Component

**Best for:** Cable/rope-like tails, fur rendering

```cpp
// In BP_CatCharacter Blueprint
Event OnTailPositionsUpdated()
{
    TArray<FVector> Positions = GetTailSegmentPositions();

    TailSpline->ClearSplinePoints();

    for (int i = 0; i < Positions.Length; i++)
    {
        TailSpline->AddSplinePoint(Positions[i], ESplineCoordinateSpace::World);
    }

    TailSpline->UpdateSpline();
}
```

## Tuning Guide

### Tail Behavior Profiles

#### Realistic Cat Tail

```cpp
TailSegmentCount = 6
TailSegmentLength = 12.0
TailStiffness = 0.7
TailDamping = 0.88
TailGravityScale = 0.4
```

**Behavior:** Natural droop, responsive to movement, slight lag

#### Stiff/Alert Tail

```cpp
TailSegmentCount = 4
TailSegmentLength = 18.0
TailStiffness = 0.95
TailDamping = 0.75
TailGravityScale = 0.1
```

**Behavior:** Stays mostly upright, minimal sway, alert appearance

#### Floppy/Relaxed Tail

```cpp
TailSegmentCount = 8
TailSegmentLength = 10.0
TailStiffness = 0.4
TailDamping = 0.95
TailGravityScale = 0.6
```

**Behavior:** Heavy droop, exaggerated movement, lazy swaying

#### Energetic/Playful Tail

```cpp
TailSegmentCount = 5
TailSegmentLength = 15.0
TailStiffness = 0.6
TailDamping = 0.80
TailGravityScale = 0.3
```

**Behavior:** Bouncy, follows movement quickly, playful animation

### Parameter Effects

| Parameter          | Low Value (0.0-0.3)               | High Value (0.7-1.0)               |
| ------------------ | --------------------------------- | ---------------------------------- |
| `TailStiffness`    | Floppy, flexible, exaggerated     | Rigid, minimal bending, stiff      |
| `TailDamping`      | Wild oscillation, never settles   | Quick stabilization, minimal swing |
| `TailGravityScale` | Floaty, weightless, points upward | Heavy droop, sags downward         |

## Performance Considerations

### CPU Cost

-   **Per Frame:** ~0.01-0.05ms (typical)
-   **Segment Count:** Linear relationship (5 segments = 2x cost vs 10 segments)
-   **Iterations:** 3 constraint iterations balanced for quality/performance

### Optimization Tips

1. **Reduce Segment Count:** Fewer segments = lower cost

    - 3-4 segments: Simple tail, very fast
    - 5-7 segments: Balanced (recommended)
    - 8+ segments: High quality, higher cost

2. **Lower Update Frequency:**

    ```cpp
    // Update tail every other frame for 50% savings
    if (GetWorld()->TimeSeconds - LastTailUpdateTime > 0.033f)
    {
        UpdateTailPhysics(DeltaTime);
        LastTailUpdateTime = GetWorld()->TimeSeconds;
    }
    ```

3. **Distance-Based LOD:**
    ```cpp
    float DistanceToCamera = (GetActorLocation() - Camera->GetLocation()).Size();
    if (DistanceToCamera > 2000.0f) // Far away
    {
        TailSegmentCount = 3; // Reduce segments
    }
    ```

## Visual Enhancements

### Fur/Hair Rendering

Use tail skeleton with hair/fur system:

1. Create hair groom asset
2. Attach to tail spline or bone chain
3. Groom follows physics-driven skeleton
4. Add wind influence for extra movement

### Particle Effects

#### Tail Trail Effect

```cpp
// Spawn particles at tail tip during fast movement
if (GetVelocity().Size() > 600.0f)
{
    FVector TailTip = TailSegmentPositions.Last();
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TailTrailParticle, TailTip);
}
```

#### Pounce Tail Flare

```cpp
// Stiffen tail during pounce for dramatic effect
if (IsPouncing())
{
    TailStiffness = 0.95; // Temporarily rigid
    TailGravityScale = 0.1; // Point upward
}
else
{
    TailStiffness = 0.7; // Return to normal
    TailGravityScale = 0.4;
}
```

## Animation Integration

### Tail Emotion States

Use tail physics + animation curves to express emotion:

| Emotion | Tail Behavior                | Implementation                        |
| ------- | ---------------------------- | ------------------------------------- |
| Happy   | High, wagging side-to-side   | Increase stiffness, add lateral force |
| Scared  | Puffed up, rigid, low        | Max stiffness, high gravity           |
| Curious | Upright, slight curve at tip | Medium stiffness, low gravity         |
| Annoyed | Twitching, quick movements   | Low damping, oscillating forces       |
| Sleepy  | Limp, wrapped around body    | Very low stiffness, high gravity      |

### Blueprint Example: Tail Wag

```cpp
Event Tick(DeltaTime)
{
    if (bIsHappy)
    {
        // Add sinusoidal force to tail base for wagging
        float WagAmount = Sin(GetWorld()->TimeSeconds * 8.0) * 100.0;
        FVector LatForce = GetActorRightVector() * WagAmount;

        // Apply force to first tail segment
        TailSegmentVelocities[0] += LatForce * DeltaTime;
    }
}
```

## Debugging

### Visualization

Add debug spheres to see tail segments:

```cpp
void ACatCharacter::UpdateTailPhysics(float DeltaTime)
{
    // ... physics code ...

    // Debug visualization (remove in shipping build)
    #if !UE_BUILD_SHIPPING
    for (int32 i = 0; i < TailSegmentPositions.Num(); ++i)
    {
        FColor SegmentColor = (i == 0) ? FColor::Red : FColor::Yellow;
        DrawDebugSphere(GetWorld(), TailSegmentPositions[i], 5.0f, 8, SegmentColor, false, 0.0f);

        if (i > 0)
        {
            DrawDebugLine(GetWorld(),
                TailSegmentPositions[i-1],
                TailSegmentPositions[i],
                FColor::Green, false, 0.0f, 0, 2.0f);
        }
    }
    #endif
}
```

### Console Commands

```cpp
// Show tail physics debug
ShowDebug CatCharacter

// Log tail segment positions
stat CatTail
```

## Common Issues & Solutions

### Problem: Tail Jitters/Shakes

**Causes:**

-   Damping too low
-   Stiffness too high with rapid movement changes
-   DeltaTime variations

**Solutions:**

```cpp
TailDamping = 0.92; // Increase damping
TailStiffness = 0.6; // Reduce stiffness slightly
// Consider clamping DeltaTime for stability
```

### Problem: Tail Detaches/Flies Away

**Causes:**

-   Constraint iterations too few
-   Extremely high velocities
-   TailBase not properly attached

**Solutions:**

```cpp
// Increase constraint iterations
for (int32 Iteration = 0; Iteration < 5; ++Iteration) // Was 3

// Clamp velocities
Velocity = Velocity.GetClampedToMaxSize(1000.0f);

// Verify TailBase attachment in constructor
TailBase->SetupAttachment(GetMesh(), TEXT("TailSocket"));
```

### Problem: Tail Doesn't Move

**Causes:**

-   TailBase not positioned correctly
-   Gravity scale too low
-   Damping at 1.0 (kills all motion)

**Solutions:**

```cpp
// Check TailBase location
UE_LOG(LogTemp, Warning, TEXT("TailBase: %s"), *TailBase->GetComponentLocation().ToString());

// Increase gravity influence
TailGravityScale = 0.5;

// Reduce damping
TailDamping = 0.85;
```

## Future Enhancements

### Possible Additions

1. **Tail Collision:** Detect tail hitting objects, wrap around obstacles
2. **Wind Response:** React to wind zones or fast movement
3. **Muscular Control:** Allow manual tail animation override
4. **Tail Attack:** Use tail to swipe objects off tables
5. **Tail Grab:** Grab tail when grooming/biting
6. **Environmental Influence:** Tail reacts to water, fire, electricity

### Advanced Physics

#### Tail Collision Detection

```cpp
void CheckTailCollisions()
{
    for (int32 i = 0; i < TailSegmentPositions.Num(); ++i)
    {
        FHitResult Hit;
        FVector Start = TailSegmentPositions[i];
        FVector End = (i < TailSegmentCount - 1)
            ? TailSegmentPositions[i + 1]
            : TailSegmentPositions[i] + GetActorForwardVector() * TailSegmentLength;

        if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic))
        {
            // Adjust segment to avoid penetration
            TailSegmentPositions[i] = Hit.Location + Hit.Normal * 5.0f;
        }
    }
}
```

## Related Systems

-   **Character Movement:** Tail follows body velocity
-   **Animation System:** Tail physics can drive skeletal animations
-   **Camera System:** Tail visible in third-person view
-   **Pounce System:** Tail stiffens during pounce for dramatic effect

## Testing Checklist

-   [ ] Tail follows character movement smoothly
-   [ ] Tail droops naturally under gravity
-   [ ] Tail responds to direction changes (lag behind body)
-   [ ] Tail behaves correctly during jumps
-   [ ] Tail doesn't glitch through geometry
-   [ ] Tail segments maintain correct length
-   [ ] Performance acceptable (< 0.1ms per frame)
-   [ ] Tail visible in all camera angles
-   [ ] Parameters tunable in Blueprint editor
-   [ ] No console errors or warnings

## Asset Integration

### Skeletal Mesh Setup

1. **Create Tail Bones in DCC:**

    - Name: `tail_01`, `tail_02`, `tail_03`, etc.
    - Parent: Connect `tail_01` to spine/pelvis bone
    - Count: Match `TailSegmentCount` value

2. **Import to Unreal:**

    - Ensure skeleton includes tail bones
    - Verify bone hierarchy correct

3. **Blueprint Setup:**
    - In `BP_CatCharacter`, override `OnTailPositionsUpdated()`
    - Use `SetBoneLocationByName()` to drive tail bones

### Material Setup (Tail Fur)

-   Use fur/hair shader for realistic look
-   Add wind influence for extra movement
-   Consider vertex animation for fine detail

---

## See Also

-   [Pounce System Guide](POUNCE_SYSTEM_GUIDE.md)
-   [Animation Integration](DEV_TOOLS.md)
-   [Character Setup](CLIMBING_SYSTEM_GUIDE.md)
-   [Physics Debugging](BUILD_GUIDE.md)
