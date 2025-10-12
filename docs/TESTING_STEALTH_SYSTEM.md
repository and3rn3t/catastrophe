# Testing the Stealth/Crouch System

## Quick Test Guide

This guide walks you through testing the newly implemented crouch/stealth mechanics in Unreal Engine 5.

**Implementation Date:** October 12, 2025
**Milestone:** 1.1 - Enhanced Cat Mechanics

---

## Prerequisites

### 1. Build the C++ Code

```powershell
# In VS Code (preferred)
Ctrl+Shift+B → Select "Build Catastrophe (Development Editor)"

# Or via terminal
& "C:\Program Files\Epic Games\UE_5.6\Engine\Build\BatchFiles\Build.bat" `
  CatastropheEditor Win64 Development "c:\git\catastrophe\Catastrophe.uproject"
```

**Expected Result:** Build succeeds with no errors ✅

### 2. Open Unreal Editor

```powershell
# Double-click or run from terminal
Start-Process "c:\git\catastrophe\Catastrophe.uproject"
```

**Wait for:** Hot reload prompt (if editor was already open) or full editor load

---

## Test Suite 1: Basic Crouch Functionality

### Test 1.1: Toggle Crouch (Standing → Crouching)

**Steps:**

1. Click Play (Alt+P) in UE5 Editor
2. Ensure you can see the cat character
3. Press **C** key

**Expected Results:**

-   ✅ Cat's capsule height visibly reduces
-   ✅ Smooth transition (not instant snap)
-   ✅ Console log: "Cat is crouching (stealth mode)"
-   ✅ Movement feels slower

**Debugging:**

-   Open console: `~` key
-   Type: `ShowDebug` to see debug text
-   Look for capsule outline shrinking

### Test 1.2: Toggle Crouch (Crouching → Standing)

**Steps:**

1. While crouched (from Test 1.1)
2. Press **C** key again

**Expected Results:**

-   ✅ Cat's capsule height returns to normal
-   ✅ Smooth transition back to standing
-   ✅ Console log: "Cat is standing up"
-   ✅ Movement speed returns to normal

### Test 1.3: Alternative Crouch Input (Left Ctrl)

**Steps:**

1. Press **Left Ctrl** instead of C

**Expected Results:**

-   ✅ Same behavior as C key
-   ✅ Toggles between crouch/stand

### Test 1.4: Gamepad Crouch Input

**Steps:**

1. Connect gamepad
2. Click **Right Thumbstick** (R3)

**Expected Results:**

-   ✅ Same behavior as keyboard
-   ✅ Toggles crouch state

---

## Test Suite 2: Movement Speed Changes

### Test 2.1: Walking Speed (Standing)

**Steps:**

1. Stand up (press C if crouched)
2. Hold **W** to move forward
3. Observe movement speed

**Expected Results:**

-   ✅ Moves at 400 units/second (normal walk speed)
-   ✅ Feels responsive

### Test 2.2: Crouching Speed

**Steps:**

1. Press **C** to crouch
2. Hold **W** to move forward
3. Observe movement speed

**Expected Results:**

-   ✅ Moves at 200 units/second (50% of walk)
-   ✅ Noticeably slower than walking
-   ✅ Console log: "Cat is crouching (stealth mode)"

**How to Measure (Optional):**

```
1. Enable stat display: `stat FPS` in console
2. Use `DisplayAll CharacterMovement MaxWalkSpeed` to see current speed
```

### Test 2.3: Sprint Speed (Standing)

**Steps:**

1. Stand up (not crouched)
2. Hold **Left Shift** + **W**

**Expected Results:**

-   ✅ Moves at 800 units/second (sprint speed)
-   ✅ Much faster than walking

---

## Test Suite 3: Interaction Restrictions

### Test 3.1: Can't Sprint While Crouched

**Steps:**

1. Press **C** to crouch
2. Try to sprint by holding **Left Shift** + **W**

**Expected Results:**

-   ✅ Sprint does NOT activate
-   ✅ Console log: "Can't sprint while crouching!"
-   ✅ Movement speed stays at 200 (crouch speed)

### Test 3.2: Sprinting Stops When Crouching

**Steps:**

1. Stand up (press C if crouched)
2. Hold **Left Shift** + **W** to sprint
3. While sprinting, press **C** to crouch

**Expected Results:**

-   ✅ Sprint automatically stops
-   ✅ Movement speed drops to 200 (crouch speed)
-   ✅ Cat crouches successfully

### Test 3.3: Can't Crouch While In Air

**Steps:**

1. Stand up
2. Press **Space** to jump
3. While in mid-air, press **C**

**Expected Results:**

-   ✅ Crouch does NOT activate
-   ✅ Console log: "Can't crouch while climbing or in the air!"
-   ✅ Cat remains standing when landing

### Test 3.4: Can't Climb While Crouched

**Prerequisites:** Need a climbable surface (curtain or tall furniture tagged "Climbable")

**Steps:**

1. Press **C** to crouch
2. Move toward climbable wall (hold **W**)
3. Attempt to climb by moving forward

**Expected Results:**

-   ✅ Climbing does NOT start
-   ✅ Console log: "Can't climb while crouching! Stand up first."
-   ✅ Must press C to stand, then can climb

### Test 3.5: Can't Crouch While Climbing

**Prerequisites:** Must be climbing a wall

**Steps:**

1. Start climbing a wall (move toward tagged surface)
2. While climbing, press **C**

**Expected Results:**

-   ✅ Crouch does NOT activate
-   ✅ Console log: "Can't crouch while climbing or in the air!"
-   ✅ Remains in climbing state

---

## Test Suite 4: Stealth Detection Mechanics

### Test 4.1: Normal Detection Range (Standing)

**Prerequisites:**

-   Need `BP_HumanNPC` placed in level
-   NPC should have `DetectionRadius = 1000` (default)

**Steps:**

1. Stand up (not crouched)
2. Position cat 900 units away from NPC
3. Face toward NPC (within vision cone)
4. Observe if detected (red debug line appears)

**Expected Results:**

-   ✅ NPC detects cat at 900 units (within 1000 range)
-   ✅ Red debug line drawn from NPC to cat
-   ✅ If sprinting, triggers "Human caught the cat!" message

### Test 4.2: Reduced Detection Range (Crouching)

**Steps:**

1. Press **C** to crouch
2. Position cat 900 units away from NPC (same as Test 4.1)
3. Face toward NPC (within vision cone)
4. Observe if detected

**Expected Results:**

-   ✅ NPC does NOT detect cat at 900 units
-   ✅ Detection range reduced to 500 units (50% of 1000)
-   ✅ No red debug line
-   ✅ Can sneak past at closer range

### Test 4.3: Stealth Advantage Visualization

**Steps:**

1. Stand 600 units from NPC (use editor ruler or `stat Unit`)
2. Verify detection (red line should appear)
3. Press **C** to crouch
4. Observe detection status

**Expected Results:**

-   ✅ **Standing:** Detected at 600 units (within 1000)
-   ✅ **Crouching:** NOT detected at 600 units (outside 500 effective range)
-   ✅ Red line disappears when crouching
-   ✅ Stealth provides significant advantage

**How to Measure Distance:**

```
1. Select cat in editor (while playing)
2. Note location in Details panel
3. Select NPC
4. Note NPC location
5. Calculate distance: sqrt((x2-x1)² + (y2-y1)²)
```

Or use Blueprint:

```
Event Tick → Get Distance to NPC → Print String
```

---

## Test Suite 5: Overhead Obstacle Detection

### Test 5.1: Can Stand in Open Space

**Steps:**

1. Crouch in open area (no obstacles above)
2. Press **C** to stand up

**Expected Results:**

-   ✅ Stands up successfully
-   ✅ Smooth transition to standing height
-   ✅ Console log: "Cat is standing up"

### Test 5.2: Can't Stand Under Low Object

**Prerequisites:** Need low-hanging object in level (table, desk, shelf)

**Steps:**

1. Crouch and move under low object
2. Try to stand up by pressing **C**

**Expected Results:**

-   ✅ Remains crouched
-   ✅ Console log: "Not enough room to stand up!"
-   ✅ No capsule collision errors

### Test 5.3: Can Stand After Moving Away

**Steps:**

1. From Test 5.2 (stuck under object)
2. Move (while crouched) away from obstacle
3. Press **C** to stand up

**Expected Results:**

-   ✅ Stands up successfully once clear
-   ✅ System correctly detects available space

**Debugging:**

-   Enable collision visualization: `Show → Collision` in viewport
-   Look for capsule sweep check (green = clear, red = blocked)

---

## Test Suite 6: Edge Cases & Polish

### Test 6.1: Rapid Toggle Spam

**Steps:**

1. Rapidly press **C** key multiple times (spam it)

**Expected Results:**

-   ✅ System handles rapid input gracefully
-   ✅ No stuck states or animation glitches
-   ✅ Smooth interpolation continues correctly

### Test 6.2: Crouch + Jump

**Steps:**

1. Press **C** to crouch
2. Press **Space** to jump (while crouched)

**Expected Results:**

-   ✅ Cat stands up automatically when jumping
-   ✅ Jump executes normally
-   ✅ Lands in standing state (not crouched)

### Test 6.3: Crouch During Fall

**Steps:**

1. Stand on elevated platform
2. Walk off edge (don't jump)
3. While falling, press **C**

**Expected Results:**

-   ✅ Crouch does NOT activate
-   ✅ Console log: "Can't crouch while climbing or in the air!"
-   ✅ Lands in standing state

### Test 6.4: Multi-Input Combos

**Test Sprint → Crouch → Jump:**

1. Hold **Shift + W** (sprint)
2. Press **C** (crouch)
3. Press **Space** (jump)

**Expected Results:**

-   ✅ Sprint stops when crouching
-   ✅ Jump causes stand-up
-   ✅ No state conflicts

---

## Test Suite 7: Performance & Stability

### Test 7.1: Frame Rate Impact

**Steps:**

1. Enable `stat FPS` in console
2. Toggle crouch on/off repeatedly
3. Observe FPS counter

**Expected Results:**

-   ✅ No noticeable FPS drops
-   ✅ Crouch system has negligible performance cost
-   ✅ Smooth 60+ FPS maintained

### Test 7.2: Extended Crouch Session

**Steps:**

1. Crouch for 5+ minutes of gameplay
2. Move around, sneak past NPCs
3. Perform various actions

**Expected Results:**

-   ✅ No memory leaks or performance degradation
-   ✅ System remains stable
-   ✅ No unexpected state changes

---

## Debugging Tools

### Enable Debug Visualization

```cpp
// Console commands (press ~ to open console)
ShowDebug                    // Show debug text overlay
stat FPS                     // Show frame rate
stat Unit                    // Show frame time breakdown
Show Collision               // Visualize collision shapes
DisplayAll CharacterMovement MaxWalkSpeed  // Show current speed
```

### Custom Debug Print (Add to Blueprint)

**In `BP_CatCharacter` Event Graph:**

```
Event Tick →
  Is Crouching (function) →
  Branch:
    True → Print String "CROUCHING" (Green)
    False → Print String "STANDING" (White)
