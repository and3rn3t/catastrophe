# CATastrophe: A Mischief Simulator

A 3D open-world adventure game built with **Unreal Engine 5** where you play as a mischievous cat causing havoc in a realistic house environment. Inspired by Untitled Goose Game, but with stunning 3D graphics, realistic physics, and AAA-quality gameplay.

## 🎮 Game Features

### Playable Cat Character
- **Third-person 3D control** with smooth movement and camera
- **Sprint mechanic** - Move faster but risk getting caught
- **Jump and climb** abilities for vertical exploration
- **Interactive system** - Knock over objects, scratch furniture, steal food

### Realistic 3D Environment
- Detailed house interior with multiple rooms
- Realistic physics for all interactive objects
- Dynamic lighting and shadows
- High-quality materials and textures

### Stealth Mechanics
- **AI-driven human NPCs** with vision cones and patrol routes
- Get caught if spotted while sprinting or causing mischief
- Strategic gameplay requiring timing and positioning

### Interactive Objects
- **Vases** (10 points) - Knock them off tables and shelves
- **Furniture** (15 points) - Scratch and damage household items
- **Curtains** (20 points) - Climb and shred window treatments
- **Food** (25 points) - Steal snacks from counters and tables

## 🛠️ Technology Stack

- **Unreal Engine 5.3+** - Industry-leading game engine
- **C++** - Core gameplay logic and character classes
- **Blueprints** - Visual scripting for game mechanics and AI
- **Enhanced Input System** - Modern input handling
- **AI Module** - NPC behavior and pathfinding

## 📋 Prerequisites

