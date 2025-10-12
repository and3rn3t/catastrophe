# ⚡ Sprint Stamina System - Implementation Guide

**Date Implemented:** October 11, 2025
**Developer:** and3rn3t
**Milestone:** 1.1 - Enhanced Cat Mechanics
**Status:** ✅ Implemented & Ready for Testing

---

## 🎯 Overview

The **Sprint Stamina System** adds resource management to movement, creating tactical gameplay decisions. Players must balance speed vs endurance, adding depth to chases, escapes, and exploration.

**Key Features:**

-   ✅ **Stamina pool** - 100 points (configurable)
-   ✅ **Sprint drains stamina** - 20 points/sec while sprinting
-   ✅ **Climb drains stamina** - 10 points/sec while climbing
-   ✅ **Auto-regeneration** - 15 points/sec after 2 second delay
-   ✅ **Stamina gates** - Can't sprint/climb when depleted
-   ✅ **Smooth recovery** - Gradual regen, not instant
-   ✅ **Blueprint-exposed** - All parameters tunable

---

## 🎮 Player Experience

### **How It Works:**

| Action                   | Stamina Cost | Effect                     |
| ------------------------ | ------------ | -------------------------- |
| **Walking**              | None         | Stamina regenerates        |
| **Idle/Standing**        | None         | Stamina regenerates faster |
| **Sprinting** (Shift)    | 20/sec       | Drains stamina             |
| **Climbing** (W on wall) | 10/sec       | Drains stamina             |
| **Jumping**              | None         | No stamina cost            |

### **Stamina States:**

**🟢 Full Stamina (100%):**

-   Can sprint freely
-   Can climb freely
-   Ready for action

**🟡 Low Stamina (< 20%):**

-   Sprint/climb still work
-   Risk of depletion
-   Should consider slowing down

**🔴 Depleted (0%):**

-   Sprint automatically stops
-   Can't start new sprint
-   Walking only until recovery
-   Climbing stops if stamina < 10

**🔄 Recovering:**

-   Stop sprinting/climbing
-   Wait 2 seconds (StaminaRegenDelay)
-   Stamina regenerates at 15/sec
-   Can sprint again once > 5 stamina

---

## 🔧 Technical Implementation

### **Core Properties (Blueprint-Exposed)**

```cpp
// Stamina Pool
MaxStamina = 100.0f;                 // Total stamina capacity
CurrentStamina = 100.0f;             // Current stamina (starts full)

// Drain Rates
StaminaDrainRate = 20.0f;            // Sprint cost (per second)
ClimbStaminaDrainRate = 10.0f;       // Climb cost (per second)

// Regeneration
StaminaRegenRate = 15.0f;            // Regen speed (per second)
StaminaRegenDelay = 2.0f;            // Delay before regen starts

// Thresholds
MinStaminaToSprint = 5.0f;           // Min stamina to start sprint
MinStaminaToClimb = 10.0f;           // Min stamina to start climb
```

### **State Variables**

```cpp
float TimeSinceStaminaUse;           // Track time since last drain
bool bStaminaDepleted;               // Flag: fully depleted?
```

---

## 📊 Stamina Math & Balance

### **Duration Calculations:**

**Sprint Duration (from full):**

-   Time = MaxStamina / StaminaDrainRate
-   Time = 100 / 20 = **5 seconds** of continuous sprint

**Climb Duration (from full):**

-   Time = MaxStamina / ClimbStaminaDrainRate
-   Time = 100 / 10 = **10 seconds** of continuous climb

**Recovery Time (from empty):**

-   Time = MaxStamina / StaminaRegenRate (after delay)
-   Time = 100 / 15 = **6.67 seconds** + 2s delay = **~8.7 seconds total**

### **Balanced Gameplay Loop:**

1. **Sprint for ~5 seconds** (escape NPC or rush to objective)
2. **Stamina depletes** (forced to walk)
3. **Wait 2 seconds** (regen delay - tactical decision)
4. **Recover for ~7 seconds** (vulnerable period)
5. **Ready to sprint again**