```

### Log Filtering

```powershell
# View logs in real-time
tail -f "c:\git\catastrophe\Saved\Logs\Catastrophe.log"

# Filter for crouch messages
Select-String -Path "c:\git\catastrophe\Saved\Logs\Catastrophe.log" -Pattern "crouch|stealth"
```

---

## Known Issues & Workarounds

### Issue: Input Not Responding

**Symptoms:** Pressing C does nothing

**Possible Causes:**

1. C++ code not compiled
2. Input bindings not loaded
3. Editor needs restart

**Solutions:**

1. Rebuild project (Ctrl+Shift+B)
2. Close and reopen Unreal Editor
3. Check `DefaultInput.ini` has Crouch mappings

### Issue: Capsule Not Changing Size

**Symptoms:** Crouch activates but capsule stays same height

**Cause:** Transition happening but too fast to see

**Solutions:**

1. Lower `CrouchTransitionSpeed` in `BP_CatCharacter` (try 5.0)
2. Enable `Show Collision` to visualize capsule
3. Check console logs confirm state change

### Issue: Always Says "Not enough room"

**Symptoms:** Can't stand up anywhere

**Cause:** Overly sensitive obstacle detection

**Solutions:**

1. Check for invisible collision boxes in level
2. Adjust capsule radius slightly smaller
3. Move to completely open area for testing

---

## Success Criteria

### Minimum Viable Feature (Must Pass)

-   [x] C key toggles crouch on/off
-   [x] Movement speed reduces while crouched
-   [x] NPC detection range reduced by 50%
-   [x] Can't sprint while crouched
-   [x] Can't climb while crouched
-   [x] Can't crouch in mid-air
-   [x] Smooth capsule transition

### Polish Features (Should Pass)

-   [x] Alternative inputs work (Ctrl, gamepad)
-   [x] Overhead obstacle detection works
-   [x] Console logs provide feedback
-   [x] No performance issues
-   [x] No state conflicts or stuck states

### Ready for Merge When:

-   ✅ All "Must Pass" tests complete
-   ✅ At least 80% of "Should Pass" tests complete
-   ✅ No critical bugs
-   ✅ Documentation complete

---

## Reporting Issues

If you find bugs during testing:

### Create GitHub Issue

**Title Format:** `[Stealth] Brief description`

**Required Info:**

```markdown
**Bug Description:**
Clear description of the issue

