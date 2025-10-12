# CATastrophe - AI Coding Agent Instructions

## Project Overview

CATastrophe is a **3D open-world mischief simulator** built with Unreal Engine 5.3+ where players control a cat causing havoc in a realistic house. This is a C++/Blueprint hybrid UE5 project inspired by Untitled Goose Game but in 3D.

**Current Phase:** Phase 1 MVP Polish (see `CURRENT_SPRINT.md`)

## Architecture & Critical Design Decisions

### C++/Blueprint Hybrid Pattern

The project uses a **C++ foundation with Blueprint extension** model:

- **C++ classes** (`Source/Catastrophe/`) define core gameplay logic, properties, and methods
- **Blueprint children** (created in UE5 Editor) extend C++ classes with visual mesh, animations, and designer-tunable parameters
- C++ classes expose properties with `UPROPERTY(EditAnywhere, BlueprintReadWrite)` for Blueprint access
- Functions marked `BlueprintCallable` or `BlueprintImplementableEvent` enable Blueprint integration

**Example:** `CatCharacter.h` defines movement logic in C++, but `BP_CatCharacter` (Blueprint child) adds skeletal mesh, animations, and specific camera settings.

### Key Components

1. **ACatCharacter** (`CatCharacter.h/cpp`) - Player-controlled cat with third-person camera
   - Uses Spring Arm + Camera Component for third-person view
   - Manages movement (walk/sprint), jumping, and interaction
   - Tracks `MischiefScore` as core gameplay metric
   - Sprint toggles between `WalkSpeed` (400) and `SprintSpeed` (800)

2. **AHumanNPC** (`HumanNPC.h/cpp`) - AI opponent with vision-based detection
   - Implements cone-based vision detection (`DetectionRadius`, `VisionConeAngle`)
   - Uses `CanSeeCat()` for line-of-sight checks
   - Blueprint events `OnCatDetected`/`OnCatEscaped` trigger AI behavior
   - Requires NavMesh for patrol routes

3. **ACatastropheGameMode** (`CatastropheGameMode.h/cpp`) - Game flow controller
   - Handles game-over state via `OnCatCaught()`
   - Tracks destructible objects for scoring

4. **Interactive Objects** - Physics-enabled actors with tags
   - Must have tag "Destructible" + type tag ("Vase", "Furniture", "Curtain", "Food")
   - Points: Vase (10), Furniture (15), Curtain (20), Food (25)
   - Physics simulation enabled for realistic destruction

### Module Dependencies

From `Catastrophe.Build.cs`, the project requires:
- **EnhancedInput** - Modern input system (not legacy input)
- **AIModule** - AI behavior trees and perception
- **NavigationSystem** - NavMesh for NPC pathfinding
- **GameplayTasks** - Async AI task execution

## Critical Developer Workflows

### Building the Project

**Windows (Primary Platform):**
```bash
# Use VS Code tasks (preferred)
Ctrl+Shift+B → "Build Catastrophe (Development Editor)"

# Or Unreal Build Tool directly (recommended over MSBuild)
& "C:\Program Files\Epic Games\UE_5.6\Engine\Build\BatchFiles\Build.bat" `
  CatastropheEditor Win64 Development "$PWD\Catastrophe.uproject"
