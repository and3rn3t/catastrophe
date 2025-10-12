# 🧪 Testing the Climbing System - Quick Start

**Date:** October 11, 2025
**Feature:** Climbing System
**Estimated Time:** 10-15 minutes

---

## 🎯 Quick Test (5 Minutes)

### **1. Build the Code** (if not already done)

In VS Code:

```
Ctrl+Shift+B
```

Wait for "Task completed successfully"

### **2. Open Unreal Editor**

If editor is closed:

```powershell
start Catastrophe.uproject
```

### **3. Create Test Wall**

1. In your test level (or create new: File → New Level → Default)
2. **Place Actors** panel → Search "Cube"
3. Drag **Cube** into viewport
4. **Details Panel:**
    - Location: X=500, Y=0, Z=100
    - Scale: X=1, Y=10, Z=10 (tall wall)
5. **Add Tag:**
    - Details → Tags → "+" button
    - Type: **Climbable**
    - Press Enter

### **4. Test Climbing**

1. Press **Alt+P** to play
2. Walk toward the wall (hold **W**)
3. Cat should **auto-start climbing** when touching wall
4. **Test controls:**
    - **W** → Climb up
    - **S** → Climb down
    - **A/D** → Strafe left/right
    - **Space** → Jump off wall
    - **S (hold, pull back)** → Dismount

### **5. Verify Behaviors**

✅ **Auto-Start:** Walking into wall starts climbing
✅ **Vertical Movement:** W/S climbs smoothly
✅ **Horizontal Strafe:** A/D moves along wall
✅ **Wall Jump:** Space launches away from wall
✅ **Reach Top:** Auto-dismount when hitting top
✅ **Pull Away:** Holding S releases from wall

---

## 🎮 Advanced Test Scenarios (10 Minutes)

### **Scenario 1: Multiple Climbable Objects**

1. Create 3-4 cubes with different heights
2. Tag all as "Climbable"
3. Place them in a pattern (like stepping stones)
4. Test:
    - Climb one, wall jump to next
    - Switch between surfaces mid-climb
    - Verify smooth transitions

### **Scenario 2: Curtain Simulation**

1. Create tall, thin cube (X=0.2, Y=2, Z=8)
2. Tag "Climbable"
3. Position like hanging curtain
4. Test:
    - Climb from bottom to top
    - Strafe side-to-side
    - Reach top → Should boost onto ledge

### **Scenario 3: Corner Climbing**

1. Create two perpendicular walls (L-shape)
2. Tag both "Climbable"
3. Test:
    - Climb one wall
    - Strafe toward corner
    - See if detection switches to other wall

### **Scenario 4: Furniture Climb**

1. Place Actors → Cube (simulate bookshelf)
2. Scale: X=2, Y=4, Z=6
3. Tag "Climbable"
4. Test:
    - Climb front face
    - Wall jump to reach top
    - Land on top surface

---

## 🐛 What to Look For (Issues)

### **Expected Behaviors:**

✅ Smooth start when walking into wall
✅ Gravity disabled while climbing
✅ Character faces away from wall
✅ Movement speed feels responsive
✅ Wall jump has good trajectory
✅ Auto-dismount at top feels natural

### **Potential Issues:**

❌ **Falling Through Wall:** Check collision is set to "Block Pawn"
❌ **Can't Start Climbing:** Verify "Climbable" tag is spelled correctly
❌ **Camera Clips Wall:** May need to adjust camera distance
❌ **Stuck at Top:** Collision might be preventing boost
❌ **Jittery Movement:** Check ClimbSpeed isn't too high
❌ **Can't Strafe:** Ensure A/D inputs are working

---

## ⚙️ Tuning Parameters (If Needed)

If climbing doesn't feel right, adjust in **BP_CatCharacter**:

1. Open `BP_CatCharacter` in Content Browser
2. **Class Defaults** → **Climbing** section
3. Adjust values:

| Parameter            | Default | Too Fast | Too Slow | Recommended |
| -------------------- | ------- | -------- | -------- | ----------- |
| ClimbSpeed           | 300     | > 400    | < 200    | 250-350     |
| ClimbDetectionRadius | 100     | > 150    | < 60     | 80-120      |
| ClimbJumpVelocity    | 500     | > 700    | < 400    | 400-600     |

4. **Compile & Save**
5. Test again (no rebuild needed!)

---

## 📊 Performance Check

While testing, press **`** (tilde key) to open console:

```
stat FPS        // Show frame rate
stat Unit       // Show detailed timing
stat Game       // Show game thread time
```

**Expected:** 60+ FPS with no lag during climbing

---

## ✅ Test Completion Checklist

Mark off as you test:

-   [ ] Auto-start climbing works
-   [ ] Vertical movement (W/S) smooth
-   [ ] Horizontal strafe (A/D) functional
-   [ ] Wall jump (Space) launches correctly
-   [ ] Reach top auto-dismounts with boost
-   [ ] Pull-back dismount (hold S) works
-   [ ] No collision glitches
-   [ ] Camera doesn't clip badly
-   [ ] Performance is smooth (60+ FPS)
-   [ ] Multiple climbable objects work

---

## 📝 Feedback Template

**What Works Well:**

-   [List positive observations]

**What Needs Improvement:**

-   [List issues or suggestions]

**Parameter Changes Tested:**

-   ClimbSpeed: [value] → [result]
-   Detection: [value] → [result]
-   Jump Velocity: [value] → [result]

---

## 🎯 Next Steps After Testing

### **If Everything Works:**

1. Mark climbing task complete in `CURRENT_SPRINT.md`
2. Move to next Milestone 1.1 feature:
    - Sprint Stamina System
    - Crouching/Stealth Mode
    - Tail Physics
    - Cat Sound Effects

### **If Issues Found:**

1. Document issues in feedback template above
2. Check console output for errors (Window → Developer Tools → Output Log)
3. Verify collision settings on climbable objects
4. Adjust tuning parameters in BP_CatCharacter
5. Rebuild if C++ changes needed (Ctrl+Shift+B)

### **For Visual Polish (Later):**

1. Import cat skeletal mesh
2. Create climbing animations
3. Add sound effects (claw scratch, effort grunts)
4. Add particle effects (dust, claw marks)

---

## 🔗 Related Documentation

-   **Full Guide:** `docs/CLIMBING_SYSTEM_GUIDE.md`
-   **Changelog:** `docs/CHANGELOG_CLIMBING_SYSTEM.md`
-   **Jump Testing:** `docs/TESTING_JUMP_MECHANICS.md`
-   **Sprint Status:** `docs/CURRENT_SPRINT.md`

---

**Ready?** Open Unreal Editor and start climbing! 🧗🐱

**Build Status:** ✅ Compiled
**Time to Test:** ~10-15 minutes
**Difficulty:** Easy (just tag objects "Climbable")

---

🎉 **Have fun breaking physics with cat climbing!**
