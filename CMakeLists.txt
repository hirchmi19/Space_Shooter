cmake_minimum_required(VERSION 3.15)

project(Space_Shooter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

add_subdirectory(external/raylib)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(Space_Shooter ${SOURCES}
        src/Constants/ScoringConstants.h
        src/SystemService/SystemService.cpp
        src/SystemService/SystemService.h
        src/SystemService/IEntityService.cpp
        src/SystemService/IEntityService.h
        src/Systems/Assets/AssetHeader/Assets.h
        src/Systems/Waving/WaveHeader/Wave.h
        src/SystemService/IAssetService.cpp
        src/SystemService/IAssetService.h
        src/SystemService/IScoreService.cpp
        src/SystemService/IScoreService.h
        src/Systems/Entities/EntitySystem.cpp
        src/Systems/Entities/EntitySystem.h
)

target_compile_definitions(Space_Shooter PRIVATE 
    ASSETS_PATH="${CMAKE_SOURCE_DIR}/assets/"
)

target_include_directories(Space_Shooter PRIVATE src)

target_link_libraries(Space_Shooter PRIVATE raylib)
