# CATastrophe - Quick Reference Guide

## 🚀 Quick Start for Developers

New to the project? Start here!

### 1. Essential Reading (Priority Order)

1. **[README.md](README.md)** - Project overview and setup (15 min)
2. **[DEV_TOOLS.md](DEV_TOOLS.md)** - Development tools setup (10 min) ⭐
3. **[ROADMAP.md](ROADMAP.md)** - Development plan and priorities (20 min)
4. **[CONTRIBUTING.md](CONTRIBUTING.md)** - How to contribute (10 min)
5. **[SETUP.md](SETUP.md)** - Quick setup guide (5 min)
6. **[DESIGN.md](DESIGN.md)** - Visual design reference (10 min)

Total reading time: ~70 minutes

---

## 📁 Repository Structure

```text
catastrophe/
├── Source/Catastrophe/          # C++ source code
│   ├── CatCharacter.cpp/.h      # Player character
│   ├── HumanNPC.cpp/.h          # AI NPCs
│   ├── CatastropheGameMode.*   # Game mode
│   └── Catastrophe.Build.cs    # Build config
├── Content/                      # Unreal assets
│   ├── Blueprints/              # Blueprint assets
│   ├── Maps/                    # Game levels
│   ├── Materials/               # Materials
│   ├── Meshes/                  # 3D models
│   └── Textures/                # Textures
├── Config/                       # Configuration files
├── .vscode/                      # VS Code configuration ⭐
├── .github/                      # GitHub workflows & templates ⭐
├── ROADMAP.md                    # Development roadmap
├── CONTRIBUTING.md               # Contribution guide
├── DEV_TOOLS.md                  # Dev tools setup ⭐
├── README.md                     # Main documentation
├── SETUP.md                      # Quick setup
├── CHANGELOG.md                  # Change history ⭐
└── DESIGN.md                     # Design document
```

---

## 🎯 Current Development Phase

Check [ROADMAP.md](ROADMAP.md) for current phase!

### How to Find Tasks

1. Open [ROADMAP.md](ROADMAP.md)
2. Find the current phase
3. Look for unchecked `[ ]` items
4. Pick something matching your skills
5. Create a branch and start coding!

---

## 💻 Common Development Commands

### Building the Project

**Windows (Visual Studio):**

```bash
# Generate project files
"%UE5_DIR%\Engine\Build\BatchFiles\Build.bat" -projectfiles

# Build from command line
MSBuild Catastrophe.sln /p:Configuration="Development Editor"
```

**Mac/Linux:**

```bash
# Generate project files
~/UnrealEngine/Engine/Build/BatchFiles/Mac/GenerateProjectFiles.sh

# Build
make CatastropheEditor
```

### Testing

**Play In Editor (PIE):**

- Press `Alt+P` in Unreal Editor
- Or click the "Play" button

**Run Tests:**

- Currently manual testing
- See [CONTRIBUTING.md](CONTRIBUTING.md) for testing checklist

### Version Control

```bash
# Create feature branch
git checkout -b feature/your-feature-name

# Commit changes
git add .
git commit -m "Brief description

Detailed explanation if needed
Reference: Roadmap Phase X, Milestone X.X"

# Push and create PR
git push origin feature/your-feature-name
```

---

## 🎮 Key Classes Overview

### ACatCharacter (Player)

**Location:** `Source/Catastrophe/CatCharacter.cpp`

**Key Functions:**

- `MoveForward(float Value)` - Forward/backward movement
- `MoveRight(float Value)` - Left/right movement
- `StartSprinting()` / `StopSprinting()` - Sprint toggle
- `Interact()` - Object interaction (E key)
- `AddMischiefScore(float Points)` - Score tracking

**Key Properties:**

- `MischiefScore` - Current score
- `WalkSpeed` / `SprintSpeed` - Movement speeds
- `CameraBoom` - Camera arm component
- `FollowCamera` - Third-person camera

### AHumanNPC (AI)

**Location:** `Source/Catastrophe/HumanNPC.cpp`

**Key Functions:**

- `Tick()` - Detection and AI updates
- `DetectCat()` - Vision cone detection
- `CanSeeCat()` - Line of sight check

**Key Properties:**

- `DetectionRadius` - How far NPCs can see
- `VisionConeAngle` - Field of view angle
- `bCatDetected` - Detection state

### ACatastropheGameMode

**Location:** `Source/Catastrophe/CatastropheGameMode.cpp`

**Key Functions:**

- `OnCatCaught()` - Handle game over
- `GetTotalDestructibleObjects()` - Count objects

---

## 🎨 Asset Creation Quick Reference

