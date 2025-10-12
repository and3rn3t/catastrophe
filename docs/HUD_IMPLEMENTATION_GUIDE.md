# 🎨 HUD System Implementation Guide

## What We Just Created

### ✅ C++ Foundation (Complete!)
- ✅ `CatHUDWidget.h/cpp` - HUD widget class
- ✅ `CatCharacter` - Added `bIsDetectedByNPC` flag and getters/setters
- ✅ `CatastropheGameMode` - Auto-creates HUD on BeginPlay
- ✅ `HumanNPC` - Notifies cat when detected
- ✅ **Build Status:** ✅ Succeeded!

---

## 🎯 Next Steps: Create the Blueprint Widget

### **Step 1: Create Widget Blueprint (5 minutes)**

1. **Open Unreal Editor**
   ```
   Open: C:\git\catastrophe\Catastrophe.uproject
   ```

2. **Create Widget Blueprint**
   ```
   Content Browser → Right-click in Content/UI folder (create if needed)
   → User Interface → Widget Blueprint
   Name it: WBP_HUD
   ```

3. **Open the Widget**
   ```
   Double-click WBP_HUD to open the UMG Designer
   ```

---

### **Step 2: Design the HUD Layout (10 minutes)**

#### **A. Score Display (Top Right)**
1. Drag **Canvas Panel** to hierarchy (if not already there)
2. Drag **Text Block** into Canvas Panel
3. Rename it to: `ScoreText`
4. Configure in Details panel:
   ```
   Position X: -200 (anchored to top-right)
   Position Y: 20
   Anchors: Top-Right
   Alignment: 1.0, 0.0
   Size: 180 x 40
   ```
5. Text properties:
   ```
   Text: "Score: 0"
   Font Size: 32
   Color: Orange (255, 165, 0)
   Justification: Right
   ```

#### **B. Stamina Bar (Bottom Center)**
1. Drag **Progress Bar** into Canvas Panel
2. Rename it to: `StaminaBar`
3. Configure:
   ```
   Position X: -150
   Position Y: -60
   Anchors: Bottom-Center
   Alignment: 0.5, 1.0
   Size: 300 x 20
   ```
4. Appearance:
   ```
   Percent: 1.0 (full)
   Fill Color: Green (0, 255, 0)
   Background Color: Dark Gray (30, 30, 30)
   ```

5. Add **Text Block** above it (label)
   ```
   Text: "Stamina"
   Font Size: 18
   Position: Above the bar
   ```

#### **C. Stealth Indicator (Top Left)**
1. Drag **Image** into Canvas Panel
2. Rename it to: `StealthIcon`
3. Configure:
   ```
   Position X: 20
   Position Y: 20
   Anchors: Top-Left
   Size: 50 x 50
   ```
4. Color & Opacity:
   ```
   Green when not detected: (0, 255, 0, 255)
   Red when detected: (255, 0, 0, 255)
   ```

5. Add **Text Block** next to it
   ```
   Rename: `StealthStatusText`
   Text: "HIDDEN"
   Font Size: 24
   ```

---

### **Step 3: Bind to C++ Class (5 minutes)**

1. **In WBP_HUD, click "Graph" tab**

2. **Set Parent Class**
   ```
   File → Reparent Blueprint
   Select: CatHUDWidget (your C++ class)
   ```

3. **Verify Variables Are Bindable**
   - In the Graph, you should now see functions from `UCatHUDWidget`

---

### **Step 4: Implement Update Functions (15 minutes)**

#### **A. Update Score**
1. In **Graph** tab, right-click → **Override** → `Update Score`
2. Connect nodes:
   ```
   Update Score (Event)
   → Format Text: "Score: {0}"
      → Score (float input)
   → Set Text (ScoreText)
   ```

#### **B. Update Stamina**
1. Override: `Update Stamina`
2. Connect:
   ```
   Update Stamina (Event)
   → Set Percent (StaminaBar)
      → StaminaPercent (input 0.0-1.0)
   ```

3. **Add color change:**
   ```
   StaminaPercent → Branch (< 0.3?)
      True → Set Fill Color (Red)
      False → Set Fill Color (Green)
   ```

