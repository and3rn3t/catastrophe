# Unreal Engine 5.6 - First Time Setup for CATastrophe

**Date:** October 11, 2025
**Project:** CATastrophe
**Engine:** Unreal Engine 5.6

---

## ✅ Prerequisites Check

### **1. Unreal Engine 5.6 Installed**

✅ **Confirmed:** You have UE 5.6 at `C:\Program Files\Epic Games\UE_5.6`

### **2. Visual Studio 2022 Installed**

✅ **Confirmed:** MSBuild found at VS 2022 Community

### **3. C++ Code Compiled**

✅ **Confirmed:** Build succeeded, DLL created

---

## 🎯 One-Time Project Setup (Do This Once)

### **Step 1: Open Project for First Time**

When you first opened `Catastrophe.uproject`, Unreal should have:

-   ✅ Compiled shaders (takes 2-5 minutes first time)
-   ✅ Generated derived data cache
-   ✅ Loaded the project

**First launch is slow - this is normal!**

---

### **Step 2: Verify C++ Module Loaded**

1. In Unreal Editor, check bottom-right corner
2. You should NOT see errors about "Failed to load module"
3. If you see "Catastrophe module loaded" → ✅ Good!

**To verify manually:**

-   **Window** → **Developer Tools** → **Modules**
-   Look for "Catastrophe" in the list
-   Should show "Loaded" status

---

### **Step 3: Configure Project Settings (Recommended)**

#### **A. Set Editor Preferences**

**Window** → **Editor Preferences:**

1. **General** → **Loading & Saving:**

    - ✅ Auto Save: Every 10 minutes
    - ✅ Auto-reload content changes

2. **General** → **Source Code:**

    - Source Code Editor: **Visual Studio 2022**

3. **Level Editor** → **Play:**
    - Play In Editor → **New Editor Window (PIE)**
    - Replication mode: **Play As Client**

#### **B. Set Project Settings**

**Edit** → **Project Settings:**

1. **Engine** → **Input:**

    - ✅ Should already have mappings from `Config/DefaultInput.ini`
    - Verify "Jump", "Sprint", "Interact" exist

2. **Maps & Modes:**

    - Default GameMode: **CatastropheGameMode** (or will be BP_CatastropheGameMode)
    - Editor Startup Map: Set once you create a level

3. **Physics:**
    - Default Gravity Z: **-980** (standard)

---

### **Step 4: Create Content Structure**

Right now your `Content/` folder is mostly empty. Create this structure:

```
Content/
├── Blueprints/       (Blueprint assets)
├── Maps/            (Game levels)
├── Characters/      (Character assets)
│   ├── Cat/        (Cat meshes/animations)
│   └── Human/      (NPC assets)
├── Materials/       (Materials)
├── Textures/        (Texture files)
└── Audio/          (Sound effects)
```

**To create folders:**

1. **Content Browser** (bottom panel)
2. Right-click → **New Folder**
3. Create the folders above

---

### **Step 5: Create Essential Blueprints**

#### **A. Create BP_CatCharacter**

This Blueprint extends your C++ `CatCharacter` class:

1. **Content Browser** → Navigate to `Content/Blueprints/`
2. Right-click → **Blueprint Class**
3. **All Classes** → Search "**CatCharacter**"
4. Select it, name it `BP_CatCharacter`
5. Open it:
    - Add **Skeletal Mesh Component** (or Capsule for now)
    - **Compile** and **Save**

#### **B. Create BP_CatastropheGameMode**

1. Content Browser → Right-click → **Blueprint Class**
2. Search "**CatastropheGameMode**"
3. Name it `BP_CatastropheGameMode`
4. Open it:
    - **Details** → **Classes** → **Default Pawn Class** = `BP_CatCharacter`
    - **Compile** and **Save**

#### **C. Set as Default GameMode**

1. **Edit** → **Project Settings** → **Maps & Modes**
2. **Default GameMode** = `BP_CatastropheGameMode`
3. Close settings

---

### **Step 6: Create a Test Level**

1. **File** → **New Level** → **Default**

    - This gives you a floor, sky, and light

2. **File** → **Save Current Level As**

    - Save to `Content/Maps/TestLevel`

