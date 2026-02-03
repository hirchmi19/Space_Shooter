cmake_minimum_required(VERSION 3.15)

project(Space_Shooter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/raylib)

add_executable(Space_Shooter
        src/main.cpp
        src/Game.cpp
        src/Game.h
        src/Player.cpp
        src/Player.h)

target_link_libraries(Space_Shooter raylib)