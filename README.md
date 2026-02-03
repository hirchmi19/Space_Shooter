# Space_Shooter

A small **C++** project using **raylib**, built with **CMake**.  
The project uses **Git submodules**, so no global raylib installation is required.

---

# Requirements:

- C++ compiler (Clang, GCC, or MSVC)
- CMake ≥ 3.15
- Git

# Clone the Project:


`git clone --recurse-submodules https://github.com/hirchmi19/Space_Shooter.git` <br>
`cd Space_Shooter`

### The `--recurse-submodules` flag is required to automatically clone raylib.

**if you forget: `git submodule update --init --recursive`**

# Build & Run:

`cmake -S . -B build` <br>
`cmake --build build`

### Run the exe (example):

`./build/Space_Shooter`