```

**Note:** Use Unreal's Build.bat (not MSBuild) to avoid building unnecessary engine tools that require .NET SDK.

**Important:** Always build with `Development` configuration for editor workflows (not `Debug` or `Shipping`).

### Regenerating Project Files

**Required after:**
- Adding/removing C++ classes
- Modifying `.uproject` file
- Changing module dependencies in `.Build.cs`

**Command:**
```bash
"%UE5_DIR%\Engine\Build\BatchFiles\Build.bat" -projectfiles -project="%CD%\Catastrophe.uproject" -game -rocket -progress
```

Or use VS Code task: "Generate Project Files"

### Testing in Editor

1. Build C++ code first (Ctrl+Shift+B)
2. Open `Catastrophe.uproject` in UE5 Editor
3. Press **Alt+P** or click Play button
4. Test controls: WASD (move), Space (jump), Shift (sprint), E (interact)

**Blueprint Setup Required:** C++ classes work but need Blueprint children (`BP_CatCharacter`, `BP_HumanNPC`) created in Editor with meshes/animations before gameplay works visually.

## Project-Specific Conventions

### Naming Conventions

- **C++ Classes:** Prefix with game name initial - `ACatCharacter`, `AHumanNPC` (Actor), `UCatGameInstance` (UObject)
- **Blueprint Assets:** Prefix with `BP_` - `BP_CatCharacter`, `BP_HumanNPC`
- **Maps/Levels:** Descriptive names - `MainLevel`, `TestArena`
- **Interactive Objects:** Tag-based system - no special prefixes, rely on Actor tags

### Input System

Uses **Enhanced Input System** (UE5 modern approach), NOT legacy input binding:
- Input mappings in `Config/DefaultInput.ini`
- Bound in C++ via `SetupPlayerInputComponent()`
- Action mappings: Jump, Sprint, Interact
- Axis mappings: MoveForward, MoveRight, Turn, LookUp
- Full gamepad support included

### Physics & Collision

Interactive objects require:
- Static Mesh with `SimulatePhysics = true`
- Collision preset allowing overlap with Pawn
- Tags for type identification
- Consider mass/damping for realistic behavior

### AI Detection Pattern

NPCs use **manual vision cone checking** (not UE5 Perception System):
```cpp
// In HumanNPC::Tick() or custom timer
if (CanSeeCat()) {
    OnCatDetected(); // Blueprint implementable
    // Trigger chase/alert behavior
}
```

This approach allows for game-specific detection tuning vs. perception system overhead.

## Common Pitfalls & Solutions

1. **"Can't find C++ class in Blueprint parent"** → Rebuild C++ project, then restart Editor
2. **NavMesh not working** → Place `NavMeshBoundsVolume` in level and press 'P' to visualize
3. **Input not responding** → Check GameMode has `BP_CatCharacter` as Default Pawn Class
4. **Objects don't break** → Verify "Destructible" tag exists and `SimulatePhysics = true`
5. **Sprint not working** → Ensure `CharacterMovement->MaxWalkSpeed` is updated in code

## File Organization

```
Source/Catastrophe/         # C++ gameplay code
├── Catastrophe.Build.cs    # Module dependencies
├── CatCharacter.h/cpp      # Player character
├── HumanNPC.h/cpp          # AI opponent
└── CatastropheGameMode.h/cpp # Game flow

Content/                    # Unreal assets (managed in Editor)
├── Blueprints/            # BP children of C++ classes
└── Maps/                  # Game levels

