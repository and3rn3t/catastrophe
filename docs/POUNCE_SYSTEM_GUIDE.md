# Pounce/Attack System - Implementation Guide

> **Status:** ✅ Complete
> **Version:** 1.0
> **Last Updated:** October 12, 2025

## Overview

The pounce system allows the cat to perform a directional dash/attack that launches the character forward in an arc, dealing extra damage to objects and providing bonus mischief points.

## Features

### Core Mechanics

-   **Directional Launch:** Launches cat in forward direction with upward arc
-   **Cooldown System:** 1.5 second cooldown between pounces
-   **Stamina Cost:** Consumes 15 stamina per pounce
-   **Enhanced Air Control:** Increased mid-air control during pounce (80% more)
-   **Range Detection:** 200 unit range for detecting pounce targets
-   **Landing Impact:** Applies downward force when landing on objects

### Pounce Properties

| Property                     | Default Value | Description                           |
| ---------------------------- | ------------- | ------------------------------------- |
| `PounceForce`                | 1200.0        | Launch force strength                 |
| `PounceCooldown`             | 1.5s          | Time between pounces                  |
| `PounceStaminaCost`          | 15.0          | Stamina consumed per pounce           |
| `PounceRange`                | 200.0         | Detection range for targets           |
| `PounceAirControlMultiplier` | 0.8           | Extra air control during pounce (80%) |

## Input Bindings

| Platform | Key Binding                    |
| -------- | ------------------------------ |
| Keyboard | **F** or **Left Mouse Button** |
| Gamepad  | **Right Trigger**              |

## Scoring System

Pouncing provides bonus points over regular interaction:

| Object Type | Regular Points | Pounce Bonus | Total Points |
| ----------- | -------------- | ------------ | ------------ |
| Vase        | 10             | +5           | **15**       |
| Furniture   | 15             | +5           | **20**       |
| Curtain     | 20             | +5           | **25**       |
| Food        | 25             | +5           | **30**       |

## C++ Implementation

### Header File (`CatCharacter.h`)

```cpp
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
```

### Key Functions

#### `Pounce()`

Executes the pounce attack:

1. Checks if pounce is available (`CanPounce()`)
2. Consumes stamina
3. Calculates pounce direction (forward + upward arc)
4. Launches character with `LaunchCharacter()`
5. Detects objects in path and applies bonus impulse
6. Awards bonus mischief points
7. Starts cooldown timer

#### `CanPounce()`

Validates pounce conditions:

-   Cooldown timer must be 0
-   Must have sufficient stamina (15 points)
-   Cannot be already pouncing
-   Must be grounded or within coyote time window
-   Cannot be climbing or crouching

#### `UpdatePounce(DeltaTime)`

Called every frame:

-   Decrements cooldown timer
-   Detects landing and calls `HandlePounceLanding()`

#### `HandlePounceLanding()`

Handles landing logic:

-   Resets `bIsPouncing` flag
-   Restores normal air control
-   Checks for objects underneath
-   Applies downward impact force on destructible objects

## Blueprint Integration

### Exposed Properties

All pounce properties are `BlueprintReadWrite`, allowing Blueprint children to tune values in the editor:

-   Force strength
-   Cooldown duration
-   Stamina cost
-   Detection range
-   Air control modifier

### Blueprint Functions

```cpp
UFUNCTION(BlueprintCallable, Category = "Pounce")
void Pounce();

UFUNCTION(BlueprintCallable, Category = "Pounce")
bool CanPounce() const;

UFUNCTION(BlueprintPure, Category = "Pounce")
bool IsPouncing() const;

UFUNCTION(BlueprintPure, Category = "Pounce")
float GetPounceCooldownPercent() const;
```

### UI Integration Example

Use `GetPounceCooldownPercent()` to display cooldown progress bar:

```cpp
// In Blueprint widget
float CooldownPercent = CatCharacter->GetPounceCooldownPercent();
// 0.0 = ready, 1.0 = on cooldown
```

## Physics Interactions

### Object Impact

When pouncing toward objects:

-   **Detection:** Sphere sweep from start to end of pounce path
-   **Sphere Radius:** 50 units
-   **Impulse Multiplier:** 1.5x normal interaction force
-   **Direction:** Away from cat toward object

### Landing Impact

When landing on destructible objects:

-   **Detection:** Line trace downward 100 units
-   **Force Direction:** Straight down
-   **Impact Strength:** 0.5x pounce force
-   **Application:** Applied at landing point (localized damage)

## Restrictions

Pounce is **disabled** when:

-   🚫 Already pouncing (prevents spam)
-   🚫 Cooldown active (1.5 seconds)
-   🚫 Insufficient stamina (< 15 points)
-   🚫 Climbing wall
-   🚫 Crouching/stealth mode
-   🚫 In air too long (past coyote time)

## Animation Integration (Blueprint)

### Recommended Animation Setup

1. **Pounce Start Animation:**

    - Trigger: `IsPouncing() == true` AND velocity.Z > 0
    - Duration: ~0.3 seconds
    - Cat crouches then springs forward

2. **Pounce Mid-Air Animation:**

    - Trigger: `IsPouncing() == true` AND in air
    - Loop until landing
    - Cat extends paws forward, tail back

3. **Pounce Landing Animation:**
    - Trigger: `IsPouncing()` transitions from true to false
    - Duration: ~0.2 seconds
    - Impact pose, slight crouch recovery