**Steps to Reproduce:**

1. Step one
2. Step two
3. Expected vs actual result

**Environment:**

-   UE5 Version: 5.6
-   Platform: Windows 11
-   Build Config: Development Editor

**Logs/Screenshots:**
Attach relevant console output or screenshots
```

**Labels:** `bug`, `stealth-system`, `priority-high` (if breaking)

---

## Next Steps After Testing

### If Tests Pass ✅

1. **Update Current Sprint:**

    - Mark "Add crouching/stealth movement mode" as complete
    - Update `docs/CURRENT_SPRINT.md`

2. **Commit Changes:**

    ```bash
    git add .
    git commit -m "feat: implement crouch/stealth system with NPC detection integration"
    git push
    ```

3. **Create Changelog Entry:**

    - Add to `docs/CHANGELOG_STEALTH_SYSTEM.md`
    - Update main `CHANGELOG.md`

4. **Move to Next Task:**
    - Begin pounce/attack mechanic (next milestone item)
    - Or polish crouch animations

### If Tests Fail ❌

1. **Document Failures:**

    - Which tests failed
    - Error messages/logs
    - Steps to reproduce

2. **Prioritize Fixes:**

    - Critical bugs first (crashes, stuck states)
    - Polish issues later (animation, feel)

3. **Iterate:**
    - Fix identified issues
    - Rebuild and re-test
    - Repeat until success criteria met

---

## Additional Resources

-   **Stealth System Guide:** [STEALTH_CROUCH_GUIDE.md](STEALTH_CROUCH_GUIDE.md)
-   **Source Code:** `Source/Catastrophe/CatCharacter.h/cpp`
-   **Input Config:** `Config/DefaultInput.ini`
-   **Sprint Docs:** [Current Sprint Progress](CURRENT_SPRINT.md)

---

**Happy Testing! Let's make this cat sneaky! 🐱🤫**
