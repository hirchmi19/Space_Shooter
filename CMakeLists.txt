cmake_minimum_required(VERSION 3.15)

project(Space_Shooter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/raylib)

add_executable(Space_Shooter
        src/main.cpp
        src/Game.cpp
        src/Game.h
        src/Entities/Player.cpp
        src/Entities/Player.h
        src/Constants/GameConstants.h
        src/Systems/Assets/TextureID.h
        src/Systems/Assets/SpriteID.h
        src/Systems/Assets/Sprite.h
        src/Systems/Assets/AssetSystem.cpp
        src/Systems/Assets/AssetSystem.h
        src/GameWorld.cpp
        src/GameWorld.h
        src/Systems/IGameSystem.h
        src/Entities/EntityID.h
        src/Entities/Entity.h
        src/Utilities/utils.h
        src/Systems/GameSystemID.h
        src/Entities/Enemy.cpp
        src/Entities/Enemy.h
        src/Entities/Projectile.cpp
        src/Entities/Projectile.h
        src/Systems/Rendering/RenderSystem.cpp
        src/Systems/Rendering/RenderSystem.h
)

target_link_libraries(Space_Shooter raylib)