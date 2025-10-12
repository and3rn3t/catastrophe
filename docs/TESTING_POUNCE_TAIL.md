# Testing Guide - Pounce & Tail Physics

> **Test Date:** October 12, 2025
> **Features:** Pounce/Attack System, Tail Physics System
> **Build Required:** Development Editor

## Pre-Testing Setup

### 1. Build the Project

```powershell
# In VS Code: Ctrl+Shift+B
# Or run task: "Build Catastrophe (Development Editor)"
```

**Expected Result:** Build succeeds with no errors

### 2. Open in Unreal Editor

```powershell
# Double-click Catastrophe.uproject
# Or use Epic Games Launcher
```

**Expected Result:** Project opens, no compilation errors

### 3. Verify Blueprint Setup

**Check `BP_CatCharacter` exists:**

-   Content Browser → `Content/Blueprints/BP_CatCharacter`
-   If missing, create Blueprint child of `CatCharacter` C++ class
-   Add Skeletal Mesh component (placeholder capsule is fine for testing)

**Check GameMode:**

-   Open `BP_CatastropheGameMode`
-   Verify Default Pawn Class = `BP_CatCharacter`

---

## Pounce System Testing

### Basic Functionality Tests

#### Test 1: Pounce Input

**Steps:**

1. Press Play (Alt+P) in editor
2. Press **F** or **Left Mouse Button** or **Right Trigger** (gamepad)

**Expected:**

-   Cat launches forward in arc trajectory
-   Movement feels responsive
-   Console log: "Cat pounced!"

**Pass/Fail:** \_\_\_

---

#### Test 2: Cooldown System

**Steps:**

1. Pounce once
2. Immediately try to pounce again
3. Wait 1.5 seconds
4. Try pouncing again

**Expected:**

-   Second pounce blocked immediately
-   Console shows no "Cat pounced!" message
-   After 1.5s, pounce works again

**Pass/Fail:** \_\_\_

---

#### Test 3: Stamina Cost

**Steps:**

1. Check stamina (should be 100)
2. Pounce once
3. Check stamina again

**Expected:**

-   Stamina decreases by 15 points (100 → 85)
-   Multiple pounces drain stamina further
-   At < 15 stamina, pounce is blocked

**Pass/Fail:** \_\_\_

---

#### Test 4: Arc Trajectory

**Steps:**

1. Stand on flat ground
2. Pounce forward
3. Observe flight path

**Expected:**

-   Cat launches forward **and** upward
-   Arc feels natural (not straight line)
-   Lands ~3-4 character lengths ahead

**Pass/Fail:** \_\_\_

---

### Object Interaction Tests

#### Test 5: Bonus Scoring (Setup Required)

**Setup:**

1. Place test object in level (e.g., SM_Chair or placeholder cube)
2. Add tags: "Destructible" and "Vase"
3. Enable Physics Simulation on object

**Steps:**

1. Pounce toward object
2. Check console for score message

**Expected:**

-   Console shows: "Pounced on: [ObjectName]! Bonus points!"
-   Score increases by **15 points** (not 10)
-   Object receives strong impulse force

**Pass/Fail:** \_\_\_

---

#### Test 6: Landing Impact

**Steps:**

1. Place destructible object on ground
2. Pounce **over** the object
3. Land on top of it

**Expected:**

-   Downward force applied on landing
-   Object pushed down/moves
-   Console: "Landed on [ObjectName] with impact!"

**Pass/Fail:** \_\_\_

---

### Restriction Tests

#### Test 7: Cannot Pounce While Climbing

**Setup:**

1. Create climbable surface (add "Climbable" tag)
2. Start climbing

**Steps:**

1. While climbing, press pounce key

**Expected:**

-   Pounce does NOT activate
-   Console: "Can't pounce while climbing or crouching!"

**Pass/Fail:** \_\_\_

---

#### Test 8: Cannot Pounce While Crouching

**Steps:**

1. Press **C** to crouch
2. Press pounce key

**Expected:**

-   Pounce does NOT activate
-   Console: "Can't pounce while climbing or crouching!"

**Pass/Fail:** \_\_\_

---

#### Test 9: Cannot Pounce in Air (Past Coyote Time)

**Steps:**

1. Jump high
2. Wait until apex of jump
3. Try to pounce

**Expected:**

