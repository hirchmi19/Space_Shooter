# Space_Shooter

A small but structured **C++ arcade-style space shooter** built with **raylib** and **CMake**.
The project uses Raylib version 5.5, which is installed by its CMake config. 

## Game

<p align="center">
  <img src="assets/screenshots/gameplay.png" width="45%">
  <img src="assets/screenshots/game_over.png" width="45%">  
</p>

**Controls:** 
- W | ← : Move left
- D | → : Move right
- SPACE : Shoot

# Requirements:

- C++ compiler (Clang, GCC, or MSVC)
- CMake ≥ 3.15
- Git
- Raylib 5.5

# Clone the Project:


`git clone --recurse-submodules https://github.com/hirchmi19/Space_Shooter.git` <br>
`cd Space_Shooter`

 **The `--recurse-submodules` flag is required to automatically clone raylib.**

**if you forget:** `git submodule update --init --recursive`

**If raylib updates or you switch branches:** `git submodule update --recursive --remote`

# Build & Run:

`cmake -B cmake-build-debug` <br>
`cmake --build cmake-build-debug`

### Run the exe (example):

`./cmake-build-debug/Space_Shooter`

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