**Total cycle:** ~14-15 seconds (sprint → recover → ready)

---

## 🎨 Visual Feedback (Recommended)

### **HUD Stamina Bar (Blueprint Widget):**

**Create Widget Blueprint: `WBP_StaminaBar`**

```cpp
// In Update Tick:
float StaminaPercent = CatCharacter->GetStaminaPercent();

// Progress Bar:
StaminaBar->SetPercent(StaminaPercent);

// Color coding:
if (StaminaPercent > 0.5f)
    StaminaBar->SetFillColor(Green);  // Good
else if (StaminaPercent > 0.2f)
    StaminaBar->SetFillColor(Yellow); // Low
else
    StaminaBar->SetFillColor(Red);    // Critical

// Hide when full (optional):
if (StaminaPercent >= 1.0f)
    StaminaBar->SetVisibility(Hidden);
```

### **Audio Cues:**

| Event               | Sound                | Description   |
| ------------------- | -------------------- | ------------- |
| Stamina Low (< 20%) | `SFX_HeartbeatFast`  | Warning sound |
| Stamina Depleted    | `SFX_Exhausted_Meow` | Cat is tired  |
| Sprint Blocked      | `SFX_ErrorBeep`      | Can't sprint  |
| Stamina Recovered   | `SFX_EnergyRecharge` | Ready again   |

### **Visual Effects:**

**Camera Effects:**

-   Low stamina (< 20%): Slight vignette + desaturation
-   Depleted: Screen edges pulse red briefly
-   Sprinting: Subtle motion blur + FOV increase

**Character Effects:**

-   Low stamina: Breathing particles (panting)
-   Depleted: Character slows, stumbles slightly
-   Recovering: Glow effect or aura

---

## 🧪 Testing the Stamina System

### **Quick Test (5 Minutes):**

1. **Open Unreal Editor** (if closed: `start Catastrophe.uproject`)
2. **Open test level** (or create new: File → New Level → Default)
3. **Press Alt+P** to play
4. **Test sprint stamina:**

    - Hold **Shift** and **W** to sprint
    - Watch Output Log for "Cat is sprinting!"
    - Sprint for 5+ seconds until depleted
    - See "Stamina depleted! Need to rest." message
    - Sprint stops automatically
    - Wait 2 seconds, stamina regenerates
    - Try sprinting again once recovered

5. **Test climb stamina:**
    - Create climbable wall (cube with "Climbable" tag)
    - Climb for 10+ seconds
    - Should stop with "Too tired to keep climbing!"
    - Recover and try again

### **Advanced Testing:**

**Test 1: Sprint → Climb Combo**

-   Sprint for 3 seconds (60 stamina left)
-   Immediately start climbing
-   Climb for 6 seconds (uses remaining 60)
-   Should stop climbing when depleted

**Test 2: Regen Delay**

-   Sprint until depleted
-   Try sprinting immediately → Blocked
-   Walk for 1 second → Still can't sprint
-   Wait 2+ seconds → Can sprint again

**Test 3: Low Stamina Sprint**

-   Sprint until 10 stamina left
-   Stop sprinting
-   Wait for regen to ~6 stamina
-   Try sprinting → Should work (min is 5)

---

## ⚙️ Tuning Guide

### **Making Sprint Last Longer:**

**Option 1: Increase Max Stamina**

```cpp
MaxStamina = 150.0f;  // +50% duration (7.5 seconds)
```

**Option 2: Reduce Drain Rate**

```cpp
StaminaDrainRate = 15.0f;  // -25% drain (6.67 seconds)
```

**Option 3: Both**

```cpp
MaxStamina = 120.0f;
StaminaDrainRate = 15.0f;  // 8 seconds of sprint
```

### **Making Recovery Faster:**

**Option 1: Reduce Delay**

```cpp
StaminaRegenDelay = 1.0f;  // Only 1s wait
```