### Required Software
- **Unreal Engine 5.3+** - [Download from Epic Games Launcher](https://www.unrealengine.com/)
- **Visual Studio 2022** (Windows) or **Xcode** (Mac) - For C++ compilation
- **Git** - For version control

### Hardware Requirements (Minimum)
- **OS**: Windows 10/11 64-bit, macOS 11+, or Ubuntu 20.04+
- **Processor**: Quad-core Intel or AMD, 2.5 GHz+
- **Memory**: 16 GB RAM
- **Graphics**: DirectX 11/12 compatible GPU with 4GB+ VRAM
- **Storage**: 10 GB available space

### Recommended
- 32 GB RAM
- NVIDIA RTX 2070 or AMD equivalent
- SSD storage

## 🚀 Getting Started

### 1. Clone the Repository
```bash
git clone https://github.com/and3rn3t/catastrophe.git
cd catastrophe
```

### 2. Generate Project Files
**Windows:**
```bash
# Right-click Catastrophe.uproject and select "Generate Visual Studio project files"
# Or use the command line:
"%UE5_DIR%\Engine\Build\BatchFiles\Build.bat" -projectfiles -project="%CD%\Catastrophe.uproject" -game -rocket -progress
```

**Mac/Linux:**
```bash
# Generate Xcode/Makefile project
~/UnrealEngine/Engine/Build/BatchFiles/Mac/GenerateProjectFiles.sh -project="$(pwd)/Catastrophe.uproject" -game -engine
```

### 3. Build the Project
**Windows:**
- Open `Catastrophe.sln` in Visual Studio 2022
- Set build configuration to **Development Editor**
- Build solution (Ctrl+Shift+B)

**Mac:**
- Open `Catastrophe.xcworkspace` in Xcode
- Select **Development Editor** scheme
- Build (Cmd+B)

**Linux:**
```bash
make CatastropheEditor
```

### 4. Open in Unreal Editor
- Double-click `Catastrophe.uproject`
- Or launch from Epic Games Launcher

## 🎨 Setting Up the Game

### Creating the Main Level
1. In Content Browser, navigate to `Content/Maps/`
2. Create a new level or open `MainLevel`
3. Add the following actors:
   - **Player Start** - Where the cat spawns
   - **Nav Mesh Bounds Volume** - For AI navigation
   - **BP_HumanNPC** - Place 2-3 human NPCs
   - **Destructible Objects** - Vases, furniture, curtains, food

### Creating Blueprints
The C++ classes are ready, but you need to create Blueprint children in the editor:

#### BP_CatCharacter (Based on CatCharacter C++)
1. Content Browser → Right-click → Blueprint Class
2. Search for "CatCharacter" parent class
3. Name it "BP_CatCharacter"
4. Add skeletal mesh component (cat model)
5. Configure camera boom and follow camera
6. Set in GameMode as Default Pawn Class

#### BP_HumanNPC (Based on HumanNPC C++)
1. Create Blueprint from HumanNPC parent class
2. Add skeletal mesh (human character model)
3. Set up AI Controller
4. Configure patrol behavior
5. Tune detection radius and vision cone

#### Interactive Objects
Create these as regular Actors with:
- Static Mesh component
- Physics simulation enabled
- Appropriate collision settings
- Tags: "Destructible" + specific type ("Vase", "Furniture", etc.)

### Input Configuration
Input mappings are already configured in `Config/DefaultInput.ini`:
- **WASD / Arrow Keys** - Move
- **Mouse** - Look around
- **Space** - Jump
- **Left Shift** - Sprint
- **E** - Interact
- **Gamepad support** included

## 🎯 How to Play

1. **Launch the game** from Unreal Editor (Alt+P or click Play button)
2. **Move around** using WASD keys
3. **Control camera** with mouse
4. **Get close to objects** and press **E** to interact/destroy
5. **Sprint with Shift** for faster movement (but humans will catch you!)
6. **Avoid human vision cones** - visible as debug lines
7. **Earn points** for each object destroyed
8. **Don't get caught** while sprinting in sight of humans!

## 📁 Project Structure

```
Catastrophe/
├── Catastrophe.uproject          # Unreal project file
├── Source/                       # C++ source code
│   ├── Catastrophe/
│   │   ├── CatCharacter.h/.cpp          # Main player character
│   │   ├── HumanNPC.h/.cpp              # AI human NPCs
│   │   ├── CatastropheGameMode.h/.cpp   # Game mode
│   │   └── Catastrophe.Build.cs         # Build configuration
│   ├── Catastrophe.Target.cs            # Game target
│   └── CatastropheEditor.Target.cs      # Editor target
├── Content/                      # Game assets (Blueprints, materials, etc.)
│   ├── Blueprints/              # Blueprint assets
│   ├── Maps/                    # Game levels
│   ├── Materials/               # Materials and shaders
│   ├── Meshes/                  # 3D models
│   └── Textures/                # Texture files
├── Config/                       # Configuration files
│   ├── DefaultEngine.ini        # Engine settings
│   ├── DefaultGame.ini          # Game settings
│   ├── DefaultInput.ini         # Input mappings
│   └── DefaultEditor.ini        # Editor settings
└── README.md                     # This file
```

## 🔧 Development

### Key C++ Classes

#### ACatCharacter
The main playable character with:
- Movement (WASD)
- Sprint mechanic (Shift)
- Interaction system (E key)
- Score tracking
- Third-person camera setup

#### AHumanNPC
AI-controlled humans with:
- Vision cone detection
- Patrol behavior
- Cat detection logic
- Game over trigger

#### ACatastropheGameMode
Game management:
- Spawn rules
- Game over handling
- Score tracking
- Object counting

### Adding New Features

1. **Modify C++ classes** in `Source/Catastrophe/`
2. **Compile** in Visual Studio/Xcode
3. **Hot reload** in Unreal Editor or restart
4. **Extend in Blueprints** for rapid iteration

### Debugging

- Use `UE_LOG(LogTemp, Warning, TEXT("Message"))` in C++
- View logs in Output Log window (Window → Developer Tools → Output Log)
- Use Visual Studio debugger: Debug → Attach to Process → Select UE5Editor

## 🎨 Asset Guidelines

### Cat Character Model
- Skeletal mesh with animations (idle, walk, run, jump, interact)
- ~5,000-10,000 polygons for good performance
- PBR materials for realistic fur

### Human NPC Models
- Skeletal mesh with basic animations (idle, walk, look around)
- ~8,000-15,000 polygons
- Various character types for diversity

### Environment
- Modular pieces for flexible level design
- Realistic house interior aesthetics
- Optimized for real-time rendering

### Destructible Objects
- Physics-enabled static meshes
- Breaking/destruction effects (optional)
- Clear visual feedback when destroyed

## 🚀 Future Enhancements

- [ ] More rooms and open-world exploration
- [ ] Cat customization (colors, patterns, accessories)
- [ ] Achievement system
- [ ] Multiple difficulty levels
- [ ] Sound effects and music
- [ ] Particle effects for destruction
- [ ] More object types and interactions
- [ ] Save/load system
- [ ] Multiplayer co-op mode
- [ ] VR support
- [ ] Mobile/console ports

## 🤝 Contributing

Contributions are welcome! Please:
1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Open a Pull Request

## 📝 License

This project is for educational and experimental purposes.

## 🎮 Credits

Inspired by **Untitled Goose Game** by House House.
Built with **Unreal Engine 5** by Epic Games.

---

**Ready to cause some CATastrophe!** 😼💥

For questions or support, please open an issue on GitHub.

