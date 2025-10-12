# Cat Sound System - Implementation Guide

> **Status:** ✅ Complete
> **Version:** 1.0
> **Last Updated:** October 12, 2025

## Overview

The cat sound system adds personality and feedback through context-aware vocalizations. The system includes meows, purrs, hisses, and action-based sounds with intelligent triggering and cooldown management.

## Features

### Core Sound Types

-   **Meow** - Player-triggered vocalization with cooldown
-   **Purr** - Automatic ambient sound when idle (looping)
-   **Hiss** - Defensive/aggressive sound (Blueprint-triggered)
-   **Landing Sound** - Plays on pounce landing
-   **Pounce Sound** - Plays when initiating pounce attack

### Smart Sound Management

-   **Cooldown System** - Prevents meow spam (3 second default)
-   **Random Purring** - 30% chance to purr when idle every 5 seconds
-   **Context-Aware** - Purring auto-stops when moving
-   **Action Integration** - Sounds automatically play for pounce/landing

## Sound Properties

| Property       | Default Value | Description                      |
| -------------- | ------------- | -------------------------------- |
| `MeowSound`    | nullptr       | USoundBase\* - Set in Blueprint  |
| `PurrSound`    | nullptr       | USoundBase\* - Should be looping |
| `HissSound`    | nullptr       | USoundBase\* - Defensive sound   |
| `LandingSound` | nullptr       | USoundBase\* - Pounce landing    |
| `PounceSound`  | nullptr       | USoundBase\* - Pounce attack     |
| `MeowCooldown` | 3.0f          | Seconds between meows            |
| `PurrInterval` | 5.0f          | Check for purr every N seconds   |
| `PurrChance`   | 0.3f          | 30% chance to purr when idle     |

## Input Bindings

| Platform | Key Binding                     |
| -------- | ------------------------------- |
| Keyboard | **M** or **Right Mouse Button** |
| Gamepad  | **Y Button** (Face Button Top)  |

## C++ Implementation

### Header File (`CatCharacter.h`)

```cpp
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
```

### Key Functions

#### `PlayMeow()`

Player-triggered meow vocalization:

1. Checks cooldown and permission
2. Plays sound at character location
3. Starts 3-second cooldown
4. Logs to console

```cpp
void ACatCharacter::PlayMeow()
{
    if (!bCanPlayMeow || MeowCooldownTimer > 0.0f)
        return;

    if (MeowSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, MeowSound, GetActorLocation());
        UE_LOG(LogTemp, Log, TEXT("Cat says: Meow!"));
    }

    MeowCooldownTimer = MeowCooldown;
    bCanPlayMeow = false;
}
```

#### `PlayPurr()` / `StopPurr()`

Ambient purring when cat is idle:

-   **PlayPurr()** - Starts looping purr sound, sets `bIsPurring = true`
-   **StopPurr()** - Stops purr, sets `bIsPurring = false`
-   Auto-managed by `TryPlayRandomPurr()`

#### `UpdateSoundSystem(DeltaTime)`

Called every frame:

1. Decrements meow cooldown timer
2. Increments purr timer
3. When purr interval reached, calls `TryPlayRandomPurr()`

#### `TryPlayRandomPurr()`

Intelligent purr triggering:

**Idle Conditions:**

-   Velocity < 50 units/second
-   On ground
-   Not climbing
-   Not pouncing

**Logic:**

-   If idle and not purring: 30% random chance to start purring
-   If not idle and purring: Stop purring

### Integration with Existing Systems

#### Pounce System Integration

```cpp
// In Pounce() function
PlayPounceSound(); // Added at end of pounce execution
```

#### Landing System Integration

```cpp
// In HandlePounceLanding() function
PlayLandingSound(); // Added after landing impact
```

## Blueprint Setup

### 1. Import Sound Assets

**Recommended Formats:**

-   **WAV** - Uncompressed, best quality (use for short sounds)
-   **OGG** - Compressed, smaller file size (use for longer sounds)

**Suggested Sources:**

-   Freesound.org
-   BBC Sound Effects Library
-   Record your own cat!

### 2. Create Sound Cues (Optional)

For variation and polish:

1. Right-click in Content Browser → Sounds → Sound Cue
2. Add multiple sound waves
3. Use Random node for variation
4. Add attenuation for 3D positioning

### 3. Assign Sounds to BP_CatCharacter

1. Open `BP_CatCharacter` Blueprint
2. In Class Defaults, find "Sound" category
3. Assign sound assets:
    - **Meow Sound** → Cat meow sound cue
    - **Purr Sound** → Looping purr (enable looping in sound asset!)
    - **Hiss Sound** → Aggressive hiss
    - **Landing Sound** → Soft thud
    - **Pounce Sound** → Attack whoosh/growl