Config/                    # Engine/game configuration
├── DefaultInput.ini       # Input bindings
├── DefaultEngine.ini      # Engine settings
└── DefaultGame.ini        # Game-specific settings
```

## Documentation References

- **Architecture:** `DESIGN.md` - Visual design and gameplay systems
- **Setup:** `README.md` - Installation and quick start
- **Development:** `DEV_TOOLS.md` - Editor setup and tooling
- **Roadmap:** `ROADMAP.md` + `CURRENT_SPRINT.md` - Feature planning

## When Adding New Features

1. **C++ Changes:**
   - Update `.h` with `UPROPERTY`/`UFUNCTION` declarations
   - Implement in `.cpp`
   - Rebuild project (Ctrl+Shift+B)
   - Restart UE5 Editor to see changes

2. **Blueprint Changes:**
   - Create Blueprint child of C++ class
   - Add visual components (mesh, particles, etc.)
   - Override BlueprintImplementableEvents if needed
   - Test in Editor (Alt+P)

3. **New Modules:**
   - Add to `PublicDependencyModuleNames` in `Catastrophe.Build.cs`
   - Regenerate project files
   - Clean + rebuild solution

## Creating Blueprint Assets in UE5 Editor

Blueprint children of C++ classes are created in the Unreal Editor (NOT in code):

### BP_CatCharacter (From CatCharacter C++ Parent)

1. **Content Browser** → Right-click in `Content/Blueprints/`
2. **Blueprint Class** → Search parent: "CatCharacter"
3. Name: `BP_CatCharacter`
4. Open blueprint, add components:
   - **Skeletal Mesh** (cat model) - attach to Capsule Component
   - Camera Boom & Follow Camera already exist from C++
5. **Class Defaults** panel:
   - Set `WalkSpeed = 400`, `SprintSpeed = 800` (or leave C++ defaults)
   - Tune `CameraBoom->TargetArmLength` for desired camera distance
6. **Compile & Save**
7. **Set as Default Pawn:** Open `BP_CatastropheGameMode` → Set Default Pawn Class to `BP_CatCharacter`

### BP_HumanNPC (From HumanNPC C++ Parent)

1. Create Blueprint from `HumanNPC` parent class
2. Add **Skeletal Mesh** component (human character model)
3. **AI Controller:** In Class Defaults, set AI Controller Class to `AIController` (or custom BP_HumanAIController)
4. Tune detection values:
   - `DetectionRadius = 1000` (10 meters)
   - `VisionConeAngle = 90` (degrees)
5. **Implement Blueprint Events:**
   - Override `OnCatDetected` - Add behavior (play animation, chase, alert sound)
   - Override `OnCatEscaped` - Return to patrol
6. **Optional:** Create Behavior Tree + Blackboard for advanced AI (see AI section below)

### Interactive Objects (Physics-Based Actors)

Example: `BP_Vase`

1. **Content Browser** → Blueprint Class → Actor
2. Add **Static Mesh Component** (vase model)
3. **Details Panel:**
   - Physics → **Simulate Physics = true**
   - Collision → **Collision Presets = PhysicsActor** (or custom allowing Pawn overlap)
4. **Tags:** Click "Tags" dropdown, add:
   - `Destructible` (required for scoring system)
   - `Vase` (required for point value)
5. **Optional:** Add particle system component for break effect (spawn on destroy)
6. Repeat for `BP_Furniture` (tag: "Furniture"), `BP_Curtain` (tag: "Curtain"), `BP_Food` (tag: "Food")

**Point Values (handled in C++):**
- Vase = 10, Furniture = 15, Curtain = 20, Food = 25

## AI Behavior Tree Pattern (Future Enhancement)

Current `HumanNPC` uses simple `Tick()` detection. For advanced AI, transition to Behavior Trees:

### Setup Steps:

1. **Create AI Controller:**
   - Blueprint → AIController → `BP_HumanAIController`
   - Set in `BP_HumanNPC` Class Defaults

2. **Create Behavior Tree:**
   - Right-click → Artificial Intelligence → Behavior Tree → `BT_HumanPatrol`
   - Blackboard asset → `BB_HumanNPC`

3. **Blackboard Keys:**
   - `PatrolLocation` (Vector) - Next waypoint
   - `CatLocation` (Vector) - Last known cat position
   - `bCatDetected` (Bool) - Detection state
   - `DetectionRadius` (Float) - From C++ variable

4. **Behavior Tree Structure:**
   ```
   Root
   ├── Sequence: Patrol
   │   ├── Get Random Patrol Point
   │   ├── Move To (PatrolLocation)
   │   └── Wait (2-5 seconds)
   ├── Selector: Detect Cat
   │   ├── Check Detection (Custom Task)
   │   └── Chase Cat (if detected)
   └── Return to Patrol
   ```

5. **Custom BT Tasks** (C++ recommended):
   - `BTTask_CheckForCat` - Calls `HumanNPC->CanSeeCat()`, updates Blackboard
   - `BTTask_ChaseCat` - Move toward cat, trigger catch if within range

6. **Integration:** In `HumanNPC.cpp`, instead of `Tick()` detection, update Blackboard values that drive BT logic

## Content Pipeline & Asset Conventions

### Importing Assets

**Skeletal Meshes (Characters):**
- Import location: `Content/Characters/Cat/` or `Content/Characters/Human/`
- Format: FBX with skeleton and animations
- Naming: `SK_CatBody`, `SK_HumanMale`, `SK_HumanFemale`
- Auto-create Physics Asset on import (for ragdoll)

**Static Meshes (Objects):**
- Import location: `Content/Props/` (subdivide by room: `Kitchen/`, `LivingRoom/`)
- Naming: `SM_Vase01`, `SM_Table01`, `SM_Curtain`
- Enable collision generation or import custom collision meshes

**Textures & Materials:**
- Location: `Content/Materials/` and `Content/Textures/`
- Naming: `T_ObjectName_BaseColor`, `T_ObjectName_Normal`, `M_ObjectName`
- Use PBR workflow: Base Color, Normal, Roughness, Metallic

**Animations:**
- Location: `Content/Characters/Cat/Animations/` or `Content/Characters/Human/Animations/`
- Naming: `Anim_Cat_Idle`, `Anim_Cat_Walk`, `Anim_Human_PatrolWalk`
- Create Animation Blueprint: `ABP_Cat`, `ABP_Human`

### Material Setup for Physics Objects

Interactive objects need proper material setup:

1. Create Material Instance Dynamic in Blueprint (optional for destruction effects)
2. Set Physical Material for collision sounds:
   - **Vase/Glass:** `PM_Glass` - high-pitched shatter
   - **Wood Furniture:** `PM_Wood` - thud sound
   - **Fabric Curtains:** `PM_Cloth` - soft rustle

## Testing Strategy

### Unit Testing C++ (Future)

- Use Unreal's Automation System: `IMPLEMENT_SIMPLE_AUTOMATION_TEST`
- Test location: `Source/Catastrophe/Tests/`
- Example: Test `CatCharacter->AddMischiefScore()` logic

### Gameplay Testing in Editor

**Quick Test Map: `Content/Maps/TestArena`**
1. Create minimal level with:
   - Player Start
   - NavMeshBoundsVolume
   - 1x `BP_HumanNPC`
   - 5-10 destructible objects
2. Use for rapid iteration on mechanics (no need to load full MainLevel)

**Debug Visualization (Already Implemented):**
- `HumanNPC` draws red debug lines to cat when detected
- Enable in editor: `Show → Developer → Visualize AI` (or debug console: `ShowDebug AI`)

**Testing Checklist for New Features:**
1. Build C++ code (Ctrl+Shift+B)
2. Open Editor, load TestArena
3. PIE (Play in Editor) - Alt+P
4. Test controls and new mechanic
5. Check console output for `UE_LOG` messages
6. Verify Blueprint events fire correctly (add Print String nodes)
7. Test in MainLevel once TestArena validation passes

**Performance Testing:**
- Editor: `stat FPS`, `stat Unit`, `stat Game` (console commands)
- Profile with Unreal Insights: Session Frontend → Start Trace

## Version Control & Binary Assets

### .gitignore Strategy

Current `.gitignore` includes:
- **Tracked:** `Content/` (Blueprint assets, maps, materials)
- **Ignored:** `Binaries/`, `Intermediate/`, `Saved/`, `DerivedDataCache/`, `.vs/`, `.sln`

**Important:** C++ generated files (`.sln`, `.vcxproj`) are ignored because they regenerate via "Generate Project Files" command.

### Git LFS for Large Assets (Recommended Setup)

**Not currently configured**, but recommended for production:

```bash
# Install Git LFS
git lfs install

