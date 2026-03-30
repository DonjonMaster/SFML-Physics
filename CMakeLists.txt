cmake_minimum_required(VERSION 3.16)
project(SFMLPhysics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Par defaut, on force Debug pour les generateurs mono-config (Unix Makefiles/Ninja).
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
)

FetchContent_MakeAvailable(SFML)

add_executable(sfml_test
    src/main.cpp
    src/Ball.cpp
    src/Platform.cpp
    src/Engine/ColliderComponent.cpp
    src/Engine/Engine.cpp
    src/Engine/scenes/PhysicsScene.cpp
)

target_link_libraries(sfml_test PRIVATE SFML::Graphics SFML::Window SFML::System)

# Symboles de debug et optimisation desactivee en mode Debug.
target_compile_options(sfml_test PRIVATE
    $<$<AND:$<CONFIG:Debug>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-g3 -O0>
    $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:/Zi /Od>
)
