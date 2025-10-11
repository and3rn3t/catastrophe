# Maps

## MainLevel
The primary game level for CATastrophe. This should include:

### Environment Setup
- A house interior with multiple rooms
- Furniture, decorations, and interactive objects
- Good lighting and atmosphere

### Required Actors
1. **Player Start** - Spawn point for the cat character
2. **Nav Mesh Bounds Volume** - For AI navigation
3. **BP_HumanNPC** instances - Place 2-3 humans with patrol routes
4. **Destructible Objects** - Vases, furniture, curtains, food items scattered around

### Lighting
- Directional Light for main illumination
- Point Lights for indoor areas
- Post Process Volume for color grading

### Navigation
- Set up Nav Mesh for AI pathing
- Ensure all walkable surfaces are navigable

## Creating the Level
1. Open Unreal Editor
2. Create new level: File → New Level → Default
3. Save as "MainLevel" in Content/Maps/
4. Add and configure the actors listed above
5. Build lighting and navigation