### 4. Configure Sound Settings

Tune in Blueprint Class Defaults:

```cpp
MeowCooldown = 3.0        // How often can meow
PurrInterval = 5.0        // Check for purr frequency
PurrChance = 0.3          // 30% chance when idle
```

### 5. Enable Looping for Purr

**Critical:** Purr sound must loop!

1. Open purr sound asset
2. Check "Looping" checkbox
3. Save asset

## Sound Design Guidelines

### Meow Sound

**Purpose:** Player expression, interaction feedback

**Characteristics:**

-   Duration: 0.5-1.5 seconds
-   Pitch: Medium-high
-   Variations: Multiple meow types (happy, demanding, questioning)
-   Volume: Medium

**Recommended Types:**

-   Short chirp meow (friendly)
-   Long demanding meow (attention)
-   Questioning meow (curiosity)

### Purr Sound

**Purpose:** Ambient personality, idle state indicator

**Characteristics:**

-   Duration: 2-5 seconds (loops seamlessly)
-   Pitch: Low, rumbling
-   Volume: Low-medium
-   **Must be looping**

**Implementation Tips:**

-   Use seamless loop (check start/end match)
-   Add slight volume envelope for smooth transitions
-   Low-pass filter for warmth

### Hiss Sound

**Purpose:** Defensive/aggressive state (future NPC interaction)

**Characteristics:**

-   Duration: 1-2 seconds
-   Pitch: High, sharp
-   Volume: High
-   Intensity: Aggressive

**Use Cases (Blueprint):**

-   When spotted by NPC (future feature)
-   When cornered
-   When defending territory

### Landing Sound

**Purpose:** Physical feedback for pounce landing

**Characteristics:**

-   Duration: 0.2-0.5 seconds
-   Pitch: Low
-   Volume: Medium (scales with landing velocity)
-   Type: Soft thud, paw impact

### Pounce Sound

**Purpose:** Attack initiation feedback

**Characteristics:**

-   Duration: 0.3-0.8 seconds
-   Pitch: Medium-low
-   Volume: Medium-high
-   Type: Whoosh + growl combination

**Layering Suggestion:**

-   Base: Whoosh/swish sound (movement)
-   Layer: Quiet growl/grunt (effort)

## Advanced Features

### Volume Scaling by Context

Add to Blueprint or C++:

```cpp
// Scale landing sound by velocity
float LandingVolume = FMath::Clamp(GetVelocity().Z / -1000.0f, 0.3f, 1.0f);
// Play with scaled volume
```

### Pitch Variation

Add randomness to prevent repetition:

```cpp
// In Blueprint or C++
float RandomPitch = FMath::RandRange(0.9f, 1.1f);
// Apply to sound instance
```

### 3D Sound Attenuation

Create **Sound Attenuation** asset:

1. Content Browser → Sounds → Sound Attenuation
2. Set attenuation distance (500-2000 units)
3. Assign to sound cues
4. Adjust falloff curve for realism

### Reverb Zones

Add environmental audio:

-   Kitchen: Slight reverb (tile floors)
-   Living room: Minimal reverb (carpet)
-   Hallway: Medium reverb (hard surfaces)

## UI Integration

### Meow Cooldown Indicator

Display cooldown in HUD:

```cpp
// In Widget Blueprint
float CooldownPercent = 1.0 - (CatCharacter->MeowCooldownTimer / CatCharacter->MeowCooldown);
// Show as progress bar or icon alpha
```

### Purring Visual

Add purr indicator icon:

```cpp
// Show when IsPurring() returns true
if (CatCharacter->IsPurring())
{
    // Display purr icon/animation
}
```

## Performance Considerations

### Sound Instance Limits

-   **Max Concurrent Sounds:** ~32 (engine default)
-   **This system uses:** 1-2 concurrent (meow or purr)
-   **No performance concerns**

### Memory Usage

| Sound Type  | Typical Size | Compressed  |
| ----------- | ------------ | ----------- |
| Meow        | ~100 KB      | ~30 KB      |
| Purr (loop) | ~200 KB      | ~50 KB      |
| Hiss        | ~150 KB      | ~40 KB      |
| Landing     | ~50 KB       | ~15 KB      |
| Pounce      | ~80 KB       | ~25 KB      |
| **Total**   | **~580 KB**  | **~160 KB** |

**Conclusion:** Negligible memory footprint

## Debugging

### Enable Sound Logging

```cpp
// Console commands
stat SoundWaves    // Show active sounds
au.Debug.Sounds 1  // Visual sound debug
```