**Option 2: Increase Regen Rate**

```cpp
StaminaRegenRate = 25.0f;  // Recover in 4 seconds
```

**Option 3: Both**

```cpp
StaminaRegenDelay = 1.0f;
StaminaRegenRate = 20.0f;  // Fast recovery (6s total)
```

### **Difficulty Tuning:**

**Easy Mode (Casual):**

```cpp
MaxStamina = 150.0f;
StaminaDrainRate = 15.0f;
StaminaRegenRate = 25.0f;
StaminaRegenDelay = 1.0f;
// Result: 10s sprint, 7s recovery
```

**Normal Mode (Balanced):**

```cpp
MaxStamina = 100.0f;
StaminaDrainRate = 20.0f;
StaminaRegenRate = 15.0f;
StaminaRegenDelay = 2.0f;
// Result: 5s sprint, 8.7s recovery (default)
```

**Hard Mode (Tactical):**

```cpp
MaxStamina = 75.0f;
StaminaDrainRate = 25.0f;
StaminaRegenRate = 10.0f;
StaminaRegenDelay = 3.0f;
// Result: 3s sprint, 10.5s recovery
```

---

## 🎯 Gameplay Integration

### **Level Design Opportunities:**

**1. Chase Sequences:**

-   Human NPC chases cat
-   Cat must manage stamina to escape
-   Safe spots for stamina recovery
-   Risk vs reward: sprint past or hide?

**2. Timed Challenges:**

-   Reach objective before stamina depletes
-   Multiple short sprints vs one long sprint
-   Stamina pickups (future: food items restore stamina)

**3. Vertical Exploration:**

-   Tall climbs require stamina management
-   Rest platforms mid-climb
-   Choose route: fast climb (risky) or slow climb (safe)

**4. Stealth Segments:**

-   Sprint reveals position (future: sound system)
-   Walking preserves stamina for emergency escape
-   Tactical decision: sneak or rush?

### **Difficulty Scaling:**

**Easy Difficulty:**

-   More stamina, slower drain
-   NPCs slower/less persistent
-   More recovery opportunities

**Hard Difficulty:**

-   Less stamina, faster drain
-   NPCs faster/more persistent
-   Fewer safe spots

---

## 🔗 Integration with Other Systems

### **Jump System:**

✅ **Compatible** - Jumping has no stamina cost

-   Jumping while low stamina still works
-   Consider: Add small stamina cost to wall jumps?

### **Climbing System:**

✅ **Integrated** - Climbing drains stamina

-   Climb = 10/sec (half of sprint cost)
-   Can't start climb below 10 stamina
-   Auto-dismount when stamina too low

### **Mischief Scoring (Future):**

-   Bonus points for completing objectives with low stamina?
-   Risk vs reward: fast (uses stamina) vs careful (slower)

### **Stealth System (Future):**

-   Sprinting makes more noise
-   Low stamina = heavy breathing (more detectable)
-   Stealth mode uses less stamina

### **NPC AI (Future):**

-   NPCs track if cat is tired
-   More aggressive when cat has low stamina
-   Can't escape as easily when depleted

---

## 📊 Blueprint Functions (For UI/Scripting)

**Available for Blueprint use:**

```cpp
// Getters
GetCurrentStamina()          // Returns float (0-100)
GetMaxStamina()              // Returns float (100 default)
GetStaminaPercent()          // Returns 0.0-1.0 for progress bars
HasStamina(Amount)           // Check if >= Amount
IsStaminaDepleted()          // Returns bool

// Modifiers
ConsumeStamina(Amount)       // Drain stamina
RegenerateStamina(Amount)    // Restore stamina (for pickups)
```

**Example Blueprint Usage:**

```
// HUD Update:
Event Tick
  → Get Player Character
  → Cast to CatCharacter
  → Get Stamina Percent
  → Set Progress Bar Percent

// Stamina Pickup:
On Overlap Begin
  → Cast to CatCharacter
  → Regenerate Stamina (50.0)
  → Play Sound
  → Destroy Pickup
```

