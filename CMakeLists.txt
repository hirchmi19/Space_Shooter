cmake_minimum_required(VERSION 3.15)

project(Space_Shooter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

include(FetchContent)
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(Space_Shooter ${SOURCES}
        src/Locators/IWaveLocator.h)

target_compile_definitions(Space_Shooter PRIVATE
        ASSETS_PATH="${CMAKE_SOURCE_DIR}/assets/"
)

target_include_directories(Space_Shooter PRIVATE src)

target_link_libraries(Space_Shooter PRIVATE raylib)
