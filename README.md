# Space_Shooter

A small but structured **C++ arcade-style space shooter** built with **raylib** and **CMake**.
The project does not require a global raylib installation thanks to Git submodules.

---

# Requirements:

- C++ compiler (Clang, GCC, or MSVC)
- CMake ≥ 3.15
- Git

# Clone the Project:


`git clone --recurse-submodules https://github.com/hirchmi19/Space_Shooter.git` <br>
`cd Space_Shooter`

## The `--recurse-submodules` flag is required to automatically clone raylib.

**if you forget:** `git submodule update --init --recursive`

**If raylib updates or you switch branches:** `git submodule update --recursive --remote`

# Build & Run:

`cmake -S . -B build` <br>
`cmake --build build`

### Run the exe (example):

`./build/Space_Shooter`


# Projec Sructure:

Space_Shooter/
│
├── src/                # Game source code
│   ├── Game/
│   ├── Systems/
│   ├── Entities/
│   ├── Constants/
│   └── main.cpp
│
├── external/
│   └── raylib/         # Git submodule
│
├── CMakeLists.txt
└── README.md

# Architecture Overview:

**The project follows a system-oriented structure:**

GameWorld manages entities

Systems operate on entities (Movement, Rendering, Wave, etc.)

Assets are mapped via enum → texture lookup