3. **Edit** → **Project Settings** → **Maps & Modes**
    - **Editor Startup Map** = `TestLevel`
    - **Game Default Map** = `TestLevel`

---

### **Step 7: Add Player Start**

1. In your TestLevel, search **Place Actors** for "**Player Start**"
2. Drag it into the level (above the floor)
3. This is where you spawn when testing

---

## 🎮 Ready to Test!

Now you can:

1. Press **Alt+P** to play
2. You'll spawn as your cat character
3. Test the jump mechanics:
    - **Tap Space** = Short jump
    - **Hold Space** = High jump

---

## 🔧 Common First-Time Issues

### **Issue 1: "No GameMode Active"**

**Symptom:** Press Alt+P, nothing happens
**Fix:**

1. World Settings → GameMode Override → Set to `BP_CatastropheGameMode`
2. Or create BP_CatastropheGameMode (see Step 5B above)

### **Issue 2: "Can't Find CatCharacter Class"**

**Symptom:** Blueprint creation fails
**Fix:**

1. Close Unreal Editor
2. Rebuild C++ code: `Ctrl+Shift+B` in VS Code
3. Reopen editor

### **Issue 3: Fall Through Floor**

**Symptom:** Character falls endlessly
**Fix:**

1. Select the floor
2. Details → Collision → Set to "Block All"

### **Issue 4: Can't See Anything (Black Screen)**

**Symptom:** Viewport is black in PIE
**Fix:**

1. Add **Directional Light** to level
2. Add **Sky Atmosphere** to level
3. Or use File → New Level → **Default** (has everything)

### **Issue 5: Shaders Compiling Forever**

**Symptom:** Progress bar stuck at "Compiling Shaders"
**Fix:**

-   This is normal on first launch!
-   Can take 5-15 minutes depending on PC
-   Let it finish, subsequent launches are fast

---

## 📊 Performance Optimization (Optional)

### **For Faster Editor Performance:**

1. **Edit** → **Editor Preferences** → **General** → **Performance:**

    - Use Less CPU when in Background: ✅ Enabled
    - Monitor Editor Performance: ✅ Enabled

2. **Window** → **Developer Tools** → **Project Launcher:**

    - Close if you don't need it

3. **Exclude from Windows Defender:**
    ```powershell
    # Run as Administrator
    Add-MpPreference -ExclusionPath "C:\git\catastrophe"
    Add-MpPreference -ExclusionPath "C:\Program Files\Epic Games\UE_5.6"
    ```

---

## ✅ Setup Complete Checklist

Use this to verify everything is ready:

-   [ ] Unreal Engine 5.6 opens `Catastrophe.uproject` without errors
-   [ ] Bottom-right shows "Catastrophe" module loaded
-   [ ] Content folders created (Blueprints, Maps, etc.)
-   [ ] `BP_CatCharacter` created from CatCharacter C++ class
-   [ ] `BP_CatastropheGameMode` created and set as default
-   [ ] Test level created with floor, light, and Player Start
-   [ ] Press Alt+P → Game starts, can move with WASD
-   [ ] Space key makes character jump
-   [ ] Holding Space = higher jump than tapping
-   [ ] No errors in Output Log

---

## 🎯 You're All Set!

**Minimal setup to test jump mechanics:**

1. ✅ Project opens in UE5.6
2. ✅ Create TestLevel (File → New Level → Default)
3. ✅ Press Alt+P
4. ✅ Test jumping (tap vs hold Space)

**Full setup for development:**

1. Complete all steps above
2. Create BP_CatCharacter with mesh
3. Set up materials and animations
4. Build out full levels

---

## 📚 Next Steps

**After testing jump mechanics:**

1. Review `CURRENT_SPRINT.md` for next features
2. Implement climbing, stamina, or crouch systems
3. Add cat mesh and animations
4. Build out house environment

---

**Need Help?**

-   Check **Window** → **Developer Tools** → **Output Log** for errors
-   Use **Help** → **Documentation** in Unreal Editor
-   All C++ code is in `Source/Catastrophe/`

---

**Updated:** October 11, 2025
**Status:** Ready for Testing ✅