### Console Output

System logs sound events:

```
LogTemp: Cat says: Meow!
LogTemp: Cat is purring...
LogTemp: Cat stopped purring
LogTemp: Cat pounce sound!
LogTemp: Cat landed with a thump
```

### Common Issues

**Problem:** Meow doesn't play
**Solutions:**

-   Check MeowSound is assigned in Blueprint
-   Verify cooldown timer = 0
-   Check audio volume settings
-   Ensure `bCanPlayMeow = true`

**Problem:** Purr doesn't loop
**Solutions:**

-   Enable "Looping" in sound asset
-   Check sound duration is > 0
-   Verify seamless loop (no clicks/pops)

**Problem:** Too much purring
**Solutions:**

-   Reduce `PurrChance` (0.3 → 0.15)
-   Increase `PurrInterval` (5.0 → 10.0)
-   Tighten idle velocity threshold (50 → 20)

**Problem:** No sound at all
**Solutions:**

-   Check master volume in editor
-   Verify sound assets imported correctly
-   Check audio device output
-   Look for console warnings

## Future Enhancements

### Contextual Meows

Different meows for different situations:

-   **Curious Meow:** When discovering new area
-   **Happy Meow:** After successful mischief
-   **Demanding Meow:** When near food/objective
-   **Scared Meow:** When spotted by NPC

**Implementation:**

```cpp
void PlayContextualMeow(EMeowType MeowType);
```

### Emotional Purr States

Purr intensity based on situation:

-   **Content Purr:** Normal idle
-   **Happy Purr:** After scoring points
-   **Nervous Purr:** When hiding from NPC

### Vocal Combos

Chain sounds for expressions:

-   Meow → Purr (satisfied)
-   Hiss → Growl (escalating aggression)
-   Chirp → Meow (excitement)

### Dynamic Mixing

Adjust volumes based on context:

-   Lower purr when meowing
-   Silence ambient when hissing
-   Boost action sounds during pounce

### Multiplayer Support

If adding multiplayer:

-   Replicate meow calls
-   Sync purring state
-   Attenuate by distance between players

## Animation Integration

### Meow Animation

**Recommendation:** Mouth open animation

```cpp
// In Animation Blueprint
if (PlayingMeowSound)
{
    PlayMontage("Meow_Mouth_Open");
}
```

### Purr Animation

**Recommendation:** Subtle chest vibration

-   Skeletal mesh animation
-   Slight breathing animation
-   Optional: Eyes half-closed

### Hiss Animation

**Recommendation:** Aggressive posture

-   Ears back
-   Teeth showing
-   Arched back
-   Puffed tail

## Related Systems

-   **Pounce System:** Automatically plays pounce sound
-   **Landing System:** Automatically plays landing sound
-   **Stealth System:** Could trigger hiss when detected (future)
-   **Scoring System:** Could play happy meow on high score (future)
-   **NPC System:** Hiss when spotted (future AI integration)

## Testing Checklist

-   [ ] Meow plays when pressing M or RMB
-   [ ] Meow cooldown prevents spam (3 seconds)
-   [ ] Purr starts when idle for 5+ seconds
-   [ ] Purr stops when starting to move
-   [ ] Purr has ~30% chance to trigger
-   [ ] Pounce sound plays on pounce action
-   [ ] Landing sound plays on pounce landing
-   [ ] All sound assets assigned in Blueprint
-   [ ] Purr sound is set to looping
-   [ ] Console logs show sound events
-   [ ] Gamepad input works correctly
-   [ ] No audio clipping or distortion

## Asset Requirements Summary

**To complete sound system, you need:**

1. **Meow sound effect** (0.5-1.5s, cat meow)
2. **Purr sound effect** (2-5s, looping, rumbling purr)
3. **Hiss sound effect** (1-2s, aggressive hiss)
4. **Landing sound** (0.2-0.5s, soft thud)
5. **Pounce sound** (0.3-0.8s, whoosh/growl)

**Where to find:**

-   Freesound.org (search "cat meow", "cat purr", etc.)
-   BBC Sound Effects (royalty-free)
-   Record your own cat (best for authenticity!)
-   Asset stores (Unreal Marketplace, Unity Asset Store)

**File format:** WAV or OGG, 44.1kHz, mono or stereo

---

## See Also

-   [Pounce System Guide](POUNCE_SYSTEM_GUIDE.md) - Integration with pounce sounds
-   [Milestone 1.1 Complete](MILESTONE_1.1_COMPLETE.md) - Final milestone documentation
-   [Current Sprint](CURRENT_SPRINT.md) - Development progress
