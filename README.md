# Space_Shooter

A small but structured **C++ arcade-style space shooter** built with **raylib** and **CMake**.
The project uses Raylib version 5.5, which is installed by its CMake config. This was my first C++ project and the goal was to get hands-on experience with the language by building something real.

## Game

<p align="center">
  <img src="assets/screenshots/gameplay.png" width="45%">
  <img src="assets/screenshots/game_over.png" width="45%">  
</p>

**Controls:** 
- W | ← : Move left
- D | → : Move right
- SPACE : Shoot

**Gameplay:**
Survive endless procedurally generated waves of enemies in a classic formation-based space shooter. Destroy enemies to build your score multiplier, enter Flow State for increased speed and firepower, collect power-ups, and hit score milestones to unlock permanent upgrades.

- Procedural wave generation, e.g random group sizes, dive counts and enemy compositions each wave
- Five Bézier curve movement types for enemy entry paths 
- Five enemy types with randomized formation placement
- Multiplier system that builds as you kill enemies and decays over time
- Flow State which triggers at high multipliers, boosting speed and unlocking dash and rapid fire
- Collectible power-ups: projectile types 
- Shield system with HP and cooldown
- Rocket explosions with area damage

**Architecture:**
The project uses a system-based architecture with a service locator for cross-system communication and a component-based entity model.

Key design decisions:

- Fixed timestep game loop decoupled from frame rate
- Index timer system (pointer-safe, no heap allocation per timer)
- Deferred entity removal with sorted index queues to avoid mid-loop invalidation
- Templated FindDeadEntities / KillEntities working across all entity types
- Compile-time sprite tables (SpriteTable.h) replacing runtime if-chains
- Bézier curves for all enemy movement paths, defined in a dedicated file
- Procedural wave generation — group sizes, dive types and formation composition randomized each wave

# Requirements:

- C++20 compiler (Clang or GCC)
- CMake ≥ 3.15
- Git
- Raylib 5.5

# Clone the Project:

`git clone https://github.com/hirchmi19/Space_Shooter.git` <br>
`cd Space_Shooter`

# Build & Run:

`mkdir build`<br>
`cd build`<br>
`cmake -B build` <br>
`cmake --build build`

### Run the exe (example):

`./build/Space_Shooter`

# Credits

### Libraries
- raylib – https://www.raylib.com/

### Assets

- Public Pixel Font – CC0 (Public Domain)  
  https://ggbot.itch.io/public-pixel-font

- “Space Eaters” sprites by cluly – Used under the license provided on itch.io
  https://cluly.itch.io/space-eaters

- Player sprite by Gustavo Vituri – Used under the license provided on itch.io
  https://gvituri.itch.io/space-shooter