-   Pounce blocked if >0.15s since leaving ground
-   Can still pounce within 0.15s (coyote time)

**Pass/Fail:** \_\_\_

---

### Feel & Polish Tests

#### Test 10: Air Control

**Steps:**

1. Pounce forward
2. While in air, press A/D (strafe left/right)

**Expected:**

-   Can adjust direction mid-air
-   Control feels responsive (80% air control)
-   Not too floaty, not too rigid

**Pass/Fail:** \_\_\_

---

#### Test 11: Visual Feedback

**Steps:**

1. Pounce forward
2. Look for debug visualization

**Expected:**

-   Red line shows trajectory (visible 0.5s)
-   Line disappears after time

**Pass/Fail:** \_\_\_

---

## Tail Physics Testing

### Basic Physics Tests

#### Test 12: Tail Initialization

**Steps:**

1. Start play session
2. Check console output

**Expected:**

-   Console: "Tail physics initialized with 5 segments"
-   No errors or warnings

**Pass/Fail:** \_\_\_

---

#### Test 13: Tail Following (Debug Visualization)

**Note:** To see tail, add debug visualization to C++ or create Blueprint visual

**Steps:**

1. Walk forward slowly (WASD)
2. Observe tail behavior

**Expected:**

-   Tail segments follow character
-   Slight lag behind body movement
-   Segments maintain spacing

**Pass/Fail:** \_\_\_

---

#### Test 14: Tail Droop (Gravity)

**Steps:**

1. Stand still for 2 seconds
2. Observe tail

**Expected:**

-   Tail gradually droops downward
-   Settles into natural hanging position
-   Movement smooth (not jittery)

**Pass/Fail:** \_\_\_

---

#### Test 15: Direction Changes

**Steps:**

1. Sprint forward
2. Quickly turn 180 degrees
3. Observe tail

**Expected:**

-   Tail swings out to side during turn
-   Lags behind body rotation
-   Eventually catches up and follows

**Pass/Fail:** \_\_\_

---

#### Test 16: Jump Response

**Steps:**

1. Jump (Space)
2. Watch tail during ascent and descent

**Expected:**

-   Tail lifts during jump takeoff
-   Trails behind during upward motion
-   Droops during fall
-   Settles on landing

**Pass/Fail:** \_\_\_

---

### Parameter Tuning Tests

#### Test 17: Adjust Stiffness

**Steps:**

1. Stop play session
2. Open `BP_CatCharacter` Blueprint
3. Set `TailStiffness` to 0.3 (very floppy)
4. Play and move around
5. Stop play, set `TailStiffness` to 0.95 (rigid)
6. Play and move around

**Expected:**

-   Low stiffness: Tail very flexible, exaggerated movement
-   High stiffness: Tail rigid, minimal bending
-   Clear difference between values

**Pass/Fail:** \_\_\_

---

#### Test 18: Adjust Gravity Scale

**Steps:**

1. Set `TailGravityScale` to 0.1 (floaty)
2. Play and stand still
3. Stop, set to 0.8 (heavy droop)
4. Play and stand still

**Expected:**

-   Low gravity: Tail stays upright, barely droops
-   High gravity: Tail hangs very low
-   Clear visual difference

**Pass/Fail:** \_\_\_

---

#### Test 19: Adjust Segment Count

**Steps:**

1. Set `TailSegmentCount` to 3
2. Play session
3. Stop, set to 10
4. Play session

**Expected:**

-   3 segments: Simple tail, less smooth
-   10 segments: More detailed, smoother curves
-   No crashes or errors

**Pass/Fail:** \_\_\_

---

### Performance Tests

#### Test 20: Frame Rate Check

**Steps:**

1. Open console (`~` key)
2. Type: `stat fps`
3. Observe FPS counter while moving

**Expected:**

-   Stable 60+ FPS (or target frame rate)
-   No noticeable drops during tail simulation
-   Tail system adds < 0.1ms to frame time

**Pass/Fail:** \_\_\_

---

#### Test 21: No Visual Glitches

**Steps:**

1. Sprint around level for 30 seconds
2. Jump repeatedly
3. Turn quickly multiple times

**Expected:**

-   No tail segments flying off
-   No jittering or shaking
-   Segments stay connected
-   No console errors

**Pass/Fail:** \_\_\_

---

## Integration Tests

### Test 22: Pounce + Tail Interaction

