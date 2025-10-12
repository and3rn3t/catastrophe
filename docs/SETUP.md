# Quick Setup Guide for CATastrophe

This guide will help you get CATastrophe up and running quickly.

## Step-by-Step Setup

### 1. Install Unreal Engine 5.3+
1. Download and install [Epic Games Launcher](https://www.epicgames.com/store/download)
2. Open Epic Games Launcher
3. Go to **Unreal Engine** tab
4. Click **Install Engine** and select version **5.3** or later
5. Wait for installation to complete (~40 GB download)

### 2. Install IDE
**Windows:**
- Download [Visual Studio 2022 Community](https://visualstudio.microsoft.com/)
- During installation, select "Game development with C++"
- Include "Unreal Engine installer" workload

**Mac:**
- Download [Xcode](https://apps.apple.com/us/app/xcode/id497799835) from App Store
- Open Xcode once to accept license agreement

**Linux:**
- Install build essentials: `sudo apt-get install build-essential clang`

### 3. Clone and Setup Project
```bash
# Clone repository
git clone https://github.com/and3rn3t/catastrophe.git
cd catastrophe

# Windows: Right-click Catastrophe.uproject → "Generate Visual Studio project files"
# Mac: Right-click Catastrophe.uproject → "Generate Xcode Project"
# Linux: Right-click Catastrophe.uproject → "Generate Project Files"
```

### 4. First Build
**Windows:**
1. Open `Catastrophe.sln` in Visual Studio
2. Set build configuration to **Development Editor**
3. Build solution (Ctrl+Shift+B)
4. Wait for compilation (~5-10 minutes first time)

**Mac:**
1. Open `Catastrophe.xcworkspace`
2. Select **Development Editor** scheme
3. Build (Cmd+B)

**Linux:**
```bash
make CatastropheEditor
```

### 5. Open in Unreal Editor
- Double-click `Catastrophe.uproject`
- Editor will open and compile shaders (first time may take a while)

### 6. Create Initial Content

#### Create the Cat Blueprint
1. Content Browser → Right-click in Blueprints folder
2. **Blueprint Class** → Search "CatCharacter" → Select it
3. Name: `BP_CatCharacter`
4. Open blueprint and add:
   - Skeletal Mesh Component (you can use placeholder UE mannequin for now)
   - Adjust camera boom distance if needed
5. Save and compile

#### Create Human NPC Blueprint  
1. Right-click in Blueprints folder → **Blueprint Class**
2. Search "HumanNPC" → Select it
3. Name: `BP_HumanNPC`
4. Add skeletal mesh (placeholder for now)
5. Set DetectionRadius: 1000
6. Set VisionConeAngle: 90
7. Save and compile

#### Create Interactive Objects
For each object type (Vase, Furniture, Curtain, Food):
1. Create **Blueprint Class** → Actor
2. Add **Static Mesh Component**
3. In Details panel:
   - Enable **Simulate Physics**
   - Set **Collision Presets** to **PhysicsActor**
4. Add tags in Details panel:
   - Add tag: "Destructible"
   - Add specific tag: "Vase" (or "Furniture", "Curtain", "Food")
5. Save

#### Create the Main Level
1. File → New Level → Default
2. Save as `MainLevel` in Content/Maps/
3. Add actors:
   - **Player Start** (from Place Actors panel)
   - **Nav Mesh Bounds Volume** (scale it large enough to cover level)
   - Drag **BP_HumanNPC** into level (add 2-3 instances)
   - Drag interactive objects into level
4. Add some basic geometry:
   - Cube for floor
   - Cubes for walls
   - Basic lighting
5. Build → Build All Levels
6. Save level

#### Set Default Game Mode
1. Edit → Project Settings
2. Maps & Modes → Default GameMode
3. Select **CatastropheGameMode**
4. Default Pawn Class: Select **BP_CatCharacter**
5. Close settings

### 7. Test the Game
1. Click **Play** button (or Alt+P)
2. Controls:
   - WASD - Move
   - Mouse - Look
   - Space - Jump  
   - Shift - Sprint
   - E - Interact with nearby objects

## Troubleshooting

### "Missing modules" error when opening project
- Right-click .uproject → Generate project files
- Rebuild in Visual Studio/Xcode

### C++ compile errors
- Make sure you have correct IDE installed
- Check Unreal Engine version is 5.3+
- Clean and rebuild solution

### Objects don't interact
- Check objects have "Destructible" tag
- Ensure physics is enabled on object
- Verify collision settings

### AI doesn't work
- Build navigation (Build → Build Paths)
- Check Nav Mesh Bounds Volume covers your level
- Press 'P' in viewport to visualize nav mesh

### Performance issues
- Lower Engine Scalability Settings (Settings → Engine Scalability Settings → Low/Medium)
- Disable Lumen in Project Settings if needed

## Next Steps

1. Import or create proper cat character model
2. Add animations (idle, walk, run, jump)
3. Create more detailed level environment
4. Add UI for score display
5. Implement sound effects
6. Polish visuals and gameplay

For detailed documentation, see the main [README.md](README.md).

Happy game development! 🎮😺