---

## 🐛 Known Limitations & Future Enhancements

### **Current Limitations:**

1. **No Visual Indicator** - Stamina only shown in logs (HUD needed)
2. **No Stamina Pickups** - Can only recover by waiting
3. **No Sprint Speed Scaling** - Constant speed until depleted
4. **No Climbing Fatigue Visual** - No animation tells you you're tired

### **Future Enhancements (Post-MVP):**

-   [ ] **HUD Stamina Bar** - Visual progress bar
-   [ ] **Stamina Pickups** - Food items restore stamina
-   [ ] **Fatigue System** - Repeated sprints reduce max stamina temporarily
-   [ ] **Sprint Speed Scaling** - Gradually slow down as stamina depletes
-   [ ] **Climb Fatigue Animation** - Grip weakens at low stamina
-   [ ] **Sound Feedback** - Panting sounds when low stamina
-   [ ] **Camera Effects** - Vignette/blur at low stamina
-   [ ] **Difficulty Settings** - Easy/Normal/Hard presets
-   [ ] **Stamina Upgrades** - Unlock higher max stamina
-   [ ] **Activity-Based Costs** - Different actions cost different amounts

---

## 🧩 Code Quality Notes

**Strengths:**

-   ✅ Clean separation: drain, regen, and delay logic
-   ✅ Automatic integration with sprint/climb
-   ✅ All parameters Blueprint-exposed
-   ✅ Smooth regeneration (gradual, not instant)
-   ✅ Depletion state prevents spam
-   ✅ Well-commented implementation

**Design Patterns:**

-   **Resource Pool Pattern** - Track current/max values
-   **Cooldown System** - Regen delay prevents instant recovery
-   **State Flags** - bStaminaDepleted prevents edge cases
-   **Delta Time Scaling** - Frame-rate independent

---

## 📚 Related Files

**Implementation:**

-   `Source/Catastrophe/CatCharacter.h` - 10 properties, 7 functions
-   `Source/Catastrophe/CatCharacter.cpp` - UpdateStamina(), ConsumeStamina(), RegenerateStamina()

**Documentation:**

-   `STAMINA_SYSTEM_GUIDE.md` - This file
-   `CHANGELOG_STAMINA_SYSTEM.md` - Implementation changelog
-   `CURRENT_SPRINT.md` - Milestone progress

---

## ✅ Testing Checklist

Before marking complete:

-   [ ] Sprint drains stamina over time
-   [ ] Sprint stops at 0 stamina
-   [ ] Can't sprint when depleted
-   [ ] Stamina regenerates after delay
-   [ ] Climbing drains stamina
-   [ ] Climbing stops at low stamina
-   [ ] Output log shows stamina messages
-   [ ] No crashes or errors
-   [ ] Parameters tunable in BP_CatCharacter
-   [ ] Smooth transitions (no stuttering)

---

## 🚀 Next Steps

### **Immediate (Testing Phase):**

1. Test sprint until depletion
2. Verify regen delay works
3. Test climbing stamina cost
4. Tune parameters if needed

### **Short-Term (Polish):**

1. Create HUD stamina bar widget
2. Add audio feedback (panting, depletion)
3. Add camera effects (vignette at low stamina)
4. Tune values based on playtesting

### **Medium-Term (Gameplay):**

1. Design levels with stamina in mind
2. Add stamina pickups (food items)
3. Balance NPC chase sequences
4. Create stamina-based challenges

---

**Status:** ✅ Code Complete - Ready for Testing
**Build Status:** ✅ Compiled Successfully
**Next Feature:** Crouching/Stealth Mode or Tail Physics

---

**Implementation Time:** ~1 hour
**Lines of Code:** ~120 lines (30 .h, 90 .cpp)
**Blueprint-Exposed Properties:** 8 configurable parameters

---

⚡ **Stamina system complete! Strategic movement awaits!** 🐱💨