**Steps:**

1. Pounce forward
2. Observe tail during pounce

**Expected:**

-   Tail responds to sudden velocity change
-   Trails behind during pounce
-   Settles after landing
-   No conflicts between systems

**Pass/Fail:** \_\_\_

---

### Test 23: All Systems Together

**Steps:**

1. Sprint forward (Shift + W)
2. Jump (Space)
3. Pounce at apex (F)
4. Land and crouch (C)
5. Stand up

**Expected:**

-   All systems work without conflicts
-   Tail responds appropriately to each action
-   No crashes or errors
-   Stamina drains/recovers correctly

**Pass/Fail:** \_\_\_

---

## Blueprint Integration Tests

### Test 24: Get Tail Positions (Blueprint)

**Setup:**

1. Open `BP_CatCharacter` Event Graph
2. Override `OnTailPositionsUpdated` event
3. Add `GetTailSegmentPositions` node
4. Print first position to screen

**Steps:**

1. Play session
2. Check screen for position output

**Expected:**

-   Position values print every frame
-   Values change as character moves
-   No errors

**Pass/Fail:** \_\_\_

---

### Test 25: Pounce Cooldown UI

**Setup:**

1. Create simple HUD widget
2. Add `GetPounceCooldownPercent` call
3. Display as progress bar

**Steps:**

1. Pounce
2. Observe progress bar

**Expected:**

-   Bar fills from 0% to 100% over 1.5 seconds
-   Accurate representation of cooldown
-   Pounce available when bar full

**Pass/Fail:** \_\_\_

---

## Regression Tests

### Test 26: Existing Features Still Work

**Steps:**

1. Test jump mechanics (hold space)
2. Test climbing (move into climbable surface)
3. Test sprint (Shift)
4. Test crouch (C)

**Expected:**

-   All previous systems function normally
-   No broken features
-   No new bugs introduced

**Pass/Fail:** \_\_\_

---

## Final Checklist

-   [ ] All pounce tests pass (Tests 1-11)
-   [ ] All tail physics tests pass (Tests 12-21)
-   [ ] Integration tests pass (Tests 22-23)
-   [ ] Blueprint tests pass (Tests 24-25)
-   [ ] Regression test passes (Test 26)
-   [ ] No console errors during testing
-   [ ] Performance acceptable (60+ FPS)
-   [ ] Build compiles without errors

---

## Known Issues Log

Document any issues found during testing:

| Test # | Issue Description | Severity | Notes |
| ------ | ----------------- | -------- | ----- |
|        |                   |          |       |
|        |                   |          |       |
|        |                   |          |       |

---

## Testing Notes

**Tester Name:** ****\_\_\_****
**Test Date:** ****\_\_\_****
**Build Version:** Development Editor
**Platform:** Windows

**Overall Assessment:**

-   [ ] Ready for production
-   [ ] Needs minor fixes
-   [ ] Needs major revision

**Additional Comments:**

```
(Add any additional observations or feedback here)
```

---

## Next Steps After Testing

1. **If all tests pass:**

    - Mark tasks as complete in CURRENT_SPRINT.md
    - Proceed to adding cat sound effects
    - Consider adding animations for pounce

2. **If tests fail:**

    - Document issues in Known Issues Log
    - Create GitHub issues for bugs
    - Fix issues and retest

3. **For visual polish:**
    - Add pounce animation states
    - Create tail visual mesh/spline
    - Add particle effects for pounce trail
    - Add impact sounds

---

## Advanced Testing (Optional)

### Stress Testing

-   Pounce 100 times in succession → Check for memory leaks
-   Create 10 cat characters → Verify tail physics scale
-   Spawn 50 destructible objects → Test pounce detection

### Edge Cases

-   Pounce at exact moment of landing
-   Pounce while stamina = exactly 15
-   Adjust tail segments during play (editor)
-   Pounce toward boundary of level

### Performance Profiling

-   Use Unreal Insights to measure exact CPU cost
-   Profile with stat commands
-   Test on lower-end hardware

---

## See Also

-   [Pounce System Guide](POUNCE_SYSTEM_GUIDE.md)
-   [Tail Physics Guide](TAIL_PHYSICS_GUIDE.md)
-   [Changelog](CHANGELOG_POUNCE_TAIL.md)
-   [Current Sprint](CURRENT_SPRINT.md)