### Naming Conventions

- **Blueprints:** `BP_Name`
- **Materials:** `M_Name` or `MI_Name` (instance)
- **Textures:** `T_Name`
- **Static Meshes:** `SM_Name`
- **Skeletal Meshes:** `SK_Name`
- **Animations:** `A_Name`
- **Sounds:** `S_Name`

### Performance Targets

- **Cat Model:** 8,000-12,000 tris
- **Human NPC:** 15,000-20,000 tris
- **Props:** 500-5,000 tris
- **Textures:** Max 4K (2K for most props)
- **Target FPS:** 60 FPS on GTX 1660/RX 580

### Required Tags for Objects

- `"Destructible"` - For all interactive objects
- `"Vase"` - 10 points
- `"Furniture"` - 15 points
- `"Curtain"` - 20 points
- `"Food"` - 25 points

---

## 🐛 Debugging Tips

### Logging in C++

```cpp
// Basic log
UE_LOG(LogTemp, Warning, TEXT("Cat position: %s"), 
       *GetActorLocation().ToString());

// Conditional log
if (bIsSprinting)
{
    UE_LOG(LogTemp, Log, TEXT("Cat is sprinting!"));
}
```

### Visual Debugging

```cpp
// Draw debug sphere
DrawDebugSphere(GetWorld(), Location, Radius, 12, FColor::Green, false, 1.0f);

// Draw debug line
DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f);
```

### Common Issues

**"Missing modules" error:**

```bash
# Right-click .uproject → Generate Visual Studio project files
# Then rebuild
```

**Physics not working:**

- Check "Simulate Physics" is enabled
- Verify collision presets are correct
- Ensure object has "Destructible" tag

**AI not moving:**

- Build navigation: Build → Build Paths
- Press `P` in viewport to visualize nav mesh
- Check Nav Mesh Bounds Volume covers level

---

## 🧪 Testing Checklist

Before committing, verify:

- [ ] Project builds without errors
- [ ] No new warnings in Output Log
- [ ] Cat movement still works (WASD)
- [ ] Camera follows properly
- [ ] Sprint mechanic works (Shift)
- [ ] Interaction works (E key)
- [ ] Score tracking updates
- [ ] NPCs detect cat when appropriate
- [ ] Game over triggers correctly
- [ ] No performance regression

---

## 📚 Helpful Resources

### Unreal Engine

- [UE5 Documentation](https://docs.unrealengine.com/5.3/)
- [C++ API Reference](https://docs.unrealengine.com/5.3/en-US/API/)
- [Blueprint Documentation](https://docs.unrealengine.com/5.3/en-US/blueprints-visual-scripting-in-unreal-engine/)
- [Unreal Slackers Discord](https://unrealslackers.org/)

### Game Development

- [Game Programming Patterns](https://gameprogrammingpatterns.com/)
- [Unreal Engine Community Wiki](https://unrealcommunity.wiki/)

### Project Specific

- [GitHub Issues](https://github.com/and3rn3t/catastrophe/issues) - Bug reports & features
- [Roadmap](ROADMAP.md) - Development plan
- [Contributing Guide](CONTRIBUTING.md) - How to help

---

## 💡 Quick Tips

### For Programmers

- Use `UPROPERTY()` macros for properties you want in editor
- Prefix member variables with letter indicating type (b for bool, etc.)
- Keep tick functions efficient - they run every frame
- Use `UE_LOG` instead of `printf` for debugging

### For Artists

- Test assets in-engine early and often
- Use LODs for better performance
- Follow PBR workflow for materials
- Keep polycount within targets

### For Designers

- Playtest frequently
- Get feedback from others
- Balance is iterative - start conservative
- Document your design decisions

### For Everyone

- Commit often with clear messages
- Reference roadmap milestones
- Ask questions - no question is dumb!
- Have fun and be creative! 😼

---

## 🎯 Next Steps

1. **Set up your environment** - Follow [SETUP.md](SETUP.md)
2. **Pick a task** - Check [ROADMAP.md](ROADMAP.md)
3. **Read contributing guide** - See [CONTRIBUTING.md](CONTRIBUTING.md)
4. **Start coding!** - Make something awesome
5. **Submit a PR** - Share your work

---

## ❓ Need Help?

- **Setup issues?** Check [SETUP.md](SETUP.md)
- **Code questions?** Open a Discussion
- **Found a bug?** Create an Issue
- **Want to contribute?** Read [CONTRIBUTING.md](CONTRIBUTING.md)

---

**Welcome to the team! Let's make CATastrophe amazing!** 😼💥
