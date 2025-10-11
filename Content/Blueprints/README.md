# Blueprint Assets

This directory contains Blueprint assets for CATastrophe.

## Key Blueprints to Create in Unreal Editor:

### Characters
- **BP_CatCharacter** (extends CatCharacter C++ class)
  - Add skeletal mesh for cat model
  - Configure animation blueprint
  - Tune camera settings

- **BP_HumanNPC** (extends HumanNPC C++ class)
  - Add skeletal mesh for human model
  - Configure AI behavior tree
  - Set up patrol routes

### Interactable Objects
Create these with the "Destructible" tag:

- **BP_Vase** - Tag: "Vase", "Destructible"
- **BP_Furniture** - Tag: "Furniture", "Destructible"  
- **BP_Curtain** - Tag: "Curtain", "Destructible"
- **BP_Food** - Tag: "Food", "Destructible"

All should have:
- Static mesh or skeletal mesh component
- Physics enabled on root component
- Collision set to simulate physics

### UI
- **WBP_HUD** - Main game HUD showing score and status
- **WBP_GameOver** - Game over screen with restart option

### Game Mode
- **BP_CatastropheGameMode** (extends CatastropheGameMode C++ class)
  - Configure spawn points
  - Set up game rules
