# ✅ BUILD SUCCESSFUL! Now Create the Blueprint

## 🎯 What You Need to Do in Unreal Editor

### **Step 1: Open Unreal Editor**
```
Double-click: C:\git\catastrophe\Catastrophe.uproject
Wait for it to load...
```

---

### **Step 2: Create UI Folder (if needed)**
```
Content Browser → Content → Right-click
→ New Folder → Name: "UI"
```

---

### **Step 3: Create Widget Blueprint**
```
Content Browser → Navigate to Content/UI
→ Right-click in empty space
→ User Interface → Widget Blueprint
→ Name it: "WBP_HUD"
→ Double-click to open
```

---

### **Step 4: Set Parent Class**
**CRITICAL STEP - Don't skip!**
```
In WBP_HUD editor:
→ Top menu: "File" → "Reparent Blueprint"
→ Search for: "CatHUDWidget"
→ Select it → Click "Select" button

Verify: Top of editor should say "WBP_HUD (CatHUDWidget)"
```

---

### **Step 5: Design the Layout**

You're now in **Designer** view. Create these widgets:

#### **A. Canvas Panel (Container)**
```
Palette (left side) → Panel → Drag "Canvas Panel" to viewport
(If one exists already, you can use it)
```

#### **B. Score Text (Top-Right)**
```
1. Palette → Common → Drag "Text" onto Canvas Panel
2. In Hierarchy, press F2 to rename
3. Type EXACTLY: ScoreText
4. In Details panel:
   - Check ☑ "Is Variable"
   
   Slot (Canvas Panel Slot):
   - Click Anchors box → Select TOP-RIGHT preset
   - Position X: -200
   - Position Y: 20
   - Size X: 180
   - Size Y: 40
   - Alignment X: 1.0
   - Alignment Y: 0.0
   
   Appearance:
   - Text: "Score: 0"
   - Color: Orange (R:1.0, G:0.65, B:0.0)
   - Font Size: 32
   - Justification: Right
   
   Optional - Shadow:
   - Shadow Offset X: 2, Y: 2
   - Shadow Color: Black
```

#### **C. Stamina Bar (Bottom-Center)**
```
1. Palette → Common → Drag "Progress Bar" onto Canvas Panel
2. Rename to: StaminaBar
3. Details panel:
   - Check ☑ "Is Variable"
   
   Slot:
   - Anchors: BOTTOM-CENTER
   - Position X: -150
   - Position Y: -80
   - Size X: 300
   - Size Y: 25
   - Alignment X: 0.5
   - Alignment Y: 1.0
   
   Appearance:
   - Percent: 1.0
   - Fill Color: Green (R:0, G:1, B:0)
   - Background Color: Dark Gray (R:0.1, G:0.1, B:0.1, A:0.8)
```

#### **D. Stamina Label (Optional but nice)**
```
1. Drag "Text" onto Canvas Panel
2. Rename to: StaminaLabel
3. Details:
   - Check ☑ "Is Variable"
   
   Slot:
   - Anchors: BOTTOM-CENTER
   - Position X: 0
   - Position Y: -110
   - Size X: 100
   - Size Y: 30
   - Alignment X: 0.5
   - Alignment Y: 1.0
   
   Appearance:
   - Text: "Stamina"
   - Color: White
   - Font Size: 20
   - Justification: Center
```

#### **E. Stealth Icon (Top-Left)**
```
1. Palette → Common → Drag "Image" onto Canvas Panel
2. Rename to: StealthIcon
3. Details:
   - Check ☑ "Is Variable"
   
   Slot:
   - Anchors: TOP-LEFT
   - Position X: 20
   - Position Y: 20
   - Size X: 50
   - Size Y: 50
   
   Appearance:
   - Color: Green (R:0, G:1, B:0)
```

