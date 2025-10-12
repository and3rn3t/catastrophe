# Jump Mechanics Testing Guide

**Date:** October 11, 2025
**Feature:** Enhanced Jump Mechanics (Milestone 1.1)
**Status:** Ready to Test

---

## 🎮 How to Test in Unreal Editor

### Step 1: Launch Play in Editor (PIE)

Once the editor opens:

1. Press **Alt+P** (or click the green Play button in toolbar)
2. Your view should switch to game mode
3. Use WASD to move around

### Step 2: Test Variable Jump Height

**Test A: Tap Jump (Short Hop)**

-   **Action:** Quickly tap and release **Space**
-   **Expected:** Cat does a short jump (~1-2 meters high)
-   **Technical:** Uses `MinJumpVelocity = 400.0f`

**Test B: Hold Jump (High Jump)**

-   **Action:** Press and hold **Space** for ~0.3 seconds
-   **Expected:** Cat jumps much higher (~3-4 meters)
-   **Technical:** Ramps up to `MaxJumpVelocity = 800.0f`

**Test C: Early Release**

-   **Action:** Start holding Space, release halfway through jump
-   **Expected:** Jump cuts short immediately, cat starts falling
-   **Technical:** `StopJumping()` reduces velocity by 50%

### Step 3: Test Coyote Time

**Test D: Walk Off Edge**

-   **Action:** Walk off a platform edge, press Space **within 150ms** of leaving edge
-   **Expected:** Cat still executes the jump (doesn't fall)
-   **Why:** Gives 150ms grace period for edge jumps
-   **Technical:** `CoyoteTime = 0.15f`

**How to verify:**

1. Walk slowly toward edge
2. As soon as you leave the ground, press Space
3. If you jump successfully = coyote time working!

### Step 4: Test Jump Buffering

**Test E: Pre-Landing Jump**

-   **Action:** While falling, press Space **before landing**
-   **Expected:** Cat jumps immediately upon touching ground (no delay)
-   **Why:** Buffers jump input for 100ms
-   **Technical:** `JumpBufferTime = 0.1f`

**How to verify:**

1. Jump off something high
2. While falling, tap Space
3. Cat should jump the instant it lands (responsive!)

### Step 5: Test Air Control

**Test F: Mid-Air Movement**

-   **Action:** Jump, then press WASD while in air
-   **Expected:** Cat responds to movement input, can steer mid-jump
-   **Compare:** Should feel MORE responsive than default UE character
-   **Technical:** `ImprovedAirControl = 0.5f` (vs default 0.2f)

---

## 🐛 What to Watch For

### Issues to Report:

**Jump Feels Wrong:**

-   Too floaty? → Decrease `MaxJumpVelocity` in BP_CatCharacter
-   Too stiff? → Increase `AirControl`
-   Not high enough? → Increase `MaxJumpVelocity`

**Coyote Time Not Working:**

-   Should jump within 150ms of leaving edge
-   If not working, check console for `UE_LOG` messages

**Jump Buffering Not Working:**

-   Should auto-jump on landing if Space pressed while falling
-   Look for "Jump buffered" message in output log

**Controls Feel Unresponsive:**

-   Check if sprint is on (Shift)
-   Verify input bindings in Edit → Project Settings → Input

---

## 📊 Debug Console Commands

Press **`** (backtick) in PIE to open console:

```
stat FPS              # Show frame rate
stat Unit             # Show performance breakdown
stat Game             # Show game thread time
ShowDebug AI          # Show NPC vision cones (if NPCs exist)
```

**To see UE_LOG messages:**

-   Window → Developer Tools → Output Log
-   Look for lines starting with "CATastrophe:"

---

## 🎯 Expected Behavior Summary

| Test              | Input                    | Expected Result      |
| ----------------- | ------------------------ | -------------------- |
| **Short Jump**    | Tap Space                | ~1-2m high jump      |
| **High Jump**     | Hold Space 0.3s          | ~3-4m high jump      |
| **Early Release** | Release during jump      | Jump cuts short      |
| **Coyote Time**   | Space after leaving edge | Still jumps          |
| **Jump Buffer**   | Space while falling      | Instant jump on land |
| **Air Control**   | WASD in air              | Responsive steering  |

---

## 🐱 First Time Testing?

### If You Don't Have BP_CatCharacter Yet:

**The C++ code works, but you need a Blueprint to see it!**

**Quick Setup:**

1. Content Browser → Right-click in `Content/Blueprints/`
2. Blueprint Class → Search "CatCharacter"
3. Name it `BP_CatCharacter`
4. Open it, add a **Capsule Component** (visual representation)
5. Compile & Save
6. World Settings → GameMode Override → Create `BP_GameMode`
7. Set Default Pawn Class to `BP_CatCharacter`

**Or test with default character:**

-   The jump code works on any `ACharacter` subclass
-   You can test with UE's default third-person character
-   But custom cat character needed for full game experience

---

## 📝 Testing Checklist

-   [ ] Editor opened successfully
-   [ ] Pressed Alt+P to play
-   [ ] Can move with WASD
-   [ ] **Tap jump** → Short hop works
-   [ ] **Hold jump** → High jump works
-   [ ] **Early release** → Jump cuts short
-   [ ] **Walk off edge + jump** → Coyote time works
-   [ ] **Jump while falling** → Buffer works
-   [ ] **WASD in air** → Air control responsive
-   [ ] No errors in Output Log
-   [ ] Performance feels smooth

---

## 🎮 Controls Reference

| Key        | Action          |
| ---------- | --------------- |
| **WASD**   | Move            |
| **Mouse**  | Look around     |
| **Space**  | Jump (tap/hold) |
| **Shift**  | Sprint          |
| **E**      | Interact        |
| **Escape** | Exit PIE        |
| **Alt+P**  | Toggle PIE      |

---

## 🚀 Next Steps After Testing

**If everything works:**

1. ✅ Mark "Jump Mechanics" complete in CURRENT_SPRINT.md
2. 🎯 Move to next feature (Climbing, Stamina, or Crouch)
3. 🎨 Consider adding jump animations

**If issues found:**

1. Note specific problems
2. Check console/log for errors
3. Adjust values in BP_CatCharacter (no recompile needed!)
4. Test again

**Tuning Parameters:**
All values in `BP_CatCharacter` → Class Defaults → Jump category:

-   `MaxJumpHoldTime` - How long to hold for max height
-   `MinJumpVelocity` - Short jump power
-   `MaxJumpVelocity` - High jump power
-   `CoyoteTime` - Edge forgiveness window
-   `JumpBufferTime` - Pre-land input buffer
-   `ImprovedAirControl` - Mid-air responsiveness

---

**Ready to test!** 🎉

Open Unreal Editor (should be launching now) and press Alt+P to play!