#### **C. Update Stealth Status**
1. Override: `Update Stealth Status`
2. Connect:
   ```
   bIsDetected → Branch
      True:
         → Set Color (StealthIcon) → Red
         → Set Text (StealthStatusText) → "DETECTED!"
      False:
         bIsCrouching → Branch
            True:
               → Set Color (StealthIcon) → Blue
               → Set Text → "SNEAKING"
            False:
               → Set Color (StealthIcon) → Green
               → Set Text → "HIDDEN"
   ```

---

### **Step 5: Assign HUD in GameMode (2 minutes)**

1. **Open BP_GameMode** (or create if you don't have one):
   ```
   Content Browser → Blueprints → BP_GameMode
   Or: Create new Blueprint Class → Game Mode Base
   ```

2. **Set HUD Widget Class**:
   ```
   In Details panel:
   → UI section
   → HUD Widget Class → Select WBP_HUD
   ```

3. **Set World Settings to use this GameMode**:
   ```
   Window → World Settings
   → Game Mode Override → BP_GameMode
   ```

---

## 🎮 Testing Your HUD (5 minutes)

### **1. Play in Editor**
```
Click Play (Alt+P)
```

### **2. Verify HUD Elements**
- ✅ Score displays in top-right (starts at 0)
- ✅ Stamina bar is visible (should be full/green)
- ✅ Stealth icon shows green ("HIDDEN")

### **3. Test Interactions**
- **Move around:** HUD should stay on screen
- **Sprint (hold Shift):** Stamina bar should drain
- **Stop sprinting:** Stamina should regenerate
- **Get detected by NPC:** Icon should turn red, text = "DETECTED!"
- **Crouch (C):** Should show "SNEAKING" in blue

---

## 🎨 Optional Enhancements

### **Make it Pretty:**
1. **Add drop shadow to text**
   ```
   ScoreText → Shadow Offset: 2, 2
   Shadow Color: Black
   ```

2. **Add background panel**
   ```
   Add Canvas Panel behind elements
   Set semi-transparent black background
   ```

3. **Animate the "DETECTED!" warning**
   ```
   Create Animation:
   - Pulse scale 1.0 → 1.2 → 1.0
   - Flash color red → white → red
   - Duration: 0.5s, Loop
   ```

### **Add More Info:**
4. **Objectives text** (bottom-left)
5. **Timer** (if timed levels)
6. **Combo counter** (for consecutive mischief)

---

## 📝 Quick Checklist

Before moving on, verify:
- [ ] WBP_HUD Blueprint exists
- [ ] Parent class is `CatHUDWidget`
- [ ] Score text displays
- [ ] Stamina bar displays
- [ ] Stealth icon displays
- [ ] HUD Widget Class set in GameMode
- [ ] Game Mode assigned in World Settings
- [ ] HUD appears when playing

---

## 🐛 Troubleshooting

### **"HUD doesn't appear"**
**Solutions:**
1. Check World Settings → Game Mode Override is set
2. Verify GameMode → HUD Widget Class is assigned
3. Check Output Log for errors
4. Make sure you hit "Compile" in BP_HUD

### **"HUD shows but doesn't update"**
**Solutions:**
1. Verify parent class is `CatHUDWidget` (not `UserWidget`)
2. Check that override functions are implemented
3. Ensure `NativeTick` is being called (add print string to test)

### **"Can't find CatHUDWidget in parent class list"**
**Solution:**
- Close Unreal Editor
- Recompile C++ code
- Reopen Unreal Editor
- Try again

---

## 🎯 What's Next?

After HUD is working:
1. ✅ **Pause Menu** - Next priority
2. ✅ **Main Menu** - For game start
3. ✅ **Game Over Screen** - When caught
4. ✅ **Interaction Prompts** - "Press E to interact"

---

## 📚 Reference

### **Variables Exposed to Blueprint:**
```cpp
// From CatCharacter:
- GetMischiefScore() → float
- GetStaminaPercent() → float (0.0-1.0)
- IsDetectedByNPC() → bool
- IsCrouching() → bool

// HUD automatically calls:
- UpdateScore(float Score)
- UpdateStamina(float Percent)
- UpdateStealthStatus(bool bDetected, bool bCrouching)
```

### **Files Created:**
- `Source/Catastrophe/CatHUDWidget.h`
- `Source/Catastrophe/CatHUDWidget.cpp`
- `Content/UI/WBP_HUD` (you'll create this)

---

**Ready to create the widget? Let me know if you need help with any step!** 🎨✨