#### **F. Stealth Status Text (Top-Left)**
```
1. Drag "Text" onto Canvas Panel
2. Rename to: StealthStatusText
3. Details:
   - Check ☑ "Is Variable"
   
   Slot:
   - Anchors: TOP-LEFT
   - Position X: 80
   - Position Y: 30
   - Size X: 150
   - Size Y: 30
   
   Appearance:
   - Text: "HIDDEN"
   - Color: Green
   - Font Size: 24
```

---

### **Step 6: Verify Widget Names**

**Double-check in Hierarchy panel (left side):**
```
Canvas Panel
├─ ScoreText          ✅ (EXACT - case sensitive!)
├─ StaminaBar         ✅ (EXACT - case sensitive!)
├─ StaminaLabel       ✅ (optional)
├─ StealthIcon        ✅ (EXACT - case sensitive!)
└─ StealthStatusText  ✅ (EXACT - case sensitive!)
```

**And make sure ALL have "Is Variable" checked!**

---

### **Step 7: Compile and Save**
```
Top toolbar → Click "Compile" button
Should show green checkmark ✓
Click "Save"
```

---

### **Step 8: Assign HUD to GameMode**

#### **A. Find or Create GameMode:**
```
Content Browser → Blueprints folder

If you have BP_GameMode:
  → Open it
  
If not:
  → Right-click → Blueprint Class
  → Search: "Game Mode Base"
  → Select it → Name: "BP_GameMode"
  → Open it
```

#### **B. Set HUD Widget Class:**
```
In BP_GameMode:
Details panel (right side)
→ Find "HUD" section
→ HUD Widget Class → Dropdown
→ Select "WBP_HUD"

Compile and Save BP_GameMode
```

#### **C. Set World to Use GameMode:**
```
Top toolbar → Settings (gear icon)
OR
Window → World Settings

In World Settings panel:
→ Game Mode section
→ Game Mode Override → Select "BP_GameMode"
```

---

### **Step 9: TEST IT!**

```
Click Play button (Alt+P)
```

**You should see:**
- ✅ "Score: 0" in top-right (orange)
- ✅ Green stamina bar at bottom (full)
- ✅ Green dot + "HIDDEN" in top-left

**Test functionality:**
- Sprint (hold Shift) → Stamina drains, bar turns yellow then red
- Stop sprinting → Stamina refills, bar turns green
- Crouch (C) → Status changes to "SNEAKING" (blue)
- Stand up → Back to "HIDDEN" (green)
- Get spotted by NPC → "DETECTED!" (red)

---

## ✅ Checklist

- [ ] Unreal Editor opened
- [ ] WBP_HUD created in Content/UI
- [ ] Parent class = CatHUDWidget
- [ ] ScoreText widget created with EXACT name
- [ ] StaminaBar widget created with EXACT name
- [ ] StealthIcon widget created with EXACT name
- [ ] StealthStatusText widget created with EXACT name
- [ ] All widgets have "Is Variable" checked
- [ ] Compiled successfully (green checkmark)
- [ ] Saved
- [ ] BP_GameMode created/opened
- [ ] BP_GameMode → HUD Widget Class = WBP_HUD
- [ ] World Settings → Game Mode = BP_GameMode
- [ ] Tested in Play mode
- [ ] HUD appears and updates!

---

## 🐛 Common Issues

### **"Can't find CatHUDWidget in parent class list"**
→ Make sure the C++ code compiled (it did!)
→ Try closing and reopening Unreal Editor

### **"HUD appears but shows 'Error' or doesn't update"**
→ Check Output Log (Window → Developer Tools → Output Log)
→ Look for warnings about widget names not matching
→ Widget names are case-sensitive: `ScoreText` not `scoretext`

### **"HUD doesn't appear at all"**
→ Check BP_GameMode → HUD Widget Class is set
→ Check World Settings → Game Mode is set
→ Look at Output Log for errors

---

## 🎉 That's It!

Once you see the HUD working:
- Score updates automatically
- Stamina bar changes color
- Stealth status updates based on detection/crouching

**NO Blueprint Graph work was needed!** The C++ handles everything! 🚀

---

**Come back when you've got it working and we'll move on to the Pause Menu!** 🎮✨