# Track large binary files
git lfs track "*.uasset"
git lfs track "*.umap"
git lfs track "*.fbx"
git lfs track "*.png"
git lfs track "*.tga"
git lfs track "*.wav"
git lfs track "*.mp3"

# Commit .gitattributes
git add .gitattributes
git commit -m "Configure Git LFS for Unreal assets"
```

**Why LFS:** Unreal `.uasset` and `.umap` files are binary and can grow large. LFS stores them efficiently on remote servers, keeping local repo lightweight.

### Merge Conflicts with Binary Files

**Blueprints/Maps (.uasset, .umap):**
- **Cannot be manually merged** - pick one version
- Strategy: Use Unreal's built-in `Diff` tool for visual comparison
- Right-click asset → Source Control → Diff Against Depot
- Resolve by accepting one side (yours/theirs) and manually re-apply changes in editor

**Best Practice:**
- Avoid concurrent edits to same Blueprint/Map
- Use CODEOWNERS or lock files during major level design work
- Break large maps into sub-levels for parallel work

## Current Sprint Focus

Per `CURRENT_SPRINT.md`, Milestone 1.1 priorities:
- Jump mechanics improvements
- Climbing system implementation
- Pounce/attack animations
- Crouching/stealth mode
- Tail physics
- Sprint stamina system

When implementing features, maintain the C++/Blueprint separation: logic in C++, visual/tunable in BP.

### Implementation Pattern Example: Climbing System

**C++ (CatCharacter.h/cpp):**
```cpp
// Add to CatCharacter.h
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cat Stats")
bool bIsClimbing;

UFUNCTION(BlueprintCallable, Category = "Movement")
void StartClimbing();

UFUNCTION(BlueprintCallable, Category = "Movement")
void StopClimbing();
```

**Blueprint (BP_CatCharacter):**
- Override `StartClimbing` event → Play climbing animation
- Add collision detection for "Climbable" tagged objects (curtains, furniture)
- Tune climb speed and camera angle in BP

**Level Setup:**
- Tag curtain/furniture actors with "Climbable" in addition to "Destructible"
