# CATastrophe: A Mischief Simulator

A 3D open-world adventure game built with **Unreal Engine 5** where you play as a mischievous cat causing havoc in a realistic house environment. Inspired by Untitled Goose Game, but with stunning 3D graphics, realistic physics, and AAA-quality gameplay.

📍 **[Product Roadmap](ROADMAP.md)** | 📖 **[Documentation Index](DOCS_INDEX.md)** | 🚀 **[Quick Start](QUICKSTART.md)**

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

### 5. Set Up Development Tools (Recommended)

For the best development experience, we provide comprehensive tooling:

```bash
# Optional: Install pre-commit hooks for automated code quality checks
pip install pre-commit
pre-commit install

# Open in VS Code with our workspace configuration
code Catastrophe.code-workspace
```

**See [DEV_TOOLS.md](DEV_TOOLS.md) for detailed setup of:**
- VS Code extensions and configuration
- Code formatting (EditorConfig, Clang-Format)
- GitHub Actions CI/CD
- Pre-commit hooks
- Debugging setup

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

See our comprehensive [**Product Roadmap**](ROADMAP.md) for detailed development plans and timelines!

### Near-Term Goals (Phases 1-2)
- [ ] Enhanced cat mechanics (jump, climb, pounce)
- [ ] Improved AI with behavior trees
- [ ] Complete UI/UX system
- [ ] Sound effects and music
- [ ] Multi-room house environment
- [ ] Particle effects for destruction
- [ ] More object types and interactions

### Mid-Term Goals (Phases 3-4)
- [ ] Mission and objective system
- [ ] Achievement system (50+ achievements)
- [ ] Cat customization (colors, patterns, accessories)
- [ ] Multiple difficulty levels and game modes
- [ ] Save/load system
- [ ] Performance optimization

### Long-Term Goals (Phase 5+)
- [ ] Multiplayer co-op mode
- [ ] VR support
- [ ] Mobile/console ports
- [ ] Seasonal content updates
- [ ] Modding support

👉 **Check out [ROADMAP.md](ROADMAP.md) for the complete 12-month development plan!**

## 🤝 Contributing

Contributions are welcome! We're following a structured development roadmap to build CATastrophe into an amazing game.

### How to Contribute

1. **Check the [Roadmap](ROADMAP.md)** - See what phase we're in and what features are planned
2. **Pick a Task** - Choose something from the current phase that matches your skills
3. **Fork the Repository** - Create your own copy
4. **Create a Feature Branch** - `git checkout -b feature/your-feature-name`
5. **Make Your Changes** - Follow our coding standards (see below)
6. **Test Thoroughly** - Ensure your changes work and don't break existing features
7. **Commit Your Changes** - Use clear, descriptive commit messages
8. **Push to Your Branch** - `git push origin feature/your-feature-name`
9. **Open a Pull Request** - Describe your changes and reference roadmap milestones

### Development Guidelines

- **C++ Code**: Follow Unreal Engine coding standards (enforced by `.clang-format`)
- **Blueprints**: Keep them organized and well-commented
- **Commits**: Use conventional commit format (`feat:`, `fix:`, `docs:`, etc.)
- **Branch Naming**: Use prefixes like `feature/`, `bugfix/`, `docs/`
- **Testing**: Test on multiple hardware configurations if possible
- **Documentation**: Update docs when adding new features
- **Code Quality**: Use provided pre-commit hooks and linters

**See [DEV_TOOLS.md](DEV_TOOLS.md) for complete development tools setup and [CONTRIBUTING.md](CONTRIBUTING.md) for detailed guidelines.**

### Areas We Need Help

Check [ROADMAP.md](ROADMAP.md) for current priorities! Common contribution areas:
- 🎨 **3D Art**: Character models, environment assets, textures
- 🎵 **Audio**: Sound effects, music tracks, voice acting
- 💻 **Programming**: Gameplay features, AI, optimization
- 🎮 **Level Design**: Room layouts, object placement
- 📝 **Documentation**: Tutorials, guides, localization
- 🧪 **Testing**: Bug reports, QA, playtest feedback

### Questions?

Open an issue or discussion on GitHub!

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

This project is for educational and experimental purposes.

## 🎮 Credits

Inspired by **Untitled Goose Game** by House House.
Built with **Unreal Engine 5** by Epic Games.

---

**Ready to cause some CATastrophe!** 😼💥

For questions or support, please open an issue on GitHub.