### Animation Blueprint Pseudocode

```cpp
// State machine
if (IsPouncing && IsGrounded)
{
    PlayAnimation("Pounce_Start");
}
else if (IsPouncing && !IsGrounded)
{
    PlayAnimation("Pounce_Air");
}
else if (!IsPouncing && WasPouncingLastFrame && IsGrounded)
{
    PlayAnimation("Pounce_Landing");
}
```

## Visual Effects (Blueprint)

### Recommended VFX

1. **Pounce Trail:**

    - Particle system attached to cat during pounce
    - Swoosh/motion blur effect
    - Color: White/orange

2. **Landing Dust:**

    - Spawn on `HandlePounceLanding()`
    - Small dust cloud at landing point
    - Scale based on landing velocity

3. **Impact Sparks:**
    - Spawn when hitting destructible objects
    - Location: Impact point from sweep hit result
    - Color coded by object type

## Audio Integration (Future)

### Suggested Sound Effects

| Event                  | Sound Description    | Volume | Pitch Variation |
| ---------------------- | -------------------- | ------ | --------------- |
| Pounce Start           | Cat growl/yowl       | 0.8    | ±10%            |
| Pounce Whoosh          | Wind/movement sound  | 0.6    | ±5%             |
| Object Impact (Pounce) | Louder crash/thud    | 1.0    | ±15%            |
| Landing                | Thump/paw impact     | 0.7    | ±10%            |
| Cooldown Ready         | Subtle "ready" chirp | 0.4    | None            |

## Debugging

### Debug Visualizations

The pounce system includes debug draws:

-   **Red Line:** Shows pounce trajectory (visible for 0.5s)
-   **Red Sphere:** Detection range for targets

### Console Commands

```cpp
// Enable debug logging for pounce
Log LogTemp Warning

// Check in output log:
"Cat pounced!"
"Pounced on: [ActorName]! Bonus points!"
"Pounce landing complete"
```

### Common Issues

**Problem:** Pounce doesn't work
**Solutions:**

-   Check stamina is above 15
-   Ensure not climbing/crouching
-   Verify cooldown timer is 0
-   Confirm input binding exists

**Problem:** Pounce feels weak
**Solutions:**

-   Increase `PounceForce` (default 1200)
-   Adjust arc by modifying forward/up ratio in `Pounce()` function
-   Check character mass (heavier = less distance)

**Problem:** Cooldown too long/short
**Solutions:**

-   Adjust `PounceCooldown` in Blueprint defaults
-   Balance with stamina cost for fair gameplay

## Performance Considerations

-   **Sweep Queries:** One multi-sweep per pounce (acceptable)
-   **Line Traces:** One trace per landing (negligible)
-   **Update Cost:** Minimal (only updates cooldown timer)
-   **No Physics:** Does not continuously simulate; uses impulses

## Future Enhancements

### Possible Additions

1. **Charged Pounce:** Hold button longer for more powerful pounce
2. **Directional Pounce:** Pounce left/right/backward based on input
3. **Combo System:** Multiple pounces in succession with diminishing stamina
4. **Pounce Meter:** Visual meter showing pounce readiness
5. **Wall Pounce:** Pounce off walls for extra height/distance
6. **Target Lock:** Auto-aim toward nearest destructible object

### Integration Ideas

-   **Stealth Takedown:** Pounce from stealth for extra bonus
-   **NPC Stun:** Pounce near NPCs to briefly stun them
-   **Environmental Interactions:** Pounce to trigger switches/buttons
-   **Achievement System:** Track total pounces, perfect pounces, etc.

## Related Systems

-   **Stamina System:** Manages pounce cost and recovery
-   **Jump System:** Shares coyote time logic
-   **Interaction System:** Enhanced version for object destruction
-   **Scoring System:** Awards bonus points for pouncing

## Testing Checklist

-   [ ] Pounce launches cat in arc trajectory
-   [ ] Cooldown prevents immediate re-use
-   [ ] Stamina drains correctly (15 points)
-   [ ] Bonus points awarded for hitting objects
-   [ ] Objects receive stronger impulse
-   [ ] Landing impact damages objects underneath
-   [ ] Cannot pounce while climbing
-   [ ] Cannot pounce while crouching
-   [ ] Air control feels responsive
-   [ ] Cooldown timer updates properly
-   [ ] Visual feedback clear to player
-   [ ] Gamepad input works correctly

## Code Example: Custom Pounce Variation

### Blueprint Example: Stealth Pounce Bonus

```cpp
// Override in BP_CatCharacter
Event Pounce()
{
    if (IsCrouching())
    {
        // Can't pounce while crouching normally,
        // but could allow "stealth pounce" variant
        StopCrouching();
        // Wait brief moment, then pounce with bonus
    }

    Super::Pounce(); // Call parent implementation

    if (WasCrouching)
    {
        // Award stealth pounce bonus
        AddMischiefScore(10); // Extra 10 points!
        PlaySound("StealthPounceSting");
    }
}
```

---

## See Also

-   [Stamina System Guide](STAMINA_SYSTEM_GUIDE.md)
-   [Jump Mechanics Guide](MILESTONE_1.1_JUMP_COMPLETE.md)
-   [Climbing System Guide](CLIMBING_SYSTEM_GUIDE.md)
-   [Stealth System Guide](STEALTH_CROUCH_GUIDE.md)
