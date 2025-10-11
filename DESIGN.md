# CATastrophe - Visual Design Document

## Game Overview
CATastrophe is a 3D open-world mischief simulator where players control a realistic cat in a fully-realized 3D house environment.

## Visual Style & Art Direction

### Target Visual Quality
- **AAA 3D Graphics**: Realistic rendering with Unreal Engine 5
- **Physically-Based Rendering (PBR)**: Realistic materials and lighting
- **Dynamic Lighting**: Real-time shadows and indirect lighting
- **High-Quality Assets**: Detailed character models and environments

## Character Design

### The Cat (Player Character)
**Visual Style:**
- Realistic cat proportions and anatomy
- Soft, detailed fur using Unreal's fur shader system
- Various color patterns (tabby, calico, black, white, etc.)
- Expressive animations (idle, walk, run, jump, crouch, pounce, stretch)
- Tail physics for natural movement
- Whisker details

**Technical Specs:**
- Skeletal mesh with ~8,000-12,000 polygons
- High-resolution texture maps (4K for main body)
- Physics-based fur rendering
- Blend shapes for facial expressions
- Third-person camera following cat

**Animation Set:**
- Idle breathing and blinking
- Walk cycle (4-legged)
- Run/sprint cycle
- Jump (anticipation, airborne, landing)
- Interact animations (paw swipe, bite, push)
- Emotional states (content, alert, mischievous)

### Human NPCs
**Visual Style:**
- Realistic human proportions
- Casual home clothing (comfortable, lived-in look)
- Various character types (different ages, styles)
- Animated faces with expression

**Technical Specs:**
- Skeletal mesh ~15,000-20,000 polygons
- PBR materials for clothing and skin
- Basic animation set (idle, walk, look around, react)

**Animations:**
- Idle standing/sitting
- Walking patrol routes
- Head turning (scanning for cat)
- Surprise/reaction when detecting cat
- Scolding gesture when catching cat

## Environment Design

### House Interior Layout
**Living Room:**
- Sofa and coffee table
- TV stand with vases and decorations
- Curtains on windows
- Bookshelves with knockable items
- Floor lamp
- Carpet/rug

**Kitchen:**
- Counters with food items
- Cabinets (some open)
- Table with chairs
- Sink area
- Appliances (fridge, stove)
- Food bowls for cat

**Bedroom:**
- Bed with pillows (for jumping on)
- Dresser with items
- Closet with hanging clothes
- Nightstands with lamps
- Curtains

**Hallway:**
- Pictures on walls (can be knocked askew)
- Decorative items on shelves
- Runner rugs

### Interactive Objects

#### Vases (10 points)
- Crystal or ceramic appearance
- PBR materials with reflections
- Physics-enabled with realistic breaking
- Placed on tables, shelves, mantles
- Particle effects when destroyed

#### Furniture (15 points)
- Wooden tables, chairs, stands
- Fabric upholstery for scratching
- Scratch marks appear when damaged
- Can be pushed over or scratched

#### Curtains (20 points)
- Fabric simulation for realistic movement
- Hanging from curtain rods
- Can climb and shred
- Tears and damage accumulate
- Cloth physics for natural draping

#### Food Items (25 points)
- Realistic food models (fish, chicken, treats)
- Placed on counters, tables, floor
- Pickup and "eat" animation for cat
- Disappears when collected

### Lighting Design
**Dynamic Day/Night Cycle (Optional):**
- Sunlight through windows (day)
- Warm interior lighting (evening)
- Shadows and ambient occlusion

**Light Sources:**
- Directional light (sun/moon)
- Point lights (lamps, ceiling fixtures)
- Spot lights (focused areas)
- Emissive materials (TV, appliances)

**Mood:**
- Warm, lived-in atmosphere
- Cozy home environment
- Good visibility for gameplay
- Dramatic shadows for stealth

## Visual Effects

### Particle Systems
- Dust particles when knocking objects
- Shatter effects for breaking items
- Sparkles for collecting food
- Claw scratch marks

### Post-Processing
- Color grading for warm home atmosphere
- Bloom for bright surfaces
- Depth of field for cinematic moments
- Motion blur during fast movement

### UI Design
**HUD Elements:**
- Score counter (top right)
- Mischief meter/progress bar
- Stealth indicator (when near humans)
- Interaction prompts
- Minimalist, non-intrusive design

**Menus:**
- Main menu with 3D cat model
- Pause menu
- Game over screen with final score
- Clean, modern UI design

## Camera & Controls

### Camera Setup
- Third-person over-the-shoulder view
- Dynamic distance based on environment
- Smooth follow with lag
- Collision detection to prevent clipping
- Optional zoom in/out

### Visual Feedback
- Object highlighting when in interaction range
- Vision cone visualization for humans (subtle)
- Cat visual state (normal vs. sprinting)
- Environmental cues for stealth

## Performance Targets
- **60 FPS** on mid-range gaming PC
- **1080p** minimum, 4K support
- Scalable graphics settings
- Optimized LODs for all assets
- Efficient collision meshes

## Reference Style

### Visual Inspirations
- **Stray**: Realistic cat movement and environment detail
- **Untitled Goose Game**: Playful mischief mechanics (but in 3D!)
- **Uncharted**: Quality third-person camera work
- **The Last of Us Part II**: Realistic indoor environments
- **What Remains of Edith Finch**: Detailed house interiors

### Color Palette
- Warm wood tones (browns, tans)
- Soft fabric colors (beiges, grays, whites)
- Natural lighting (warm yellows, cool blues)
- Pop of color from decorations
- Cat coat color as visual anchor

## Audio-Visual Integration (Future)
- Footstep sounds synced to animation
- Object collision sounds
- Ambient house sounds
- Music adapts to gameplay state
- Human dialogue/reactions

---

This design document serves as the visual target for CATastrophe's Unreal Engine implementation.
